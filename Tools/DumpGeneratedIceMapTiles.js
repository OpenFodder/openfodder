/* global WScript, ActiveXObject */

var fso = new ActiveXObject("Scripting.FileSystemObject");
var root = fso.GetAbsolutePathName(".");

function print(pMessage) {
    WScript.Echo(String(pMessage));
}

if(!Array.prototype.indexOf) {
    Array.prototype.indexOf = function(pSearch) {
        for(var index = 0; index < this.length; ++index) {
            if(this[index] === pSearch)
                return index;
        }

        return -1;
    };
}

function argumentValue(pName, pDefault) {
    var prefix = "--" + pName + "=";

    for(var index = 0; index < WScript.Arguments.Length; ++index) {
        var value = String(WScript.Arguments.Item(index));
        if(value.indexOf(prefix) === 0)
            return value.substring(prefix.length);
    }

    return pDefault;
}

function splitList(pValue) {
    var result = [];
    var parts = String(pValue || "").split(",");

    for(var index = 0; index < parts.length; ++index) {
        var item = parts[index].replace(/^\s+|\s+$/g, "");
        if(item)
            result.push(item);
    }

    return result;
}

function readScript(pPath) {
    var path = fso.BuildPath(root, pPath);
    var file = fso.OpenTextFile(path, 1, false);
    var code = file.ReadAll();
    file.Close();
    code = code.replace(/\bconst\b/g, "var").replace(/\blet\b/g, "var");
    return "\n// " + pPath + "\n" + code + "\n";
}

var scripts = [
    "Run/Scripts/Common/Terrain.js",
    "Run/Scripts/MapGen/Random.js",
    "Run/Scripts/MapGen/Layers.js",
    "Run/Scripts/MapGen/Context.js",
    "Run/Scripts/MapGen/Profiles.js",
    "Run/Scripts/MapGen/Metrics.js",
    "Run/Scripts/MapGen/Layout/Anchors.js",
    "Run/Scripts/MapGen/Layout/Coast.js",
    "Run/Scripts/MapGen/Layout/Rivers.js",
    "Run/Scripts/MapGen/Layout/Paths.js",
    "Run/Scripts/MapGen/Layout/CriticalSites.js",
    "Run/Scripts/MapGen/Layout/PathSpurs.js",
    "Run/Scripts/MapGen/Layout/Clearings.js",
    "Run/Scripts/MapGen/Layout/Outcrops.js",
    "Run/Scripts/MapGen/Layout/EdgeBiomes.js",
    "Run/Scripts/MapGen/Layout/DefensiveLines.js",
    "Run/Scripts/MapGen/Layout/Regions.js",
    "Run/Scripts/MapGen/Layout/Templates/Bootstrap.js",
    "Run/Scripts/MapGen/Layout/Templates/Classic.js",
    "Run/Scripts/MapGen/Layout/Templates/LocalisedZone.js",
    "Run/Scripts/MapGen/Layout/Templates/Compositions.js",
    "Run/Scripts/MapGen/Layout/PathStyles.js",
    "Run/Scripts/MapGen/Layout/Index.js",
    "Run/Scripts/MapGen/Terrain/TileCatalog.js",
    "Run/Scripts/MapGen/Terrain/TilePalettes.js",
    "Run/Scripts/MapGen/Terrain/Water.js",
    "Run/Scripts/MapGen/Terrain/Jungle.js",
    "Run/Scripts/MapGen/Terrain/Transitions.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Core.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/JungleData.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/IceData.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/IceTileEdges.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Basic.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Desert.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Ice.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Interior.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Jungle.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Moors.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Render.js",
    "Run/Scripts/MapGen/Terrain/TilePolish.js",
    "Run/Scripts/MapGen/Terrain/Index.js",
    "Run/Scripts/MapGen/Features/Index.js",
    "Run/Scripts/MapGen/Encounters/Index.js",
    "Run/Scripts/MapGen/Decor.js",
    "Run/Scripts/MapGen/Decor/Jungle.js",
    "Run/Scripts/MapGen/Decor/Ice.js",
    "Run/Scripts/MapGen/Validate.js",
    "Run/Scripts/MapGen/Repair.js",
    "Run/Scripts/MapGen/Retry.js",
    "Run/Scripts/MapGen/Render.js",
    "Run/Scripts/MapGen/Generator.js",
    "Run/Scripts/MapGen/Batch.js"
];

var combined = "";
for(var scriptIndex = 0; scriptIndex < scripts.length; ++scriptIndex)
    combined += readScript(scripts[scriptIndex]);

eval(combined);

var seeds = splitList(argumentValue("seeds", "101"));
var width = Number(argumentValue("width", "64"));
var height = Number(argumentValue("height", "48"));
var profileName = argumentValue("profile", "ice_outpost");

for(var seedIndex = 0; seedIndex < seeds.length; ++seedIndex) {
    var seed = Number(seeds[seedIndex]);
    var context = MapGen.BuildCampaignPlan({
        Seed: seed,
        ProfileName: profileName,
        Overrides: {
            Width: width,
            Height: height,
            TerrainType: Terrain.Types.Ice,
            TerrainTypeSub: 0,
            MaxRepairPasses: 1
        },
        Attempts: 1,
        RenderInvalid: true
    });
    var rendered = context.RenderedMap;

    if(!rendered || !rendered.Tiles) {
        print("error seed=" + seed + " message=no_rendered_tiles");
        continue;
    }

    print("begin generated_" + seed + " " + width + " " + height);
    for(var y = 0; y < height; ++y) {
        var row = [];

        for(var x = 0; x < width; ++x)
            row.push(Number(MapGen.Layers.Get(rendered.Tiles, x, y, 0)));

        print(row.join(","));
    }
    print("end generated_" + seed);

    // Dump charmap for ice terrain so the regen script can verify bank shape.
    if(context.Profile && context.Profile.TerrainType === Terrain.Types.Ice &&
        MapGen.Terrain && MapGen.Terrain.Smoothing && MapGen.Terrain.Smoothing.Ice) {
        var iceMod = MapGen.Terrain.Smoothing.Ice;
        var coreMod = MapGen.Terrain.Smoothing.Core;
        var chars = iceMod.BuildCharMap(context);

        function dumpStage(pStageName) {
            var sp = fso.BuildPath(root, "Run/icecharmap_" + pStageName + "_" + seed + ".txt");
            var sf = fso.OpenTextFile(sp, 2, true);
            sf.WriteLine("# Ice char map (" + pStageName + ") for seed " + seed);
            sf.WriteLine("# Size: " + width + "x" + height);
            for(var dy = 0; dy < height; ++dy) {
                var dr = "";
                for(var dx = 0; dx < width; ++dx)
                    dr += MapGen.Layers.Get(chars, dx, dy, "#");
                sf.WriteLine(dr);
            }
            sf.Close();
        }

        dumpStage("pre");
        iceMod.FixCharMap(context, chars);
        dumpStage("after_fix1");
        // Wobble is disabled in production (Ice.js SmoothCharMap). Mirror that
        // here so the diagnostic dump reflects the rendered state instead of
        // an alternate path nothing else takes.
        dumpStage("after_wobble");
        coreMod.SmoothCharMap(
            context, chars,
            [
                { center: iceMod.Chars.wet, ground: iceMod.Chars.ground, next: iceMod.Chars.path },
                { center: iceMod.Chars.path, ground: iceMod.Chars.ground, next: iceMod.Chars.tree },
                { center: iceMod.Chars.tree, ground: iceMod.Chars.ground }
            ],
            function(px, py) { return iceMod.IsProtectedChar(context, px, py); },
            2
        );
        dumpStage("after_smooth");
        for(var fp = 0; fp < 16; ++fp) {
            var pc = iceMod.FixCharMap(context, chars);
            if(pc === 0) break;
        }
        dumpStage("after_fixloop");
        var charPath = fso.BuildPath(root, "Run/icecharmap_" + seed + ".txt");
        var charFile = fso.OpenTextFile(charPath, 2, true);
        charFile.WriteLine("# Ice char map for seed " + seed);
        charFile.WriteLine("# Chars: . water  # ground  + path  T tree  ~ bank  W wet");
        charFile.WriteLine("# Size: " + width + "x" + height);
        for(var cy = 0; cy < height; ++cy) {
            var charRow = "";
            for(var cx = 0; cx < width; ++cx)
                charRow += MapGen.Layers.Get(chars, cx, cy, "#");
            charFile.WriteLine(charRow);
        }
        charFile.Close();
    }
}
