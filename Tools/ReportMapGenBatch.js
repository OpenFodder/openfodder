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

function formatPercent(pValue) {
    return (Math.round((pValue || 0) * 10000) / 100) + "%";
}

function keysForReport(pMap) {
    var keys = [];

    for(var key in pMap) {
        if(pMap.hasOwnProperty(key))
            keys.push(key + "=" + pMap[key]);
    }

    keys.sort();
    return keys.length ? keys.join("|") : "none";
}

function runCase(pCase, pCount, pAttempts, pBaseSeed) {
    var summary = MapGen.Batch.Run({
        Mode: pCase.mode || "campaign",
        ProfileName: pCase.profile,
        Count: pCount,
        Seed: pBaseSeed + (pCase.offset || 0),
        Attempts: pAttempts,
        UseRetries: true,
        Overrides: {
            Width: pCase.width || 64,
            Height: pCase.height || 48,
            MaxRepairPasses: 1
        }
    });

    print(
        "batch mode=" + summary.Mode +
        " profile=" + summary.ProfileName +
        " count=" + summary.Count +
        " ok=" + summary.Ok +
        " failed=" + summary.Failed +
        " fatal=" + summary.Fatal +
        " passRate=" + formatPercent(summary.PassRate) +
        " fatalRate=" + formatPercent(summary.FatalRate) +
        " water=" + formatPercent(summary.Average.waterCoverage) +
        " blocked=" + formatPercent(summary.Average.blockedCoverage) +
        " walkable=" + formatPercent(summary.Average.largestWalkableComponentCoverage) +
        " clearings=" + Math.round(summary.Average.clearings * 10) / 10 +
        " crossings=" + Math.round(summary.Average.crossings * 10) / 10 +
        " reasons=" + keysForReport(summary.Reasons) +
        " warnings=" + keysForReport(summary.Warnings)
    );

    if(summary.FailureReplays.length) {
        for(var replayIndex = 0; replayIndex < summary.FailureReplays.length; ++replayIndex) {
            var replay = summary.FailureReplays[replayIndex];
            print(
                "  replay mode=" + replay.Mode +
                " profile=" + replay.ProfileName +
                " seed=" + replay.Seed +
                " reasons=" + (replay.Reasons || []).join("|")
            );
        }
    }

    return summary.Failed === 0 && summary.Fatal === 0;
}

var count = Math.max(1, parseInt(argumentValue("count", "10"), 10));
var attempts = Math.max(1, parseInt(argumentValue("attempts", "3"), 10));
var baseSeed = parseInt(argumentValue("seed", "31001"), 10);
var cases = [
    { profile: "classic_jungle", offset: 0 },
    { profile: "dense_jungle_trails", offset: 1000 },
    { profile: "jungle_village", offset: 2000 },
    { profile: "jungle_base", offset: 3000 },
    { profile: "island_assault", offset: 4000 },
    { profile: "desert_patrol", offset: 5000 },
    { profile: "ice_outpost", offset: 6000 },
    { profile: "moors_wetlands", offset: 7000 },
    { profile: "interior_compound", offset: 8000 },
    { profile: "afx_snowfield", offset: 9000 },
    { mode: "multiplayer", profile: "pvp_balanced_jungle", offset: 10000 }
];
var failures = 0;

print("MapGen batch report count=" + count + " attempts=" + attempts + " seed=" + baseSeed);

for(var caseIndex = 0; caseIndex < cases.length; ++caseIndex) {
    if(!runCase(cases[caseIndex], count, attempts, baseSeed))
        ++failures;
}

print("failures=" + failures);

if(failures)
    WScript.Quit(1);
