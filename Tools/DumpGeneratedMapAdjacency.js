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

function argumentValue(pName, pDefault) {
    var prefix = "--" + pName + "=";

    for(var index = 0; index < WScript.Arguments.Length; ++index) {
        var value = String(WScript.Arguments.Item(index));
        if(value.indexOf(prefix) === 0)
            return value.substring(prefix.length);
    }

    return pDefault;
}

function splitList(pValue) {
    var result = [];
    var parts = String(pValue || "").split(",");

    for(var index = 0; index < parts.length; ++index) {
        var item = parts[index].replace(/^\s+|\s+$/g, "");
        if(item)
            result.push(item);
    }

    return result;
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

var terrainTypes = {
    jungle: Terrain.Types.Jungle,
    desert: Terrain.Types.Desert,
    ice: Terrain.Types.Ice,
    moors: Terrain.Types.Moors,
    interior: Terrain.Types.Interior,
    amigaFormat: Terrain.Types.AmigaFormat,
    afx: Terrain.Types.AmigaFormat
};

var terrainNames = {};
terrainNames[Terrain.Types.Jungle] = "jungle";
terrainNames[Terrain.Types.Desert] = "desert";
terrainNames[Terrain.Types.Ice] = "ice";
terrainNames[Terrain.Types.Moors] = "moors";
terrainNames[Terrain.Types.Interior] = "interior";
terrainNames[Terrain.Types.AmigaFormat] = "amigaFormat";

function addCount(pTable, pKey, pAmount) {
    if(!pTable[pKey])
        pTable[pKey] = 0;

    pTable[pKey] += pAmount || 1;
}

function sortedKeysByCount(pTable) {
    var keys = [];

    for(var key in pTable) {
        if(pTable.hasOwnProperty(key))
            keys.push(key);
    }

    keys.sort(function(pLeft, pRight) {
        var diff = pTable[pRight] - pTable[pLeft];
        if(diff !== 0)
            return diff;

        return String(pLeft) < String(pRight) ? -1 : 1;
    });

    return keys;
}

function pairKey(pLeft, pRight) {
    return pLeft <= pRight ? pLeft + "|" + pRight : pRight + "|" + pLeft;
}

function createStats() {
    return {
        maps: 0,
        cells: 0,
        pairSamples: 0,
        failures: 0,
        tiles: {},
        pairs: {},
        isolated: {},
        backends: {}
    };
}

function sameTileAround(pTiles, pWidth, pHeight, pX, pY, pTile) {
    for(var dx = -1; dx <= 1; ++dx) {
        for(var dy = -1; dy <= 1; ++dy) {
            if(dx === 0 && dy === 0)
                continue;

            var x = pX + dx;
            var y = pY + dy;
            if(x < 0 || y < 0 || x >= pWidth || y >= pHeight)
                continue;

            if(Number(MapGen.Layers.Get(pTiles, x, y, -1)) === pTile)
                return true;
        }
    }

    return false;
}

function recordTiles(pStats, pTiles, pWidth, pHeight) {
    for(var y = 0; y < pHeight; ++y) {
        for(var x = 0; x < pWidth; ++x) {
            var tile = Number(MapGen.Layers.Get(pTiles, x, y, 0));
            addCount(pStats.tiles, tile, 1);
            ++pStats.cells;

            if(!sameTileAround(pTiles, pWidth, pHeight, x, y, tile))
                addCount(pStats.isolated, tile, 1);

            if(x + 1 < pWidth) {
                addCount(pStats.pairs, pairKey(tile, Number(MapGen.Layers.Get(pTiles, x + 1, y, 0))), 1);
                ++pStats.pairSamples;
            }

            if(y + 1 < pHeight) {
                addCount(pStats.pairs, pairKey(tile, Number(MapGen.Layers.Get(pTiles, x, y + 1, 0))), 1);
                ++pStats.pairSamples;
            }
        }
    }
}

function emitCounts(pPrefix, pTerrain, pTable) {
    var keys = sortedKeysByCount(pTable);

    for(var index = 0; index < keys.length; ++index)
        print(pPrefix + " terrain=" + pTerrain + " key=" + keys[index] + " count=" + pTable[keys[index]]);
}

var seeds = splitList(argumentValue("seeds", "101,202,303,404,505"));
var terrainArgs = splitList(argumentValue("terrains", "jungle,desert,ice,moors,interior,amigaFormat"));
var width = Number(argumentValue("width", "64"));
var height = Number(argumentValue("height", "48"));
var profileName = argumentValue("profile", "classic_jungle");
var statsByTerrain = {};

for(var terrainIndex = 0; terrainIndex < terrainArgs.length; ++terrainIndex) {
    var requestedTerrain = terrainArgs[terrainIndex];
    var terrainType = terrainTypes[requestedTerrain];

    if(typeof terrainType === "undefined") {
        print("error terrain=" + requestedTerrain + " message=unknown_terrain");
        WScript.Quit(2);
    }

    var terrainName = terrainNames[terrainType];
    var stats = createStats();
    statsByTerrain[terrainName] = stats;

    for(var seedIndex = 0; seedIndex < seeds.length; ++seedIndex) {
        var seed = Number(seeds[seedIndex]);

        try {
            var context = MapGen.BuildCampaignPlan({
                Seed: seed,
                ProfileName: profileName,
                Overrides: {
                    Width: width,
                    Height: height,
                    TerrainType: terrainType,
                    TerrainTypeSub: 0,
                    MaxRepairPasses: 1
                },
                Attempts: 1,
                RenderInvalid: true
            });
            var validation = context.Validation || { ok: false, fatal: true, reasons: ["missing_validation"] };
            var rendered = context.RenderedMap;
            var backend = rendered ? rendered.Backend : "none";

            addCount(stats.backends, backend, 1);

            if(!validation.ok || validation.fatal || !rendered || !rendered.Tiles) {
                ++stats.failures;
                print("map terrain=" + terrainName + " seed=" + seed + " ok=" + validation.ok + " fatal=" + validation.fatal + " backend=" + backend + " skipped=true");
                continue;
            }

            ++stats.maps;
            recordTiles(stats, rendered.Tiles, width, height);
            print("map terrain=" + terrainName + " seed=" + seed + " ok=" + validation.ok + " fatal=" + validation.fatal + " backend=" + backend + " skipped=false");
        }
        catch(pError) {
            ++stats.failures;
            print("map terrain=" + terrainName + " seed=" + seed + " error=" + String(pError.message).replace(/\s+/g, "_") + " skipped=true");
        }
    }
}

for(var name in statsByTerrain) {
    if(!statsByTerrain.hasOwnProperty(name))
        continue;

    var terrainStats = statsByTerrain[name];
    var backendParts = [];
    var backendKeys = sortedKeysByCount(terrainStats.backends);

    for(var backendIndex = 0; backendIndex < backendKeys.length; ++backendIndex)
        backendParts.push(backendKeys[backendIndex] + ":" + terrainStats.backends[backendKeys[backendIndex]]);

    print(
        "summary terrain=" + name +
        " maps=" + terrainStats.maps +
        " cells=" + terrainStats.cells +
        " pairSamples=" + terrainStats.pairSamples +
        " uniqueTiles=" + sortedKeysByCount(terrainStats.tiles).length +
        " uniquePairs=" + sortedKeysByCount(terrainStats.pairs).length +
        " isolatedTiles=" + sortedKeysByCount(terrainStats.isolated).length +
        " failures=" + terrainStats.failures +
        " backends=" + backendParts.join(",")
    );
    emitCounts("tile", name, terrainStats.tiles);
    emitCounts("pair", name, terrainStats.pairs);
    emitCounts("isolated", name, terrainStats.isolated);
}
