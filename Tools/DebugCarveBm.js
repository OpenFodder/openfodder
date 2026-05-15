/* global WScript, ActiveXObject */

var fso = new ActiveXObject("Scripting.FileSystemObject");
var root = fso.GetAbsolutePathName(".");

function print(pMessage) { WScript.Echo(String(pMessage)); }

if(!Array.prototype.indexOf) {
    Array.prototype.indexOf = function(pSearch) {
        for(var i = 0; i < this.length; ++i) if(this[i] === pSearch) return i;
        return -1;
    };
}

function readScript(pPath) {
    var path = fso.BuildPath(root, pPath);
    var file = fso.OpenTextFile(path, 1, false);
    var code = file.ReadAll();
    file.Close();
    return "\n// " + pPath + "\n" + code.replace(/\bconst\b/g, "var").replace(/\blet\b/g, "var") + "\n";
}

var scripts = [
    "Run/Scripts/Common/Terrain.js","Run/Scripts/MapGen/Random.js","Run/Scripts/MapGen/Layers.js",
    "Run/Scripts/MapGen/Context.js","Run/Scripts/MapGen/Profiles.js","Run/Scripts/MapGen/Metrics.js",
    "Run/Scripts/MapGen/Layout/Anchors.js","Run/Scripts/MapGen/Layout/Coast.js","Run/Scripts/MapGen/Layout/Rivers.js",
    "Run/Scripts/MapGen/Layout/Paths.js","Run/Scripts/MapGen/Layout/CriticalSites.js","Run/Scripts/MapGen/Layout/PathSpurs.js",
    "Run/Scripts/MapGen/Layout/Clearings.js","Run/Scripts/MapGen/Layout/Outcrops.js","Run/Scripts/MapGen/Layout/EdgeBiomes.js",
    "Run/Scripts/MapGen/Layout/DefensiveLines.js","Run/Scripts/MapGen/Layout/Regions.js",
    "Run/Scripts/MapGen/Layout/Templates/Bootstrap.js","Run/Scripts/MapGen/Layout/Templates/Classic.js",
    "Run/Scripts/MapGen/Layout/Templates/LocalisedZone.js","Run/Scripts/MapGen/Layout/Templates/Compositions.js",
    "Run/Scripts/MapGen/Layout/PathStyles.js","Run/Scripts/MapGen/Layout/Index.js",
    "Run/Scripts/MapGen/Terrain/TileCatalog.js","Run/Scripts/MapGen/Terrain/TilePalettes.js",
    "Run/Scripts/MapGen/Terrain/Water.js","Run/Scripts/MapGen/Terrain/Jungle.js","Run/Scripts/MapGen/Terrain/Transitions.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Core.js","Run/Scripts/MapGen/Terrain/Smoothing/Data/JungleData.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/IceData.js","Run/Scripts/MapGen/Terrain/Smoothing/Basic.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Desert.js","Run/Scripts/MapGen/Terrain/Smoothing/Ice.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Interior.js","Run/Scripts/MapGen/Terrain/Smoothing/Jungle.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Moors.js","Run/Scripts/MapGen/Terrain/Smoothing/Render.js",
    "Run/Scripts/MapGen/Terrain/TilePolish.js","Run/Scripts/MapGen/Terrain/Index.js",
    "Run/Scripts/MapGen/Features/Index.js","Run/Scripts/MapGen/Encounters/Index.js",
    "Run/Scripts/MapGen/Decor.js","Run/Scripts/MapGen/Decor/Jungle.js","Run/Scripts/MapGen/Decor/Ice.js",
    "Run/Scripts/MapGen/Validate.js","Run/Scripts/MapGen/Repair.js","Run/Scripts/MapGen/Retry.js",
    "Run/Scripts/MapGen/Render.js","Run/Scripts/MapGen/Generator.js","Run/Scripts/MapGen/Batch.js"
];

var combined = "";
for(var i = 0; i < scripts.length; ++i) combined += readScript(scripts[i]);
eval(combined);

var seed = 4242;
var ctx = MapGen.BuildCampaignPlan({
    Seed: seed, ProfileName: "ice_outpost",
    Overrides: { Width: 96, Height: 72, TerrainType: Terrain.Types.Ice, TerrainTypeSub: 0, MaxRepairPasses: 1 },
    Attempts: 1, RenderInvalid: true
});

// Re-run smoothing to get final char map
var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(ctx);
ice.SmoothCharMap(ctx, chars);

// Find all forcedBank cells, print their final char and 3x3 neighborhood
var fbCells = [];
for(var x = 0; x < ctx.Width; ++x) {
    for(var y = 0; y < ctx.Height; ++y) {
        if(MapGen.Layers.Get(ctx.Layers.forcedBank, x, y, 0))
            fbCells.push({ x: x, y: y });
    }
}
print("seed=" + seed + " forcedBank cells=" + fbCells.length);

// Group by proximity (each carve is 2-4 adjacent cells)
function groupKey(x, y) { return Math.floor(x / 5) + "," + Math.floor(y / 5); }
var groups = {};
for(var ci = 0; ci < fbCells.length; ++ci) {
    var k = groupKey(fbCells[ci].x, fbCells[ci].y);
    if(!groups[k]) groups[k] = [];
    groups[k].push(fbCells[ci]);
}

var shown = 0;
for(var gk in groups) {
    if(shown >= 6) break;
    var g = groups[gk];
    var minX = 999, minY = 999;
    for(var gi = 0; gi < g.length; ++gi) {
        if(g[gi].x < minX) minX = g[gi].x;
        if(g[gi].y < minY) minY = g[gi].y;
    }
    print("\n--- carve group at ~(" + minX + "," + minY + ") cells=" + g.length + " ---");
    for(var dy = -2; dy <= 4; ++dy) {
        var row = "";
        for(var dx = -2; dx <= 4; ++dx) {
            var cx = minX + dx;
            var cy = minY + dy;
            row += MapGen.Layers.Get(chars, cx, cy, "?");
        }
        print("  " + row);
    }
    // Compute bm for the test cell (top-left of pattern)
    var c = g[0];  // assume first is the test cell
    var offsets = [[-1,-1],[0,-1],[1,-1],[-1,0],[1,0],[-1,1],[0,1],[1,1]];
    var bm = "";
    for(var oi = 0; oi < offsets.length; ++oi) {
        var nch = MapGen.Layers.Get(chars, c.x + offsets[oi][0], c.y + offsets[oi][1], "#");
        var isGround = (nch === "W" || nch === "#" || nch === "+" || nch === "T") ? "1" : "0";
        bm += isGround;
    }
    print("  test=(" + c.x + "," + c.y + ") char=" + MapGen.Layers.Get(chars, c.x, c.y, "?") + " bm=" + bm);
    ++shown;
}
