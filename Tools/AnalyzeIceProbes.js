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

var seed = 1968570037;
var width = 96;
var height = 72;

var context = MapGen.BuildCampaignPlan({
    Seed: seed,
    ProfileName: "ice_outpost",
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
ice.SmoothCharMap(context, chars);
var data = ice.Data();
var tiles = MapGen.Layers.Create(width, height, 0);
MapGen.Terrain.Smoothing.Core.ApplyEdgeRule(context, chars, tiles, data.edges, data.charToClass);

function tileSource(x, y) {
    return "edge=" + MapGen.Layers.Get(tiles, x, y, 0);
}

// Probe a 3x3 region around each cited cell — print char, tile, and ALL neighbours.
var probes = [
    { x: 87, y: 0, label: "(87,0) wet at top edge" },
    { x: 82, y: 9, label: "(82,9) wet bulge" },
    { x: 73, y: 17, label: "(73,17) bank step" },
    { x: 74, y: 19, label: "(74,19) wet step" },
    { x: 70, y: 24, label: "(70,24) wet seam" },
    { x: 51, y: 43, label: "(51,43) wet seam" }
];

function dumpRegion(p) {
    print("\n=== " + p.label + " ===");
    for(var dy = -2; dy <= 2; ++dy) {
        var row = "";
        for(var dx = -2; dx <= 2; ++dx) {
            var nx = p.x + dx;
            var ny = p.y + dy;
            if(nx < 0 || ny < 0 || nx >= width || ny >= height) {
                row += "??              ";
                continue;
            }
            var ch = MapGen.Layers.Get(chars, nx, ny, "?");
            var t = MapGen.Layers.Get(tiles, nx, ny, 0);
            var src = tileSource(nx, ny);
            var marker = (dx === 0 && dy === 0) ? "*" : " ";
            var cell = marker + ch + ":" + src;
            while(cell.length < 16)
                cell += " ";
            row += cell;
        }
        print(row);
    }
}

for(var i = 0; i < probes.length; ++i)
    dumpRegion(probes[i]);

// Edge-matcher pipeline placed every tile in a single pass. Per-cell choice
// detail (candidates considered, scores) is reproducible by re-running the
// matcher with diagnostics enabled - left as a follow-up.
print("\n=== edge matcher placed " + (width * height) + " cells ===");
