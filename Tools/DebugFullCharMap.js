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

print("rivers=" + ctx.Rivers.length + " (sizes:");
for(var ri = 0; ri < ctx.Rivers.length; ++ri) {
    var r = ctx.Rivers[ri];
    print("  river " + ri + ": kind=" + r.kind + " width=" + r.width + " points=" + r.points.length);
}

var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(ctx);
ice.SmoothCharMap(ctx, chars);

// Print column headers
var hdr = "    ";
for(var x = 0; x < ctx.Width; ++x) hdr += (x % 10 === 0) ? String(Math.floor(x / 10)) : " ";
print(hdr);
hdr = "    ";
for(var x = 0; x < ctx.Width; ++x) hdr += String(x % 10);
print(hdr);

for(var y = 0; y < ctx.Height; ++y) {
    var row = "";
    for(var x = 0; x < ctx.Width; ++x)
        row += MapGen.Layers.Get(chars, x, y, "?");
    var label = (y < 10 ? " " : "") + (y < 100 ? " " : "") + y + " ";
    print(label + row);
}

// Compare char map vs water layer to find demoted-bank cells (chars `.` but water layer = 0)
print("\nDemoted-bank cells (char='.', water layer = 0) — these were carved out of land:");
var demoted = 0;
for(var dy = 0; dy < ctx.Height; ++dy) {
    for(var dx = 0; dx < ctx.Width; ++dx) {
        var ch = MapGen.Layers.Get(chars, dx, dy, "?");
        var wt = MapGen.Layers.Get(ctx.Layers.water, dx, dy, 0);
        if(ch === "." && !wt) ++demoted;
    }
}
var actualWater = 0;
for(var ay = 0; ay < ctx.Height; ++ay)
    for(var ax = 0; ax < ctx.Width; ++ax)
        if(MapGen.Layers.Get(ctx.Layers.water, ax, ay, 0)) ++actualWater;
print("  total `.` cells with water=0: " + demoted);
print("  total water layer cells: " + actualWater);
print("  total `.` rendered: " + (demoted + actualWater) + " (" + Math.round((demoted + actualWater) * 100 / (ctx.Width * ctx.Height)) + "% of map)");

// Specific cells the user mentioned
print("\nUser-mentioned cells:");
for(var ux = 11; ux <= 18; ++ux) {
    var c = MapGen.Layers.Get(chars, ux, 15, "?");
    var cw = MapGen.Layers.Get(ctx.Layers.water, ux, 15, 0);
    print("  (" + ux + ",15) char=" + c + " water=" + cw);
}
for(var ux = 14; ux <= 22; ++ux) {
    var c = MapGen.Layers.Get(chars, ux, 38, "?");
    var cw = MapGen.Layers.Get(ctx.Layers.water, ux, 38, 0);
    print("  (" + ux + ",38) char=" + c + " water=" + cw);
}
