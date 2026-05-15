/* global WScript, ActiveXObject */

var fso = new ActiveXObject("Scripting.FileSystemObject");
var root = fso.GetAbsolutePathName(".");

function print(pMessage) {
    WScript.Echo(String(pMessage));
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

// Patch Build to log water layer at probe cells before/after each step
var probes = [[71, 14], [72, 14], [73, 14], [70, 16], [71, 16], [72, 16], [73, 16], [73, 17], [73, 18], [74, 17]];

function snapshot(layers, label) {
    var line = label + ":";
    for(var i = 0; i < probes.length; ++i) {
        var x = probes[i][0], y = probes[i][1];
        var w = MapGen.Layers.Get(layers.water, x, y, 0) ? "1" : "0";
        line += " (" + x + "," + y + ")=" + w;
    }
    print(line);
}

var origBuild = MapGen.Layout.Rivers.Build;
MapGen.Layout.Rivers.Build = function(pContext) {
    var that = this;
    var layers = pContext.Layers;

    if(!this.ShouldBuild(pContext)) {
        this.BuildPonds(pContext);
        this.BuildStreams(pContext);
        snapshot(layers, "before-trim");
        this.TrimWaterPeninsulas(pContext);
        snapshot(layers, "after-trim ");
        this.SmoothInwardCorners(pContext);
        snapshot(layers, "after-smic");
        var retracted0 = this.EdgeFuzz(pContext);
        snapshot(layers, "after-fuzz");
        this.SmoothPinchesOnly(pContext, retracted0);
        snapshot(layers, "after-pinc");
        this.RepairBankProfile(pContext);
        snapshot(layers, "after-repr");
        return null;
    }

    var horizontal = pContext.Random.Chance(0.5);
    var start = horizontal ? this.EdgePoint(pContext, "left") : this.EdgePoint(pContext, "top");
    var end = horizontal ? this.EdgePoint(pContext, "right") : this.EdgePoint(pContext, "bottom");
    var width = this.RiverWidth(pContext);
    var river = {
        start: start,
        end: end,
        width: width,
        points: [],
        kind: "river"
    };

    this.DrawRiver(pContext, river);
    snapshot(layers, "after-draw");
    this.PlaceCrossings(pContext, river);
    pContext.Rivers.push(river);
    this.BuildPonds(pContext);
    this.BuildStreams(pContext);
    snapshot(layers, "before-trim");
    this.TrimWaterPeninsulas(pContext);
    snapshot(layers, "after-trim ");
    this.SmoothInwardCorners(pContext);
    snapshot(layers, "after-smic");
    var retracted = this.EdgeFuzz(pContext);
    snapshot(layers, "after-fuzz");
    this.SmoothPinchesOnly(pContext, retracted);
    snapshot(layers, "after-pinc");
    this.RepairBankProfile(pContext);
    snapshot(layers, "after-repr");
    return river;
};

var context = MapGen.BuildCampaignPlan({
    Seed: 1968570037,
    ProfileName: "ice_outpost",
    Overrides: {
        Width: 96,
        Height: 72,
        TerrainType: Terrain.Types.Ice,
        TerrainTypeSub: 0,
        MaxRepairPasses: 1
    },
    Attempts: 1,
    RenderInvalid: true
});

print("");
snapshot(context.Layers, "post-plan ");
print("");
print("Logs:");
if(context.Log) {
    for(var li = 0; li < context.Log.length; ++li)
        print("  " + context.Log[li]);
}
