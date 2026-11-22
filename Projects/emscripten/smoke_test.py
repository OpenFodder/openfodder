"""Browser integration checks; requires a build with WEB_DIAGNOSTICS=ON."""
import argparse
import asyncio
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import json
from pathlib import Path
import re
import threading

from playwright.async_api import async_playwright

CAMPAIGNS = ("Cannon Fodder Plus", "Amiga The One", "Amiga Action",
             "Amiga Power Alien Levels", "Amiga Format Christmas Special",
             "Amiga Format Not Very Festive", "PC Format")
STATE = "(runtimeInitialized ? JSON.parse(Module.UTF8ToString(Module._openfodder_web_state())) : {})"


class QuietHandler(SimpleHTTPRequestHandler):
    def log_message(self, *args):
        pass


async def open_game(browser, url, arguments=None, viewport=None):
    context = await browser.new_context(viewport=viewport or {"width": 1280, "height": 1000})
    page = await context.new_page()
    errors = []
    page.on("pageerror", lambda error: errors.append(str(error)))
    if arguments is not None:
        async def replace_arguments(route):
            response = await route.fetch()
            html, count = re.subn(r"arguments:\s*\[[^\]]*\]",
                                 "arguments: " + json.dumps(arguments), await response.text())
            assert count == 1, "Could not set smoke-check arguments"
            await route.fulfill(response=response, body=html)
        await page.route("**/OpenFodder.html", replace_arguments)
    await page.goto(url)
    await page.wait_for_function("typeof runtimeInitialized !== 'undefined' && runtimeInitialized && typeof Module._openfodder_web_state === 'function'")
    return context, page, errors


async def check_menu(browser, url, artifacts):
    context, page, errors = await open_game(browser, url)
    try:
        await page.wait_for_function(f"({STATE}).campaigns === 7", timeout=15000)
        assert not await page.locator("#output").text_content(), "Unexpected startup warning"
        await page.screenshot(path=str(artifacts / "menu.png"))
        for viewport in ({"width": 1280, "height": 1000}, {"width": 800, "height": 850}):
            await page.set_viewport_size(viewport)
            box = await page.locator("canvas").bounding_box()
            dimensions = await page.locator("canvas").evaluate("c => [c.width, c.height]")
            for x, y in ((0, 0), (dimensions[0] - 1, 0), (0, 60), (90, 0), (120, 90)):
                await page.mouse.move(box["x"] + x * box["width"] / dimensions[0],
                                      box["y"] + y * box["height"] / dimensions[1])
                await page.wait_for_function(
                    f"Math.abs(({STATE}).eventX - {x}) <= 1 && Math.abs(({STATE}).eventY - {y}) <= 1")
            # Release at zero: the old web filter left mouse buttons stuck here.
            await page.mouse.down()
            await page.wait_for_timeout(80)
            await page.mouse.move(box["x"], box["y"])
            await page.mouse.up()
            await page.wait_for_function(f"({STATE}).buttons === 0 && ({STATE}).eventX === 0 && ({STATE}).eventY === 0")
        await page.evaluate("""() => new Promise((resolve, reject) => {
            FS.writeFile('/Saves/browser-smoke.json', '{"ok":true}');
            FS.syncfs(false, error => error ? reject(error) : resolve());
        })""")
        await page.reload()
        await page.wait_for_function(f"({STATE}).campaigns === 7")
        assert await page.evaluate("FS.readFile('/Saves/browser-smoke.json', {encoding:'utf8'})") == '{"ok":true}'
        assert not errors, errors
        print("PASS: seven campaigns, zero-coordinate input at two canvas scales, persistent saves", flush=True)
    finally:
        await context.close()


async def check_campaign(browser, url, name, index, artifacts):
    args = ["--skipintro", "--alternate-mouse", "--window", "--campaign", name,
            "--mission", "1", "--phase", "1", "--skipbriefing"]
    context, page, errors = await open_game(browser, url, args)
    try:
        await page.wait_for_function(f"({STATE}).engine >= 3", timeout=20000)
        assert not await page.locator("#output").text_content(), f"Unexpected startup warning in {name}"
        before = await page.evaluate(STATE)
        await page.wait_for_timeout(1500)
        after = await page.evaluate(STATE)
        interrupts = after["interrupt"] - before["interrupt"]
        ticks = after["engine"] - before["engine"]
        # The engine runs one logic tick per three 50 Hz interrupt ticks.
        assert 45 <= interrupts <= 90, (name, before, after)
        assert 14 <= ticks <= 31 and abs(interrupts - 3 * ticks) <= 5, (name, before, after)
        if name == "PC Format":
            await check_pc_audio(page)
        await page.screenshot(path=str(artifacts / f"demo-{index}.png"))
        assert not errors, (name, errors)
        print(f"PASS: {name}: {interrupts} interrupts / {ticks} logic ticks", flush=True)
    finally:
        await context.close()


async def check_pc_audio(page):
    # Observe the real Web Audio output after SDL/mixer have filled it. This
    # catches a silent backend or missing decoder even when the game runs.
    assert await page.evaluate("Module._openfodder_web_play_audio(0)") == 1, "PC mixer backend is inactive"
    await page.wait_for_timeout(700)  # finish the music fade
    await page.evaluate("""() => {
        const node = Module.SDL3.audio_playback.scriptProcessorNode;
        const original = node.onaudioprocess;
        window.audioProbe = {frames:0, peak:0};
        node.onaudioprocess = function(event) {
            original.call(this, event);
            const samples = event.outputBuffer.getChannelData(0);
            audioProbe.frames += samples.length;
            for (const sample of samples) audioProbe.peak = Math.max(audioProbe.peak, Math.abs(sample));
        };
    }""")
    for kind, label in ((2, "VOC effects"), (1, "MOD music")):
        await page.evaluate("audioProbe = {frames:0, peak:0}")
        assert await page.evaluate(f"Module._openfodder_web_play_audio({kind})") == 1
        await page.wait_for_function("audioProbe.frames > 0 && audioProbe.peak > 0.01", timeout=5000)
        probe = await page.evaluate("audioProbe")
        print(f"PASS: PC {label}: Web Audio peak={probe['peak']:.3f}", flush=True)
        if kind == 2:
            await page.wait_for_timeout(1000)  # let the effect finish before testing music
    assert not await page.locator("#output").text_content(), "PC audio reported a decoder error"


async def check_audio_unlock(playwright, args, url):
    # No autoplay exemption: match a fresh browser profile before its first click.
    browser = await playwright.chromium.launch(executable_path=args.chromium,
        args=["--autoplay-policy=document-user-activation-required"])
    try:
        context, page, errors = await open_game(browser, url,
            ["--skipintro", "--alternate-mouse", "--window", "--campaign", "PC Format",
             "--mission", "1", "--phase", "1", "--skipbriefing"])
        try:
            await page.wait_for_function(f"({STATE}).engine >= 3", timeout=20000)
            initial_state = await page.evaluate("Module.SDL3.audioContext.state")
            assert initial_state in ("suspended", "running"), initial_state
            await page.locator("canvas").click(position={"x": 400, "y": 300})
            await page.wait_for_function("Module.SDL3.audioContext.state === 'running'")
            await check_pc_audio(page)
            assert not errors, errors
            print(f"PASS: PC audio after a normal browser click (initial state: {initial_state})", flush=True)
        finally:
            await context.close()
    finally:
        await browser.close()


async def run(args, url):
    args.artifacts.mkdir(parents=True, exist_ok=True)
    async with async_playwright() as playwright:
        browser = await playwright.chromium.launch(executable_path=args.chromium,
            args=["--autoplay-policy=no-user-gesture-required"])
        try:
            if args.production_only:
                context = await browser.new_context(viewport={"width": 1280, "height": 1000})
                page = await context.new_page()
                errors = []
                page.on("pageerror", lambda error: errors.append(str(error)))
                await page.goto(url)
                await page.wait_for_function("document.getElementById('status').textContent.startsWith('Ready')")
                await page.wait_for_timeout(2000)
                assert not await page.locator("#output").text_content(), "Packaged build reported an error"
                assert await page.evaluate("typeof Module._openfodder_web_state") == "undefined"
                assert await page.evaluate("typeof Module._openfodder_web_play_audio") == "undefined"
                screenshot = await page.locator("canvas").screenshot(path=str(args.artifacts / "production.png"))
                assert len(screenshot) > 10000, "The packaged game canvas appears blank"
                assert not errors, errors
                await context.close()
                print("PASS: extracted distribution starts and renders without diagnostic exports", flush=True)
                return
            await check_menu(browser, url, args.artifacts)
            for index, name in enumerate(CAMPAIGNS):
                await check_campaign(browser, url, name, index, args.artifacts)
        finally:
            await browser.close()
        await check_audio_unlock(playwright, args, url)


def main():
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, default=root / "RunE")
    parser.add_argument("--artifacts", type=Path, default=root / "build" / "web-smoke")
    parser.add_argument("--chromium", help="Use an installed Chromium/Chrome executable")
    parser.add_argument("--production-only", action="store_true",
                        help="Check a packaged distribution without diagnostic exports")
    args = parser.parse_args()
    server = ThreadingHTTPServer(("127.0.0.1", 0), partial(QuietHandler, directory=str(args.runtime)))
    threading.Thread(target=server.serve_forever, daemon=True).start()
    try:
        asyncio.run(run(args, f"http://127.0.0.1:{server.server_port}/OpenFodder.html"))
    finally:
        server.shutdown()
        server.server_close()


if __name__ == "__main__":
    main()
