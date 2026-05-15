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

function hasArgument(pName) {
    var value = "--" + pName;

    for(var index = 0; index < WScript.Arguments.Length; ++index) {
        if(String(WScript.Arguments.Item(index)) === value)
            return true;
    }

    return false;
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

var seed = Number(argumentValue("seed", "1968570037"));
var width = Number(argumentValue("width", "96"));
var height = Number(argumentValue("height", "72"));
var profile = argumentValue("profile", "ice_outpost");
var noHeader = hasArgument("no-header");

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

var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(context);
var smoothing = ice.SmoothCharMap(context, chars);

if(!noHeader) {
    print("seed=" + seed + " profile=" + profile + " size=" + width + "x" + height +
        " charFixes=" + smoothing.fixed + " charSmooth=" + smoothing.smoothed);
    print("legend #=snow .=water ~=bank W=wet +=path/ford/reserved T=blocked-cover");

    for(var riverIndex = 0; riverIndex < context.Rivers.length; ++riverIndex) {
        var river = context.Rivers[riverIndex];
        print("river[" + riverIndex + "] kind=" + river.kind + " width=" + river.width +
            " start=" + river.start.x + "," + river.start.y +
            " end=" + river.end.x + "," + river.end.y +
            " points=" + river.points.length);
        for(var ptIdx = 0; ptIdx < river.points.length; ++ptIdx)
            print("  spine[" + ptIdx + "]=" + river.points[ptIdx].x + "," + river.points[ptIdx].y);
    }
}

for(var y = 0; y < height; ++y) {
    var row = "";

    for(var x = 0; x < width; ++x)
        row += MapGen.Layers.Get(chars, x, y, ice.Chars.ground);

    print(row);
}
