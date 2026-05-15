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

// Use a seed that places lakes
var seed = 12345;
var ctx = MapGen.BuildCampaignPlan({
    Seed: seed, ProfileName: "ice_outpost",
    Overrides: { Width: 96, Height: 72, TerrainType: Terrain.Types.Ice, TerrainTypeSub: 0, MaxRepairPasses: 1 },
    Attempts: 1, RenderInvalid: true
});

print("seed=" + seed + " lakes=" + ctx.Lakes.length + " ponds=" + ctx.Ponds.length + " rivers=" + ctx.Rivers.length);
for(var li = 0; li < ctx.Lakes.length; ++li) {
    var lake = ctx.Lakes[li];
    print("  lake " + li + ": center=(" + lake.x + "," + lake.y + ") r=" + lake.radius + " stamps=" + lake.stamps.length);
    for(var s = 0; s < lake.stamps.length; ++s)
        print("    stamp[" + s + "]: (" + lake.stamps[s].x + "," + lake.stamps[s].y + ") r=" + lake.stamps[s].r);
}

// Re-run smoothing to inspect chars + bms
var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(ctx);
ice.SmoothCharMap(ctx, chars);

// Dump the char map cropped around each lake
function dumpRegion(cx, cy, r) {
    var pad = r + 4;
    var minX = Math.max(0, cx - pad), maxX = Math.min(ctx.Width - 1, cx + pad);
    var minY = Math.max(0, cy - pad), maxY = Math.min(ctx.Height - 1, cy + pad);
    print("Region around (" + cx + "," + cy + ") r=" + r + ":");
    for(var y = minY; y <= maxY; ++y) {
        var row = "";
        for(var x = minX; x <= maxX; ++x) {
            row += MapGen.Layers.Get(chars, x, y, "?");
        }
        print("  " + row);
    }
}

for(var li2 = 0; li2 < ctx.Lakes.length; ++li2) {
    var lake2 = ctx.Lakes[li2];
    dumpRegion(lake2.x, lake2.y, lake2.radius);
}

// Count bm distribution at every ~ cell
// bm bit positions: 0=NW 1=N 2=NE 3=W 4=E 5=SW 6=S 7=SE  (1=ground, 0=center-family)
var bmCounts = {};
function isGround(c) { return c === "#"; }
for(var x = 1; x < ctx.Width - 1; ++x) {
    for(var y = 1; y < ctx.Height - 1; ++y) {
        if(MapGen.Layers.Get(chars, x, y, "?") !== "~") continue;
        var nw = isGround(MapGen.Layers.Get(chars, x-1, y-1, "#")) ? 1 : 0;
        var n  = isGround(MapGen.Layers.Get(chars, x,   y-1, "#")) ? 1 : 0;
        var ne = isGround(MapGen.Layers.Get(chars, x+1, y-1, "#")) ? 1 : 0;
        var w  = isGround(MapGen.Layers.Get(chars, x-1, y,   "#")) ? 1 : 0;
        var e  = isGround(MapGen.Layers.Get(chars, x+1, y,   "#")) ? 1 : 0;
        var sw = isGround(MapGen.Layers.Get(chars, x-1, y+1, "#")) ? 1 : 0;
        var s  = isGround(MapGen.Layers.Get(chars, x,   y+1, "#")) ? 1 : 0;
        var se = isGround(MapGen.Layers.Get(chars, x+1, y+1, "#")) ? 1 : 0;
        var bm = "" + nw + n + ne + w + e + sw + s + se;
        if(!bmCounts[bm]) bmCounts[bm] = { count: 0, samples: [] };
        bmCounts[bm].count++;
        if(bmCounts[bm].samples.length < 3) bmCounts[bm].samples.push(x + "," + y);
    }
}
print("\nbm distribution at ~ cells (where 1=ground/#, 0=other):");
var keys = [];
for(var k in bmCounts) keys.push(k);
keys.sort(function(a,b) { return bmCounts[b].count - bmCounts[a].count; });
for(var ki = 0; ki < keys.length; ++ki) {
    var bk = keys[ki];
    print("  " + bk + " count=" + bmCounts[bk].count + " samples=" + bmCounts[bk].samples.join(" "));
}
