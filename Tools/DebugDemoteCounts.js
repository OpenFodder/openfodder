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

var ice = MapGen.Terrain.Smoothing.Ice;

// Snapshot initial chars (before FixCharMap)
var initialChars = ice.BuildCharMap(ctx);

// Count initial bank/wet
var initialBank = 0, initialWet = 0;
for(var x = 0; x < ctx.Width; ++x)
    for(var y = 0; y < ctx.Height; ++y) {
        var c = MapGen.Layers.Get(initialChars, x, y, "?");
        if(c === "~") ++initialBank;
        if(c === "W") ++initialWet;
    }

// Run smoothing for final state
var finalChars = ice.BuildCharMap(ctx);
ice.SmoothCharMap(ctx, finalChars);

// Count final
var finalDot = 0, finalBank = 0, finalWet = 0, finalGround = 0;
var demotedToWater = 0;  // initial=~ but final=.
var demotedFromBank = 0;  // initial=~ but final != ~
for(var fx = 0; fx < ctx.Width; ++fx)
    for(var fy = 0; fy < ctx.Height; ++fy) {
        var ic = MapGen.Layers.Get(initialChars, fx, fy, "?");
        var fc = MapGen.Layers.Get(finalChars, fx, fy, "?");
        if(fc === ".") ++finalDot;
        if(fc === "~") ++finalBank;
        if(fc === "W") ++finalWet;
        if(fc === "#") ++finalGround;
        if(ic === "~" && fc === ".") ++demotedToWater;
        if(ic === "~" && fc !== "~") ++demotedFromBank;
    }

// Count cells where char='.' but water layer = 0 (these are demoted bank/wet)
var phantomWater = 0;
for(var px = 0; px < ctx.Width; ++px)
    for(var py = 0; py < ctx.Height; ++py) {
        var fc2 = MapGen.Layers.Get(finalChars, px, py, "?");
        var wl = MapGen.Layers.Get(ctx.Layers.water, px, py, 0);
        if(fc2 === "." && !wl) ++phantomWater;
    }

// Detail by initial char
var initialWetToFinalWater = 0;
for(var rx = 0; rx < ctx.Width; ++rx)
    for(var ry = 0; ry < ctx.Height; ++ry) {
        var ic2 = MapGen.Layers.Get(initialChars, rx, ry, "?");
        var fc3 = MapGen.Layers.Get(finalChars, rx, ry, "?");
        if(ic2 === "W" && fc3 === ".") ++initialWetToFinalWater;
    }

print("seed=" + seed);
print("Initial: bank=" + initialBank + " wet=" + initialWet);
print("Final:   bank=" + finalBank + " wet=" + finalWet + " water=. cells=" + finalDot + " ground=" + finalGround);
print("Bank cells that became something else: " + demotedFromBank);
print("Bank cells demoted to water (.): " + demotedToWater);
print("Wet cells that became water (.): " + initialWetToFinalWater);
print("Phantom water (char=. but water layer=0): " + phantomWater);
print("Actual water layer cells: 829");
