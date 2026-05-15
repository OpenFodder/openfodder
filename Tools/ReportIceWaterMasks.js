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
    "Run/Scripts/MapGen/Layout/Regions.js",
    "Run/Scripts/MapGen/Layout/Templates/Bootstrap.js",
    "Run/Scripts/MapGen/Layout/Templates/Classic.js",
    "Run/Scripts/MapGen/Layout/Templates/LocalisedZone.js",
    "Run/Scripts/MapGen/Layout/Templates/Compositions.js",
    "Run/Scripts/MapGen/Layout/PathStyles.js",
    "Run/Scripts/MapGen/Layout/Index.js",
    "Run/Scripts/MapGen/Layout/DefensiveLines.js",
    "Run/Scripts/MapGen/Terrain/TileCatalog.js",
    "Run/Scripts/MapGen/Terrain/TilePalettes.js",
    "Run/Scripts/MapGen/Terrain/Water.js",
    "Run/Scripts/MapGen/Terrain/Jungle.js",
    "Run/Scripts/MapGen/Terrain/Transitions.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Core.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/JungleData.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/IceData.js",
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

function tableSet(pData) {
    var set = {};

    for(var index = 0; index < pData.bitmask.length; ++index)
        set[pData.bitmask[index].bm] = true;

    return set;
}

function addCount(pCounts, pKey) {
    pCounts[pKey] = (pCounts[pKey] || 0) + 1;
}

function sortedKeys(pCounts) {
    var keys = [];

    for(var key in pCounts) {
        if(pCounts.hasOwnProperty(key))
            keys.push(key);
    }

    keys.sort(function(pA, pB) {
        return pCounts[pB] - pCounts[pA];
    });

    return keys;
}

function printCounts(pTitle, pCounts, pLimit) {
    var keys = sortedKeys(pCounts);
    var parts = [];
    var limit = Math.min(keys.length, pLimit || 20);

    for(var index = 0; index < limit; ++index)
        parts.push(keys[index] + "=" + pCounts[keys[index]]);

    print(pTitle + " unique=" + keys.length + " " + parts.join(" "));
}

function reportRule(pContext, pChars, pRuleName, pRule, pKnown) {
    var core = MapGen.Terrain.Smoothing.Core;
    var counts = {};
    var missing = {};

    for(var x = 0; x < pContext.Width; ++x) {
        for(var y = 0; y < pContext.Height; ++y) {
            if(core.GetChar(pChars, x, y, "") !== pRule.center)
                continue;

            var bitmask = core.TransitionBitmask(pChars, x, y, pRule);
            if(bitmask === null)
                continue;

            addCount(counts, bitmask);
            if(!pKnown[bitmask])
                addCount(missing, bitmask);
        }
    }

    printCounts(pRuleName + " raw", counts, 16);
    printCounts(pRuleName + " raw_missing", missing, 16);
}

var seed = Number(argumentValue("seed", "101"));
var width = Number(argumentValue("width", "96"));
var height = Number(argumentValue("height", "72"));
var profile = argumentValue("profile", "ice_outpost");

var context = MapGen.BuildCampaignPlan({
    Seed: seed,
    ProfileName: profile,
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

for(var riverIndex = 0; riverIndex < context.Rivers.length; ++riverIndex) {
    var river = context.Rivers[riverIndex];
    print("river[" + riverIndex + "] kind=" + river.kind + " width=" + river.width +
        " start=" + river.start.x + "," + river.start.y +
        " end=" + river.end.x + "," + river.end.y +
        " points=" + river.points.length);
}

var ice = MapGen.Terrain.Smoothing.Ice;
var data = ice.BitmaskData ? ice.BitmaskData(context) : ice.Data(context);
var chars = ice.BuildCharMap(context);
var smoothing = ice.SmoothCharMap(context, chars);

print("seed=" + seed + " profile=" + profile + " charFixes=" + smoothing.fixed + " charSmooth=" + smoothing.smoothed);

reportRule(context, chars, "deepWater", {
    center: ice.Chars.water,
    ground: ice.Chars.bank,
    extras: [ice.Chars.wet, ice.Chars.ground, ice.Chars.path]
}, tableSet(data.deepWater));

reportRule(context, chars, "shallowWater", {
    center: ice.Chars.bank,
    ground: ice.Chars.wet,
    centerAliases: [ice.Chars.water],
    extras: [ice.Chars.ground, ice.Chars.path]
}, tableSet(data.shallowWater));

reportRule(context, chars, "wetIce", {
    center: ice.Chars.wet,
    ground: ice.Chars.ground,
    centerAliases: [ice.Chars.water, ice.Chars.bank],
    extras: [ice.Chars.path]
}, tableSet(data.wetIce));
