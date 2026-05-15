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

function cPosition(pX, pY) {
    this.x = pX || 0;
    this.y = pY || 0;
}

var scripts = [
    "Run/Scripts/Common/Terrain.js",
    "Run/Scripts/Common/Sprites.js",
    "Run/Scripts/Common/Multiplayer.js",
    "Run/Scripts/Common/Session.js",
    "Run/Scripts/Objectives/Base.js",
    "Run/Scripts/Settings.js",
    "Run/Scripts/Common/Structures.js",
    "Run/Scripts/Common/Structures/Hut.js",
    "Run/Scripts/Common/Structures/Barracks.js",
    "Run/Scripts/Common/Structures/Bunker.js",
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
    "Run/Scripts/MapGen/Integration.js"
];

for(var scriptIndex = 0; scriptIndex < scripts.length; ++scriptIndex)
    eval(readScript(scripts[scriptIndex]));

var Map = {
    seed: 0,
    width: 0,
    height: 0,
    terrainType: Terrain.Types.Jungle,
    terrainTypeSub: 0,
    tiles: [],
    sprites: [],

    Create: function(pWidth, pHeight, pTerrainType, pTerrainTypeSub) {
        this.width = pWidth;
        this.height = pHeight;
        this.terrainType = pTerrainType;
        this.terrainTypeSub = pTerrainTypeSub || 0;
        this.tiles = [];
        this.sprites = [];

        for(var x = 0; x < pWidth; ++x) {
            this.tiles[x] = [];
            for(var y = 0; y < pHeight; ++y)
                this.tiles[x][y] = 0;
        }
    },

    TileSet: function(pX, pY, pTile) {
        if(pX < 0 || pY < 0 || pX >= this.width || pY >= this.height)
            return;

        this.tiles[pX][pY] = pTile;
    },

    TileGet: function(pX, pY) {
        if(pX < 0 || pY < 0 || pX >= this.width || pY >= this.height)
            return -1;

        return this.tiles[pX][pY];
    },

    SpriteAdd: function(pType, pX, pY) {
        this.sprites.push({
            type: pType,
            position: new cPosition(pX, pY),
            getPosition: function() {
                return this.position;
            }
        });
    },

    getSpriteTypeCount: function(pType) {
        var count = 0;
        for(var index = 0; index < this.sprites.length; ++index) {
            if(this.sprites[index].type === pType)
                ++count;
        }

        return count;
    },

    getSpritesByType: function(pType) {
        var sprites = [];
        for(var index = 0; index < this.sprites.length; ++index) {
            if(this.sprites[index].type === pType)
                sprites.push(this.sprites[index]);
        }

        return sprites;
    },

    getWidth: function() {
        return this.width;
    },

    getHeight: function() {
        return this.height;
    },

    getArea: function() {
        return this.width * this.height;
    },

    getTileType: function() {
        return this.terrainType;
    },

    getRandomInt: function(pMin, pMax) {
        this.seed = MapGen.Random.DeriveSeed(this.seed || 1, 1);
        return pMin + (Math.abs(this.seed) % ((pMax - pMin) + 1));
    }
};

var terrainNames = {};
terrainNames[Terrain.Types.Jungle] = "jungle";
terrainNames[Terrain.Types.Desert] = "desert";
terrainNames[Terrain.Types.Ice] = "ice";
terrainNames[Terrain.Types.Moors] = "moors";
terrainNames[Terrain.Types.Interior] = "interior";
terrainNames[Terrain.Types.AmigaFormat] = "amigaFormat";

var failures = 0;
var terrains = [
    Terrain.Types.Jungle,
    Terrain.Types.Desert,
    Terrain.Types.Ice,
    Terrain.Types.Moors,
    Terrain.Types.Interior,
    Terrain.Types.AmigaFormat
];

for(var terrainIndex = 0; terrainIndex < terrains.length; ++terrainIndex) {
    var terrainType = terrains[terrainIndex];
    var seed = 1201 + terrainIndex;

    Session.Reset();
    Map.seed = seed;
    Settings.Seed = seed;
    Settings.Width = 96;
    Settings.Height = 72;
    Settings.TerrainType = terrainType;
    Settings.TerrainTypeSub = 0;
    Settings.RandomMap.Enabled = false;
    Settings.Multiplayer.Enabled = false;
    Settings.Objectives = [Objectives.KillAllEnemy, Objectives.DestroyEnemyBuildings];
    Settings.RandomBuildings();

    MapGen.Integration.GenerateCampaignMap();

    var enemyCount = Settings.GetEnemyCount();
    var grenadeCount = Settings.GetMinimumGrenades();
    var rocketCount = Settings.GetMinimumRockets() / 2;
    var playersPlaced = MapGen.Integration.PlaceCampaignPlayers(8);
    var enemiesPlaced = MapGen.Integration.PlaceCampaignEnemies(enemyCount);
    var buildingsPlaced = MapGen.Integration.PlaceCampaignBuildings(Settings.GetEnemyBuildingCount(), "enemy");
    var pickupPlaced = MapGen.Integration.PlaceCampaignPickups(grenadeCount, rocketCount);
    var decorPlaced = MapGen.Integration.PlaceMapDecor({
        Palms: 999,
        Bushes1: 999,
        Blooms: 999
    });
    var liveValidation = MapGen.Integration.ValidateMaterializedCampaign({
        players: 8,
        enemies: enemyCount,
        enemyBuildings: 1,
        totalStructures: 1,
        grenadeBoxes: grenadeCount,
        rocketBoxes: rocketCount
    });
    var enemyBuildingCount = Session.getEnemyBuildings().length;
    var playerCount = Map.getSpriteTypeCount(SpriteTypes.Player);
    var palmCount = Map.getSpriteTypeCount(SpriteTypes.Tree);

    print(
        "integration " + terrainNames[terrainType] +
        " seed=" + seed +
        " players=" + playerCount +
        " enemyBuildings=" + enemyBuildingCount +
        " palms=" + palmCount +
        " playersPlaced=" + playersPlaced +
        " enemiesPlaced=" + enemiesPlaced +
        " buildingsPlaced=" + buildingsPlaced +
        " pickupPlaced=" + pickupPlaced +
        " decorPlaced=" + decorPlaced +
        " liveOk=" + liveValidation.ok +
        " liveReasons=" + liveValidation.reasons.join("|")
    );

    if(!playersPlaced || playerCount < 8 || !enemiesPlaced || !buildingsPlaced || enemyBuildingCount < 1 ||
        !pickupPlaced || !decorPlaced || !liveValidation.ok)
        ++failures;
}

print("failures=" + failures);
if(failures)
    WScript.Quit(1);
