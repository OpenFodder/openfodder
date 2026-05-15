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

var context = MapGen.BuildCampaignPlan({
    Seed: 1968570037, ProfileName: "ice_outpost",
    Overrides: { Width: 96, Height: 72, TerrainType: Terrain.Types.Ice, TerrainTypeSub: 0, MaxRepairPasses: 1 },
    Attempts: 1, RenderInvalid: true
});

var forcedCount = 0;
var forcedCells = [];
for(var x = 0; x < context.Width; ++x) {
    for(var y = 0; y < context.Height; ++y) {
        if(MapGen.Layers.Get(context.Layers.forcedBank, x, y, 0)) {
            ++forcedCount;
            if(forcedCells.length < 30) forcedCells.push(x + "," + y);
        }
    }
}

print("forcedBank cells: " + forcedCount);
print("first cells: " + forcedCells.join(" "));

var ice = MapGen.Terrain.Smoothing.Ice;
var chars = ice.BuildCharMap(context);
ice.SmoothCharMap(context, chars);
var survivors = 0;
print("char at each forcedBank cell AFTER SmoothCharMap:");
for(var fi = 0; fi < forcedCells.length; ++fi) {
    var parts = forcedCells[fi].split(",");
    var fx = Number(parts[0]); var fy = Number(parts[1]);
    var ch = MapGen.Layers.Get(chars, fx, fy, "?");
    if(ch === ice.Chars.bank) ++survivors;
    print("  (" + fx + "," + fy + ") final char='" + ch + "'" +
          " left=" + MapGen.Layers.Get(chars, fx-1, fy, "?") +
          " right=" + MapGen.Layers.Get(chars, fx+1, fy, "?"));
}
print("survivors as ~: " + survivors + " / " + forcedCells.length);

if(context.Log) {
    for(var li = 0; li < context.Log.length; ++li) {
        var entry = context.Log[li];
        if(typeof entry === "string" && entry.indexOf("step bank") >= 0) print("LOG: " + entry);
        else if(entry && entry.message && entry.message.indexOf && entry.message.indexOf("step bank") >= 0) print("LOG: " + entry.message);
    }
}
