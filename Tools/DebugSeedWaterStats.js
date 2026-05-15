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

var seed = 1968570037;
var ctx = MapGen.BuildCampaignPlan({
    Seed: seed, ProfileName: "ice_outpost",
    Overrides: { Width: 96, Height: 72, TerrainType: Terrain.Types.Ice, TerrainTypeSub: 0, MaxRepairPasses: 1 },
    Attempts: 1, RenderInvalid: true
});

var totalCells = ctx.Width * ctx.Height;
var waterCells = 0;
for(var x = 0; x < ctx.Width; ++x)
    for(var y = 0; y < ctx.Height; ++y)
        if(MapGen.Layers.Get(ctx.Layers.water, x, y, 0)) ++waterCells;

print("seed=" + seed + " size=" + ctx.Width + "x" + ctx.Height + " totalCells=" + totalCells);
print("rivers=" + ctx.Rivers.length + " lakes=" + ctx.Lakes.length + " ponds=" + ctx.Ponds.length);
print("waterCells=" + waterCells + " (" + Math.round(waterCells * 100 / totalCells) + "%)");

if(ctx.Rivers.length) {
    for(var ri = 0; ri < ctx.Rivers.length; ++ri) {
        var r = ctx.Rivers[ri];
        print("  river " + ri + ": width=" + r.width + " points=" + (r.points ? r.points.length : 0) +
              " start=(" + r.start.x + "," + r.start.y + ") end=(" + r.end.x + "," + r.end.y + ")");
    }
}
if(ctx.Lakes.length) {
    for(var li = 0; li < ctx.Lakes.length; ++li) {
        var lk = ctx.Lakes[li];
        print("  lake " + li + ": center=(" + lk.x + "," + lk.y + ") r=" + lk.radius);
    }
}

// Render one row out of every 4 as a quick ascii view
var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(ctx);
ice.SmoothCharMap(ctx, chars);
print("\nchar map (every 2nd row, every 2nd col):");
for(var y2 = 0; y2 < ctx.Height; y2 += 2) {
    var row = "";
    for(var x2 = 0; x2 < ctx.Width; x2 += 2)
        row += MapGen.Layers.Get(chars, x2, y2, "?");
    print(row);
}
