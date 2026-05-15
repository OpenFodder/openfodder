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

var terrainNames = {};
terrainNames[Terrain.Types.Jungle] = "jungle";
terrainNames[Terrain.Types.Desert] = "desert";
terrainNames[Terrain.Types.Ice] = "ice";
terrainNames[Terrain.Types.Moors] = "moors";
terrainNames[Terrain.Types.Interior] = "interior";
terrainNames[Terrain.Types.AmigaFormat] = "amigaFormat";

var failures = 0;
var seeds = [101, 202, 303, 404, 505];
var terrains = [
    Terrain.Types.Jungle,
    Terrain.Types.Desert,
    Terrain.Types.Ice,
    Terrain.Types.Moors,
    Terrain.Types.Interior,
    Terrain.Types.AmigaFormat
];

function buildCampaignPlanForSmoke(pOptions) {
    return MapGen.BuildCampaignPlan(pOptions);
}

function countRenderedTiles(pContext, pAllowedTiles) {
    var lookup = {};
    var count = 0;

    for(var tileIndex = 0; tileIndex < pAllowedTiles.length; ++tileIndex)
        lookup[pAllowedTiles[tileIndex]] = true;

    if(!pContext || !pContext.RenderedMap || !pContext.RenderedMap.Tiles)
        return 0;

    for(var x = 0; x < pContext.RenderedMap.Tiles.length; ++x) {
        for(var y = 0; y < pContext.RenderedMap.Tiles[x].length; ++y) {
            if(lookup[pContext.RenderedMap.Tiles[x][y]])
                ++count;
        }
    }

    return count;
}

function countMisplacedIceRootTiles(pContext) {
    var rootTiles = {
        210: true,
        211: true,
        212: true,
        233: true,
        234: true
    };
    var treeTiles = {};
    var count = 0;
    var family = MapGen.Terrain.Smoothing.IceData.tileFamilies.iceTree;

    if(!pContext || !pContext.RenderedMap || !pContext.RenderedMap.Tiles || !family)
        return 0;

    for(var tileIndex = 0; tileIndex < family.all.length; ++tileIndex)
        treeTiles[Number(family.all[tileIndex])] = true;

    for(var x = 0; x < pContext.RenderedMap.Tiles.length; ++x) {
        for(var y = 0; y < pContext.RenderedMap.Tiles[x].length; ++y) {
            if(!rootTiles[pContext.RenderedMap.Tiles[x][y]])
                continue;
            if(treeTiles[MapGen.Layers.Get(pContext.RenderedMap.Tiles, x, y + 1, -1)])
                ++count;
        }
    }

    return count;
}

function countRiverBankNearRivers(pContext) {
    var count = 0;

    if(!pContext || !pContext.Layers || !pContext.Layers.riverBank)
        return 0;

    for(var x = 0; x < pContext.Width; ++x) {
        for(var y = 0; y < pContext.Height; ++y) {
            if(MapGen.Layers.Get(pContext.Layers.riverBank, x, y, 0) &&
                MapGen.Terrain.Water.NearRiver(pContext, x, y, 2))
                ++count;
        }
    }

    return count;
}

function countFordRenderedWaterTiles(pContext, pWaterTiles) {
    var lookup = {};
    var count = 0;

    if(!pContext || !pContext.Layers || !pContext.RenderedMap || !pContext.RenderedMap.Tiles)
        return 0;

    for(var tileIndex = 0; tileIndex < pWaterTiles.length; ++tileIndex)
        lookup[pWaterTiles[tileIndex]] = true;

    for(var x = 0; x < pContext.Width; ++x) {
        for(var y = 0; y < pContext.Height; ++y) {
            if(MapGen.Layers.Get(pContext.Layers.ford, x, y, 0) &&
                lookup[MapGen.Layers.Get(pContext.RenderedMap.Tiles, x, y, -1)])
                ++count;
        }
    }

    return count;
}

function renderedTileHash(pContext) {
    var hash = 2166136261;

    if(!pContext || !pContext.RenderedMap || !pContext.RenderedMap.Tiles)
        return 0;

    for(var y = 0; y < pContext.Height; ++y) {
        for(var x = 0; x < pContext.Width; ++x) {
            hash ^= pContext.RenderedMap.Tiles[x][y] & 0xFFFF;
            hash = ((hash * 16777619) % 4294967296) >>> 0;
        }
    }

    return hash >>> 0;
}

function verifyRetrySeedSpread() {
    var spreadSeeds = [7789, 73325, 138861, 204397];
    var hashes = {};
    var uniqueHashes = 0;

    for(var index = 0; index < spreadSeeds.length; ++index) {
        var context = MapGen.BuildBestCampaignPlan({
            Seed: spreadSeeds[index],
            ProfileName: "dense_jungle_trails",
            Overrides: {
                Width: 64,
                Height: 48,
                TerrainType: Terrain.Types.Jungle,
                TerrainTypeSub: 0,
                MaxRepairPasses: 1
            },
            Attempts: 1,
            RenderInvalid: true
        });
        var hash = renderedTileHash(context);

        if(!hashes[hash]) {
            hashes[hash] = true;
            ++uniqueHashes;
        }

        print("seed_spread seed=" + spreadSeeds[index] + " ctxSeed=" + context.Seed + " hash=" + hash);
    }

    if(uniqueHashes !== spreadSeeds.length)
        ++failures;
}

verifyRetrySeedSpread();

for(var terrainIndex = 0; terrainIndex < terrains.length; ++terrainIndex) {
    var terrainType = terrains[terrainIndex];

    for(var seedIndex = 0; seedIndex < seeds.length; ++seedIndex) {
        var context = null;
        try {
            context = buildCampaignPlanForSmoke({
                Seed: seeds[seedIndex],
                ProfileName: "classic_jungle",
                Overrides: {
                    Width: 64,
                    Height: 48,
                    TerrainType: terrainType,
                    TerrainTypeSub: 0,
                    MaxRepairPasses: 1
                },
                Attempts: 1,
                RenderInvalid: true
            });
        }
        catch(pError) {
            print(terrainNames[terrainType] + " seed=" + seeds[seedIndex] + " error=" + pError.message);
            ++failures;
            continue;
        }
        var validation = context.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
        var backend = context.RenderedMap ? context.RenderedMap.Backend : "none";
        var transitionTiles = context.RenderedMap && context.RenderedMap.Smoothing ? context.RenderedMap.Smoothing.transitionTiles || 0 : 0;

        print(
            terrainNames[terrainType] +
            " seed=" + seeds[seedIndex] +
            " ok=" + validation.ok +
            " fatal=" + validation.fatal +
            " backend=" + backend +
            " transitionTiles=" + transitionTiles +
            " reasons=" + (validation.reasons || []).join("|")
        );

        if(!validation.ok || validation.fatal)
            ++failures;

        if(terrainType === Terrain.Types.Desert && backend !== "desert_adapter")
            ++failures;
        if(terrainType === Terrain.Types.Moors && (backend !== "moors_adapter" || transitionTiles < 1))
            ++failures;
        if(terrainType === Terrain.Types.Interior && (backend !== "interior_adapter" || transitionTiles < 1))
            ++failures;
    }
}

var profilePresets = [
    { name: "classic_jungle", backend: "jungle_bitmask" },
    { name: "dense_jungle_trails", backend: "jungle_bitmask" },
    { name: "jungle_village", backend: "jungle_bitmask" },
    { name: "jungle_base", backend: "jungle_bitmask" },
    { name: "island_assault", backend: "jungle_bitmask" },
    { name: "pvp_balanced_jungle", backend: "jungle_bitmask" },
    { name: "desert_patrol", backend: "desert_adapter" },
    { name: "ice_outpost", backend: "ice_bitmask" },
    { name: "moors_wetlands", backend: "moors_adapter" },
    { name: "interior_compound", backend: "interior_adapter" },
    { name: "afx_snowfield", backend: "ice_profiled" }
];

for(var profileIndex = 0; profileIndex < profilePresets.length; ++profileIndex) {
    var preset = profilePresets[profileIndex];
    try {
        var profileContext = buildCampaignPlanForSmoke({
            Seed: 707 + profileIndex,
            ProfileName: preset.name,
            Overrides: {
                Width: 64,
                Height: 48,
                MaxRepairPasses: 1
            },
            Attempts: 1,
            RenderInvalid: true
        });
        var profileValidation = profileContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
        var profileBackend = profileContext.RenderedMap ? profileContext.RenderedMap.Backend : "none";

        print(
            "profile " + preset.name +
            " ok=" + profileValidation.ok +
            " fatal=" + profileValidation.fatal +
            " backend=" + profileBackend +
            " reasons=" + (profileValidation.reasons || []).join("|")
        );

        if(!profileValidation.ok || profileValidation.fatal || profileBackend !== preset.backend)
            ++failures;
    }
    catch(pProfileError) {
        print("profile " + preset.name + " error=" + pProfileError.message);
        ++failures;
    }
}

try {
    var iceFeatureContext = buildCampaignPlanForSmoke({
        Seed: 808,
        ProfileName: "ice_outpost",
        Overrides: {
            Width: 96,
            Height: 72,
            TerrainType: Terrain.Types.Ice,
            TerrainTypeSub: 0,
            RiverChance: 0,
            PondChance: 0,
            CoastChance: 0,
            MicroStampDensity: 4,
            MaxRepairPasses: 1
        },
        Attempts: 1,
        RenderInvalid: true
    });
    var iceFeatureValidation = iceFeatureContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
    var iceCoverTiles = countRenderedTiles(iceFeatureContext, [
        152, 153, 154, 155,
        170, 171, 172, 173, 174, 175,
        190, 191, 192, 193, 194, 195,
        210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        230, 231, 232, 233, 234, 235, 238, 239
    ]);
    var iceFeatureTiles = countRenderedTiles(iceFeatureContext, [
        35, 36, 55, 56,
        74, 75, 76, 77,
        38, 39, 58, 59,
        245, 246, 247, 264, 265, 267, 284, 285, 287
    ]);
    var misplacedIceRootTiles = countMisplacedIceRootTiles(iceFeatureContext);

    print(
        "ice_features" +
        " ok=" + iceFeatureValidation.ok +
        " fatal=" + iceFeatureValidation.fatal +
        " coverTiles=" + iceCoverTiles +
        " featureTiles=" + iceFeatureTiles +
        " misplacedRoots=" + misplacedIceRootTiles +
        " reasons=" + (iceFeatureValidation.reasons || []).join("|")
    );

    if(!iceFeatureValidation.ok || iceFeatureValidation.fatal || iceCoverTiles < 1 || iceFeatureTiles < 1 || misplacedIceRootTiles > 0)
        ++failures;
}
catch(pIceFeatureError) {
    print("ice_features error=" + pIceFeatureError.message);
    ++failures;
}

var islandSeeds = [111, 222];
for(var islandSeedIndex = 0; islandSeedIndex < islandSeeds.length; ++islandSeedIndex) {
    try {
        var islandContext = buildCampaignPlanForSmoke({
            Seed: islandSeeds[islandSeedIndex],
            ProfileName: "island_assault",
            Overrides: {
                Width: 64,
                Height: 48,
                MaxRepairPasses: 1
            },
            Attempts: 1,
            RenderInvalid: true
        });
        var islandValidation = islandContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
        var beachCount = islandContext.Metrics && islandContext.Metrics.Counts ? islandContext.Metrics.Counts.beach || 0 : 0;
        var beachPlans = islandContext.Beaches ? islandContext.Beaches.length : 0;
        var renderedBeachTiles = countRenderedTiles(islandContext, [
            255, 256, 257, 258, 259, 275, 276, 277, 278, 279,
            293, 295, 296, 297, 315, 316, 317, 324, 335, 336,
            337, 353, 354, 373, 374
        ]);

        print(
            "island_assault seed=" + islandSeeds[islandSeedIndex] +
            " ok=" + islandValidation.ok +
            " fatal=" + islandValidation.fatal +
            " beaches=" + beachPlans +
            " beachTiles=" + beachCount +
            " renderedBeachTiles=" + renderedBeachTiles +
            " reasons=" + (islandValidation.reasons || []).join("|")
        );

        if(!islandValidation.ok || islandValidation.fatal || beachPlans < 1 || beachCount < 1 || renderedBeachTiles < 1)
            ++failures;
    }
    catch(pIslandError) {
        print("island_assault seed=" + islandSeeds[islandSeedIndex] + " error=" + pIslandError.message);
        ++failures;
    }
}

var hazardTerrains = [
    Terrain.Types.Jungle,
    Terrain.Types.Ice,
    Terrain.Types.Moors,
    Terrain.Types.AmigaFormat
];

for(var hazardIndex = 0; hazardIndex < hazardTerrains.length; ++hazardIndex) {
    var hazardTerrain = hazardTerrains[hazardIndex];
    try {
        var hazardContext = buildCampaignPlanForSmoke({
            Seed: 909 + hazardIndex,
            ProfileName: "classic_jungle",
            Overrides: {
                Width: 64,
                Height: 48,
                TerrainType: hazardTerrain,
                TerrainTypeSub: 0,
                RiverChance: 1,
                MaxRiverCount: 1,
                RiverWidth: 2,
                CrossingCount: 2,
                PondChance: 0,
                CoastChance: 0,
                MaxRepairPasses: 1,
                LayoutTemplates: { classic: 1.0 },
                PathStyles: { bezier: 1.0 }
            },
            Attempts: 1,
            RenderInvalid: true
        });
        var hazardValidation = hazardContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
        var hazardRivers = hazardContext.Rivers ? hazardContext.Rivers.length : 0;
        var hazardCrossings = hazardContext.Crossings ? hazardContext.Crossings.length : 0;
        var hazardRiverBanks = hazardTerrain === Terrain.Types.Jungle ? countRiverBankNearRivers(hazardContext) : 0;
        var fordWaterTiles = hazardTerrain === Terrain.Types.Jungle ? countFordRenderedWaterTiles(hazardContext, [326, 346]) : 0;

        print(
            "soft_hazard " + terrainNames[hazardTerrain] +
            " ok=" + hazardValidation.ok +
            " fatal=" + hazardValidation.fatal +
            " rivers=" + hazardRivers +
            " crossings=" + hazardCrossings +
            " riverBankNearRiver=" + hazardRiverBanks +
            " fordWaterTiles=" + fordWaterTiles +
            " reasons=" + (hazardValidation.reasons || []).join("|")
        );

        if(!hazardValidation.ok || hazardValidation.fatal || hazardRivers < 1 || hazardCrossings < 1 ||
            hazardRiverBanks > 0 || fordWaterTiles > 0)
            ++failures;
    }
    catch(pHazardError) {
        print("soft_hazard " + terrainNames[hazardTerrain] + " error=" + pHazardError.message);
        ++failures;
    }
}

try {
    var interiorLiquidContext = buildCampaignPlanForSmoke({
        Seed: 919,
        ProfileName: "classic_jungle",
        Overrides: {
            Width: 64,
            Height: 48,
            TerrainType: Terrain.Types.Interior,
            TerrainTypeSub: 0,
            RiverChance: 1,
            MaxRiverCount: 1,
            PondChance: 1,
            MaxPondCount: 3,
            CoastChance: 0,
            MaxRepairPasses: 1
        },
        Attempts: 1,
        RenderInvalid: true
    });
    var interiorLiquidValidation = interiorLiquidContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
    var interiorRivers = interiorLiquidContext.Rivers ? interiorLiquidContext.Rivers.length : 0;
    var interiorPonds = interiorLiquidContext.Ponds ? interiorLiquidContext.Ponds.length : 0;

    print(
        "water_policy interior" +
        " ok=" + interiorLiquidValidation.ok +
        " fatal=" + interiorLiquidValidation.fatal +
        " rivers=" + interiorRivers +
        " ponds=" + interiorPonds +
        " reasons=" + (interiorLiquidValidation.reasons || []).join("|")
    );

    if(!interiorLiquidValidation.ok || interiorLiquidValidation.fatal || interiorRivers !== 0 || interiorPonds < 1)
        ++failures;
}
catch(pInteriorLiquidError) {
    print("water_policy interior error=" + pInteriorLiquidError.message);
    ++failures;
}

try {
    var desertWaterContext = buildCampaignPlanForSmoke({
        Seed: 929,
        ProfileName: "classic_jungle",
        Overrides: {
            Width: 64,
            Height: 48,
            TerrainType: Terrain.Types.Desert,
            TerrainTypeSub: 0,
            RiverChance: 1,
            MaxRiverCount: 1,
            PondChance: 1,
            MaxPondCount: 3,
            CoastChance: 1,
            MaxRepairPasses: 1
        },
        Attempts: 1,
        RenderInvalid: true
    });
    var desertWaterValidation = desertWaterContext.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
    var desertRivers = desertWaterContext.Rivers ? desertWaterContext.Rivers.length : 0;
    var desertPonds = desertWaterContext.Ponds ? desertWaterContext.Ponds.length : 0;
    var desertBeaches = desertWaterContext.Beaches ? desertWaterContext.Beaches.length : 0;

    print(
        "water_policy desert" +
        " ok=" + desertWaterValidation.ok +
        " fatal=" + desertWaterValidation.fatal +
        " rivers=" + desertRivers +
        " ponds=" + desertPonds +
        " beaches=" + desertBeaches +
        " reasons=" + (desertWaterValidation.reasons || []).join("|")
    );

    if(!desertWaterValidation.ok || desertWaterValidation.fatal || desertRivers !== 0 || desertPonds !== 0 || desertBeaches !== 0)
        ++failures;
}
catch(pDesertWaterError) {
    print("water_policy desert error=" + pDesertWaterError.message);
    ++failures;
}

print("failures=" + failures);
if(failures)
    WScript.Quit(1);
