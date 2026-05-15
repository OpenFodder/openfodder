/* global WScript, ActiveXObject */

// Probe ApplyEdgeRule for specific cells: print char, class, candidate count,
// scored candidates, selected tile, AND what the rendered tile actually is.
// Lets us tell whether ApplyEdgeRule itself produced a wrong-class tile, or
// whether some later pass overwrote it.

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

var seed = Number(argumentValue("seed", "1968570037"));
var width = Number(argumentValue("width", "96"));
var height = Number(argumentValue("height", "72"));
var profileName = argumentValue("profile", "ice_outpost");

var probes = [
    { x: 34, y: 66 },
    { x: 35, y: 66 },
    { x: 33, y: 66 },
    { x: 36, y: 66 },
    { x: 34, y: 65 },
    { x: 35, y: 65 },
    { x: 34, y: 67 },
    { x: 35, y: 67 }
];

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
    print("error: no rendered tiles");
    WScript.Quit(1);
}

var iceMod = MapGen.Terrain.Smoothing.Ice;
var coreMod = MapGen.Terrain.Smoothing.Core;
var data = iceMod.Data();
if(!data) {
    print("error: no ice edge data");
    WScript.Quit(1);
}

// Replicate the runtime char map computation.
var chars = iceMod.BuildCharMap(context);
iceMod.FixCharMap(context, chars);
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
for(var fp = 0; fp < 16; ++fp) {
    var pc = iceMod.FixCharMap(context, chars);
    if(pc === 0) break;
}

// Re-run the matcher onto a fresh tiles layer.
var probeTiles = MapGen.Layers.Create(context.Width, context.Height, 0);
coreMod.ApplyEdgeRule(context, chars, probeTiles, data.edges, data.charToClass);

print("seed=" + seed + " profile=" + profileName);
for(var pi = 0; pi < probes.length; ++pi) {
    var p = probes[pi];
    var ch = MapGen.Layers.Get(chars, p.x, p.y, "?");
    var cls = data.charToClass[ch] || "?";
    var candidates = (data.edges.byCenter && data.edges.byCenter[cls]) || [];
    var probeTile = MapGen.Layers.Get(probeTiles, p.x, p.y, -1);
    var renderTile = MapGen.Layers.Get(rendered.Tiles, p.x, p.y, -1);
    var inBucket = candidates.indexOf(probeTile) >= 0;

    print("---");
    print("cell=(" + p.x + "," + p.y + ") char=" + ch + " class=" + cls);
    print("  bucket size=" + candidates.length + " bucket=[" + candidates.join(",") + "]");
    print("  matcher_probe_tile=" + probeTile + " in_bucket=" + inBucket);
    print("  rendered_tile=" + renderTile);
    if(probeTile !== renderTile)
        print("  *** DIFFERS *** rendered changed by post-matcher pass");

    // Show neighbour chars
    var n = MapGen.Layers.Get(chars, p.x, p.y - 1, "?");
    var s = MapGen.Layers.Get(chars, p.x, p.y + 1, "?");
    var w = MapGen.Layers.Get(chars, p.x - 1, p.y, "?");
    var e = MapGen.Layers.Get(chars, p.x + 1, p.y, "?");
    print("  N=" + n + " S=" + s + " W=" + w + " E=" + e);
}
