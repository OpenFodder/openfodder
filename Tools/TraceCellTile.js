/* global WScript, ActiveXObject */
var fso = new ActiveXObject("Scripting.FileSystemObject");
var root = fso.GetAbsolutePathName(".");

function print(pMessage) { WScript.Echo(String(pMessage)); }

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

var probes = [[61, 32], [62, 32], [60, 32], [61, 33], [62, 33], [61, 31], [62, 31]];

function snap(layers, label) {
    var line = label + ":";
    for(var i = 0; i < probes.length; ++i) {
        var x = probes[i][0], y = probes[i][1];
        var t = MapGen.Layers.Get(layers, x, y, -1);
        line += " (" + x + "," + y + ")=" + t;
    }
    print(line);
}

function snapChars(chars, label) {
    var line = label + ":";
    for(var i = 0; i < probes.length; ++i) {
        var x = probes[i][0], y = probes[i][1];
        var c = MapGen.Layers.Get(chars, x, y, "?");
        line += " (" + x + "," + y + ")='" + c + "'";
    }
    print(line);
}

var origSmoothingRender = MapGen.Terrain.Smoothing.Render;
MapGen.Terrain.Smoothing.Render = function(pContext) {
    print("HOOK: Smoothing.Render called, terrainType=" + (pContext && pContext.Profile && pContext.Profile.TerrainType));
    return origSmoothingRender(pContext);
};

var ice = MapGen.Terrain.Smoothing.Ice;
var origRender = ice.Render;
ice.Render = function(pContext) {
    print("HOOK: ice.Render called");
    if(pContext.Profile.TerrainType !== Terrain.Types.Ice)
        return origRender.call(this, pContext);

    try {
    var data = this.Data();
    if(!data) { print("HOOK: data null"); return null; }

    var chars = this.BuildCharMap(pContext);
    snapChars(chars, "after-build  ");
    var charSmoothing = this.SmoothCharMap(pContext, chars);
    snapChars(chars, "after-smooth ");
    this.DumpCharMap(pContext, chars);
    var tiles = MapGen.Layers.Create(pContext.Width, pContext.Height, 0);
    snap(tiles, "after-create ");

    MapGen.Terrain.Smoothing.Core.ApplyEdgeRule(pContext, chars, tiles, data.edges, data.charToClass);
    snap(tiles, "after-edges  ");

    var self = this;
    MapGen.Terrain.Smoothing.Core.ApplyTreeRules(pContext, chars, tiles, data.tree,
        function(x, y) { return self.IsTreeTileProtected(pContext, x, y); });
    snap(tiles, "after-trees  ");

    if(MapGen.Terrain.TilePolish) {
        MapGen.Terrain.TilePolish.Apply(pContext, chars, tiles);
        snap(tiles, "after-polish ");
    }
    pContext.RenderedMap = tiles;
    return {
        Tiles: tiles,
        Backend: "ice_edges",
        TransitionTiles: 0,
        TerrainTransitionTiles: 0,
        TreeTiles: 0,
        ProtectedTiles: 0,
        PolishTiles: 0,
        CharFixes: 0,
        CharSmoothChanges: 0
    };
    }
    catch(e) {
        print("HOOK ERR: " + e.message + " at " + (e.number || "?"));
        throw e;
    }
};

print("BuildCampaignPlan type=" + typeof MapGen.BuildCampaignPlan);
print("Layout.BuildCampaign=" + typeof (MapGen.Layout && MapGen.Layout.BuildCampaign));
print("Terrain.Build=" + typeof (MapGen.Terrain && MapGen.Terrain.Build));
print("Features.BuildCampaign=" + typeof (MapGen.Features && MapGen.Features.BuildCampaign));
print("Encounters.BuildCampaign=" + typeof (MapGen.Encounters && MapGen.Encounters.BuildCampaign));
print("Render.BuildTileLayer=" + typeof (MapGen.Render && MapGen.Render.BuildTileLayer));
print("ValidateAndRepair=" + typeof MapGen.ValidateAndRepair);
print("Context.Create=" + typeof (MapGen.Context && MapGen.Context.Create));
try {
    var ctxOpts = { Seed: 1968570037, ProfileName: "ice_outpost",
        Overrides: { Width: 96, Height: 72, TerrainType: Terrain.Types.Ice, TerrainTypeSub: 0, MaxRepairPasses: 1 },
        Attempts: 1, RenderInvalid: true };
    print("step: Context.Create");
    var context = MapGen.Context.Create(ctxOpts);
    print("step: Layout.BuildCampaign");
    MapGen.Layout.BuildCampaign(context);
    print("step: Terrain.Build");
    MapGen.Terrain.Build(context);
    print("step: Features.BuildCampaign");
    MapGen.Features.BuildCampaign(context);
    print("step: Encounters.BuildCampaign");
    MapGen.Encounters.BuildCampaign(context);
    print("step: Decor.Build");
    if(MapGen.Decor) MapGen.Decor.Build(context);
    print("step: Render.BuildTileLayer");
    MapGen.Render.BuildTileLayer(context);
    print("step: ValidateAndRepair");
    MapGen.ValidateAndRepair(context, { RequireReservedWalkable: true, MinimumLargestWalkableComponent: 0.25 });
    print("step: done");
} catch(e) {
    print("OUTER ERR: " + e.message);
}

print("");
print("done");
