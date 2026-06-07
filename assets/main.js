/* =============================================================================
 * Open Fodder — assets/main.js
 * Vanilla JS, no dependencies, no build step.
 * IIFE wrapper; safe to run before or after DOMContentLoaded thanks to the
 * defer attribute, but we still guard for readyState.
 * =============================================================================
 */
(function () {
  'use strict';

  // ---------------------------------------------------------------------------
  // Boot guard: run once DOM is parsed.
  // ---------------------------------------------------------------------------
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', boot, { once: true });
  } else {
    boot();
  }

  // ---------------------------------------------------------------------------
  // Shared state
  //
  // NOTE on hoisting: prefersReducedMotion and REDUCED_MOTION need to be
  // available the moment boot() runs. boot() is called from line ~17 (in the
  // readyState branch above) BEFORE this section is executed top-to-bottom.
  // Function declarations (function foo() {}) are fully hoisted; var
  // assignments (var foo = function () {}) are not — only the `var foo`
  // hoists, the value stays undefined until the line is executed. So:
  //   - prefersReducedMotion uses `function` declaration form (hoisted).
  //   - REDUCED_MOTION is wrapped in a memoised lazy getter so the actual
  //     matchMedia call happens at first use (after the IIFE has finished
  //     initialising), not at the top-of-file evaluation order.
  // ---------------------------------------------------------------------------
  var _REDUCED_MOTION = null;
  function getReducedMotion() {
    if (_REDUCED_MOTION) return _REDUCED_MOTION;
    _REDUCED_MOTION = window.matchMedia
      ? window.matchMedia('(prefers-reduced-motion: reduce)')
      : { matches: false, addEventListener: function () {}, addListener: function () {} };
    return _REDUCED_MOTION;
  }

  function prefersReducedMotion() { return !!getReducedMotion().matches; }

  // Cross-browser "media query change" subscription. Returns an unsubscribe fn.
  function onMotionChange(handler) {
    var mql = getReducedMotion();
    if (mql.addEventListener) {
      mql.addEventListener('change', handler);
      return function () { mql.removeEventListener('change', handler); };
    }
    if (mql.addListener) {
      mql.addListener(handler);
      return function () { mql.removeListener(handler); };
    }
    return function () {};
  }

  // ---------------------------------------------------------------------------
  function boot() {
    // Each handler is wrapped so one failure does not block the rest.
    // Critically: the page is already fully rendered and visible at this
    // point; nothing JS does is required for content to show.
    var safe = function (name, fn) {
      try { fn(); }
      catch (err) {
        if (window && window.console) window.console.warn('[OF]', name, 'failed:', err);
      }
    };

    safe('prefersReducedMotionGate', prefersReducedMotionGate);
    safe('yearStamp', yearStamp);
    safe('mobileNavToggle', mobileNavToggle);
    safe('smoothScrollAnchors', smoothScrollAnchors);
    safe('heroParallaxLayers', heroParallaxLayers);
    safe('activeNavLinkOnScroll', activeNavLinkOnScroll);
    safe('typewriter', typewriter);
    safe('coordReadout', coordReadout);
    safe('hudFrameDrawIn', hudFrameDrawIn);
    safe('ledPulse', ledPulse);
    safe('marqueeSeamless', marqueeSeamless);
  }

  // ---------------------------------------------------------------------------
  // prefersReducedMotionGate — toggles body.no-motion when the OS pref changes
  // ---------------------------------------------------------------------------
  function prefersReducedMotionGate() {
    var apply = function () {
      document.body.classList.toggle('no-motion', prefersReducedMotion());
    };
    apply();
    onMotionChange(apply);
  }

  // ---------------------------------------------------------------------------
  // yearStamp — fills [data-current-year] AND #footer-year with current year.
  // ---------------------------------------------------------------------------
  function yearStamp() {
    var year = String(new Date().getFullYear());
    var nodes = document.querySelectorAll('[data-current-year]');
    for (var i = 0; i < nodes.length; i++) nodes[i].textContent = year;
    var footer = document.getElementById('footer-year');
    if (footer) footer.textContent = year;
  }

  // ---------------------------------------------------------------------------
  // mobileNavToggle — hamburger button toggles aria-expanded + .is-open + .nav--open
  // ---------------------------------------------------------------------------
  function mobileNavToggle() {
    var toggle = document.querySelector('[data-nav-toggle]');
    if (!toggle) return;

    var targetSel = toggle.getAttribute('data-nav-target')
      || (toggle.getAttribute('aria-controls') ? '#' + toggle.getAttribute('aria-controls') : null)
      || '.nav__panel';
    var panel = document.querySelector(targetSel);
    var nav = toggle.closest('.nav') || (panel && panel.closest('.nav'));

    var setOpen = function (open) {
      toggle.setAttribute('aria-expanded', open ? 'true' : 'false');
      if (panel) panel.classList.toggle('is-open', open);
      if (nav) nav.classList.toggle('nav--open', open);
    };

    setOpen(toggle.getAttribute('aria-expanded') === 'true');

    toggle.addEventListener('click', function (e) {
      e.preventDefault();
      setOpen(toggle.getAttribute('aria-expanded') !== 'true');
    });

    document.addEventListener('keydown', function (e) {
      if (e.key === 'Escape' && toggle.getAttribute('aria-expanded') === 'true') setOpen(false);
    });

    document.addEventListener('click', function (e) {
      if (toggle.getAttribute('aria-expanded') !== 'true') return;
      if (toggle.contains(e.target)) return;
      if (panel && panel.contains(e.target)) return;
      setOpen(false);
    });

    if (panel) {
      panel.addEventListener('click', function (e) {
        var a = e.target.closest && e.target.closest('a[href^="#"]');
        if (a) setOpen(false);
      });
    }
  }

  // ---------------------------------------------------------------------------
  // smoothScrollAnchors — any a[href^="#"] scrolls smoothly; instant under
  // reduced motion. Routes "#main" / skip-link clicks to the <main> landmark.
  // ---------------------------------------------------------------------------
  function smoothScrollAnchors() {
    document.addEventListener('click', function (e) {
      var a = e.target.closest && e.target.closest('a[href^="#"]');
      if (!a) return;
      var hash = a.getAttribute('href');
      if (!hash || hash === '#' || hash.length < 2) return;
      var target = document.getElementById(hash.slice(1));
      if (!target) return;

      e.preventDefault();
      var behavior = prefersReducedMotion() ? 'auto' : 'smooth';
      try {
        target.scrollIntoView({ behavior: behavior, block: 'start' });
      } catch (_) {
        target.scrollIntoView();
      }
      // Move keyboard focus to the target for accessibility.
      if (!target.hasAttribute('tabindex')) target.setAttribute('tabindex', '-1');
      target.focus({ preventScroll: true });
    });
  }

  // ---------------------------------------------------------------------------
  // heroParallaxLayers — translates layer by scrollY*rate, clamped to ±40px.
  // Disabled under reduced motion AND re-checks live when the OS pref flips.
  // ---------------------------------------------------------------------------
  function heroParallaxLayers() {
    var layers = document.querySelectorAll('.hero__art-bg, [data-parallax]');
    if (!layers.length) return;

    var MAX = 40;
    var ticking = false;
    var attached = false;

    var clamp = function (v, lo, hi) { return Math.max(lo, Math.min(hi, v)); };

    var resetLayers = function () {
      for (var i = 0; i < layers.length; i++) {
        layers[i].style.setProperty('--parallax-y', '0px');
      }
    };

    var update = function () {
      ticking = false;
      var y = window.pageYOffset || document.documentElement.scrollTop || 0;
      for (var i = 0; i < layers.length; i++) {
        var el = layers[i];
        var rate = parseFloat(el.getAttribute('data-parallax')) || 0.1;
        var offset = clamp(y * rate, -MAX, MAX);
        el.style.setProperty('--parallax-y', offset.toFixed(2) + 'px');
      }
    };

    var onScroll = function () {
      if (ticking) return;
      ticking = true;
      requestAnimationFrame(update);
    };

    var attach = function () {
      if (attached) return;
      window.addEventListener('scroll', onScroll, { passive: true });
      window.addEventListener('resize', onScroll, { passive: true });
      attached = true;
      update();
    };

    var detach = function () {
      if (!attached) return;
      window.removeEventListener('scroll', onScroll);
      window.removeEventListener('resize', onScroll);
      attached = false;
      resetLayers();
    };

    if (!prefersReducedMotion()) attach();

    // Live-respond to OS-level reduced-motion changes mid-session.
    onMotionChange(function () {
      if (prefersReducedMotion()) detach();
      else attach();
    });
  }

  // ---------------------------------------------------------------------------
  // activeNavLinkOnScroll
  // ---------------------------------------------------------------------------
  function activeNavLinkOnScroll() {
    if (!('IntersectionObserver' in window)) return;
    var links = document.querySelectorAll('.nav__link[href^="#"]');
    if (!links.length) return;

    var linkBySection = {};
    var sections = [];
    for (var i = 0; i < links.length; i++) {
      var id = links[i].getAttribute('href').slice(1);
      var sec = document.getElementById(id);
      if (sec) {
        linkBySection[id] = links[i];
        sections.push(sec);
      }
    }
    if (!sections.length) return;

    var setActive = function (id) {
      for (var k in linkBySection) {
        if (Object.prototype.hasOwnProperty.call(linkBySection, k)) {
          if (k === id) linkBySection[k].setAttribute('aria-current', 'true');
          else linkBySection[k].removeAttribute('aria-current');
        }
      }
    };

    var visible = {};
    var io = new IntersectionObserver(function (entries) {
      entries.forEach(function (entry) {
        visible[entry.target.id] = entry.isIntersecting ? entry.intersectionRatio : 0;
      });
      var bestId = null, bestRatio = 0;
      for (var id in visible) {
        if (visible[id] > bestRatio) { bestRatio = visible[id]; bestId = id; }
      }
      if (bestId) setActive(bestId);
    }, { threshold: [0.25, 0.5, 0.75] });

    for (var s = 0; s < sections.length; s++) io.observe(sections[s]);
  }

  // ---------------------------------------------------------------------------
  // typewriter — for every [data-typewriter], read data-typewriter-text and
  // either type it character-by-character (default) or write it instantly under
  // reduced motion. Always seeds the visible text so a no-JS / pre-JS frame
  // shows full content (the seed runs synchronously at boot).
  // Optional: data-typewriter-delay (ms before start), data-typewriter-speed
  // (ms per char, default 35).
  // ---------------------------------------------------------------------------
  function typewriter() {
    var nodes = document.querySelectorAll('[data-typewriter]');
    if (!nodes.length) return;

    // Stage 1 (synchronous): make every span show its full text immediately.
    // This is the accessibility baseline — sighted users, screen readers, and
    // search engines all see the real content even if JS animation is skipped.
    var entries = [];
    for (var i = 0; i < nodes.length; i++) {
      var el = nodes[i];
      var text = el.getAttribute('data-typewriter-text');
      if (text == null || text === '') {
        // Allow inline fallback text — if the span already has text, use that.
        text = el.textContent || '';
      }
      el.textContent = text;
      // Mark as seeded so screen readers don't re-announce when we re-type.
      el.setAttribute('aria-label', text);
      entries.push({ el: el, text: text });
    }

    // Under reduced motion, we are done — full text already in place.
    if (prefersReducedMotion()) return;

    // Stage 2 (animated): wipe and re-type each entry on a stagger.
    // Use aria-hidden during the typing pass so AT users hear the final string
    // (already exposed via aria-label) once, not character by character.
    entries.forEach(function (entry, idx) {
      var el = entry.el;
      var text = entry.text;
      var speed = parseInt(el.getAttribute('data-typewriter-speed'), 10);
      if (!speed || speed < 0) speed = 35;
      var startDelay = parseInt(el.getAttribute('data-typewriter-delay'), 10);
      if (isNaN(startDelay) || startDelay < 0) startDelay = idx * 220;

      el.setAttribute('aria-hidden', 'true');
      el.textContent = '';
      var pos = 0;

      var step = function () {
        // Defensive: bail mid-flight if reduced-motion got toggled on.
        if (prefersReducedMotion()) {
          el.textContent = text;
          el.removeAttribute('aria-hidden');
          return;
        }
        pos++;
        el.textContent = text.slice(0, pos);
        if (pos >= text.length) {
          el.removeAttribute('aria-hidden');
          el.classList.add('typewriter--done');
          return;
        }
        setTimeout(step, speed);
      };

      setTimeout(step, startDelay);
    });

    // If the OS pref flips on mid-typing, snap everything to final text.
    onMotionChange(function () {
      if (!prefersReducedMotion()) return;
      for (var i = 0; i < entries.length; i++) {
        entries[i].el.textContent = entries[i].text;
        entries[i].el.removeAttribute('aria-hidden');
      }
    });
  }

  // ---------------------------------------------------------------------------
  // coordReadout — rotates short coordinate strings through [data-coord-readout]
  // every ~2.4s. Frozen under reduced motion (shows the first string).
  // ---------------------------------------------------------------------------
  function coordReadout() {
    var nodes = document.querySelectorAll('[data-coord-readout]');
    if (!nodes.length) return;

    var defaultStrings = [
      'GRID 17.42N // 04.18E   ELEV +12m   WIND 06kt NE',
      'GRID 17.49N // 04.22E   ELEV +14m   WIND 05kt NE',
      'GRID 17.51N // 04.27E   ELEV +15m   WIND 07kt N',
      'GRID 17.55N // 04.31E   ELEV +13m   WIND 08kt N',
      'SIGNAL +OK   COMMS NOMINAL   ROUTE GREEN',
      'CONTACT NEG   PERIMETER CLEAR   STATUS GREEN'
    ];

    nodes.forEach(function (node) {
      var raw = node.getAttribute('data-coord-readout');
      var strings;
      if (raw && raw.indexOf('|') !== -1) {
        strings = raw.split('|').map(function (s) { return s.trim(); }).filter(Boolean);
      } else if (raw && raw.length > 0) {
        strings = [raw];
      } else {
        strings = defaultStrings.slice();
      }

      // Seed first string immediately.
      node.textContent = strings[0];

      if (prefersReducedMotion() || strings.length < 2) return;

      var idx = 0;
      var period = parseInt(node.getAttribute('data-coord-interval'), 10) || 2400;
      var timer = setInterval(function () {
        if (prefersReducedMotion()) return; // pause silently if pref flips
        idx = (idx + 1) % strings.length;
        node.textContent = strings[idx];
      }, period);

      // Pause when tab is hidden — saves cycles, avoids surprise updates.
      document.addEventListener('visibilitychange', function () {
        if (document.hidden) {
          clearInterval(timer);
        }
      });
    });
  }

  // ---------------------------------------------------------------------------
  // hudFrameDrawIn — when [data-hud-frame] enters the viewport, add
  // .hud-frame--drawn so CSS can run its corner-bracket draw-in animation.
  // No-op without IntersectionObserver (we just mark them drawn immediately).
  // ---------------------------------------------------------------------------
  function hudFrameDrawIn() {
    var frames = document.querySelectorAll('[data-hud-frame]');
    if (!frames.length) return;

    if (!('IntersectionObserver' in window) || prefersReducedMotion()) {
      for (var i = 0; i < frames.length; i++) {
        frames[i].classList.add('hud-frame--drawn');
      }
      return;
    }

    var io = new IntersectionObserver(function (entries) {
      entries.forEach(function (entry) {
        if (entry.isIntersecting && entry.intersectionRatio >= 0.2) {
          entry.target.classList.add('hud-frame--drawn');
          io.unobserve(entry.target);
        }
      });
    }, { threshold: [0.2, 0.5] });

    for (var j = 0; j < frames.length; j++) io.observe(frames[j]);
  }

  // ---------------------------------------------------------------------------
  // ledPulse — blink class on [data-led-pulse] every ~1100ms. Static under
  // reduced motion (just marks the LED "on").
  // ---------------------------------------------------------------------------
  function ledPulse() {
    var leds = document.querySelectorAll('[data-led-pulse]');
    if (!leds.length) return;

    for (var i = 0; i < leds.length; i++) leds[i].classList.add('led--on');

    if (prefersReducedMotion()) return;

    var on = true;
    var period = 1100;
    var loop = function () {
      on = !on;
      for (var j = 0; j < leds.length; j++) {
        leds[j].classList.toggle('led--pulse', on);
      }
      setTimeout(loop, period);
    };
    setTimeout(loop, period);

    onMotionChange(function () {
      if (!prefersReducedMotion()) return;
      for (var k = 0; k < leds.length; k++) {
        leds[k].classList.remove('led--pulse');
        leds[k].classList.add('led--on');
      }
    });
  }

  // ---------------------------------------------------------------------------
  // marqueeSeamless — ensure the marquee track has exactly TWO sets of items
  // so a -50% transform wraps cleanly.
  //
  // The HTML already ships the duplicate inline (every clone marked
  // aria-hidden="true"). If those clones are present we leave it alone.
  // Otherwise — e.g. some future page that omits them — we duplicate once.
  // Without this guard, the JS would add a third set on top of the inline
  // duplicate, producing the "scrolling text on two stacked lines" bug.
  // ---------------------------------------------------------------------------
  function marqueeSeamless() {
    // Target the inner track (the element being translated). Falls back to
    // [data-marquee] itself for older markup that didn't have a __track child.
    var tracks = document.querySelectorAll('[data-marquee] > .marquee__track');
    if (!tracks.length) tracks = document.querySelectorAll('[data-marquee]');
    if (!tracks.length) return;

    for (var i = 0; i < tracks.length; i++) {
      var track = tracks[i];
      if (track.getAttribute('data-marquee-cloned') === 'true') continue;

      var children = track.children;
      if (!children || children.length === 0) continue;

      // If the HTML already has clones (any child carrying aria-hidden="true"),
      // assume the duplicate set is already there and don't add another.
      var hasInlineClones = false;
      for (var k = 0; k < children.length; k++) {
        if (children[k].getAttribute && children[k].getAttribute('aria-hidden') === 'true') {
          hasInlineClones = true;
          break;
        }
      }
      track.setAttribute('data-marquee-cloned', 'true');
      if (hasInlineClones) continue;

      var frag = document.createDocumentFragment();
      for (var c = 0; c < children.length; c++) {
        var clone = children[c].cloneNode(true);
        clone.setAttribute('aria-hidden', 'true');
        clone.setAttribute('data-marquee-clone', 'true');
        frag.appendChild(clone);
      }
      track.appendChild(frag);
    }
  }
})();
