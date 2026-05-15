/* global WScript, ActiveXObject */

// Regression-test the ice tree rule data against real CF1 ice maps.
//
// For each .map in args (default: mapm11), we:
//   1. Load tiles from the binary.
//   2. Identify tree cells (TREES set).
//   3. Build a chars layer with "T" where the original has a tree tile,
//      and "#" everywhere else.
//   4. Run Core.ApplyTreeRules with our bm_cf1_ice_tree data into a fresh
//      tiles layer.
//   5. Compare the placed tile IDs against the original tile IDs at every
//      tree cell. Report match rate, confusion pairs, and per-bm-vector
//      match rate (so we can see which rules need work).
//
// Usage:
//   cscript //nologo Tools\MatchIceTreeRules.js [--maps=mapm11,mapm15]
//                                                [--top=20]
//                                                [--show-mismatches]

var fso = new ActiveXObject("Scripting.FileSystemObject");
var ado = new ActiveXObject("ADODB.Stream");
var root = fso.GetAbsolutePathName(".");

function print(pMessage) { WScript.Echo(String(pMessage)); }

if(!Array.prototype.indexOf) {
    Array.prototype.indexOf = function(pSearch) {
        for(var index = 0; index < this.length; ++index)
            if(this[index] === pSearch) return index;
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

function hasFlag(pName) {
    var flag = "--" + pName;
    for(var index = 0; index < WScript.Arguments.Length; ++index)
        if(String(WScript.Arguments.Item(index)) === flag)
            return true;
    return false;
}

function readScript(pPath) {
    var path = fso.BuildPath(root, pPath);
    var file = fso.OpenTextFile(path, 1, false);
    var code = file.ReadAll();
    file.Close();
    code = code.replace(/\bconst\b/g, "var").replace(/\blet\b/g, "var");
    return "\n// " + pPath + "\n" + code + "\n";
}

function readBinary(pPath) {
    ado.Type = 1;
    ado.Open();
    ado.LoadFromFile(pPath);
    var bytes = ado.Read();
    ado.Close();
    var arr = [];
    for(var index = 0; index < bytes.lbound; ++index) arr.push(0);
    // ADO returns SafeArray of bytes; convert via VBScript helper.
    return bytes;
}

// JScript can't easily index a VARIANT byte array. Use a tiny helper file
// stream that returns a string we can charCodeAt instead.
function readBytesAsCodes(pPath) {
    var stream = new ActiveXObject("ADODB.Stream");
    stream.Type = 1;
    stream.Open();
    stream.LoadFromFile(pPath);
    var bin = stream.Read();
    stream.Close();
    // Convert via a temporary text stream with binary-to-text trick.
    var s2 = new ActiveXObject("ADODB.Stream");
    s2.Type = 1;
    s2.Open();
    s2.Write(bin);
    s2.Position = 0;
    s2.Type = 2;
    s2.Charset = "iso-8859-1";
    var text = s2.ReadText();
    s2.Close();
    var out = [];
    for(var index = 0; index < text.length; ++index)
        out.push(text.charCodeAt(index) & 0xFF);
    return out;
}

function readBEWord(pBytes, pOffset) {
    return ((pBytes[pOffset] & 0xFF) << 8) | (pBytes[pOffset + 1] & 0xFF);
}

function loadMap(pPath) {
    var bytes = readBytesAsCodes(pPath);
    var width = readBEWord(bytes, 0x54);
    var height = readBEWord(bytes, 0x56);
    var tiles = [];
    for(var index = 0; index < width * height; ++index)
        tiles.push(readBEWord(bytes, 0x60 + (index * 2)));
    return { width: width, height: height, tiles: tiles };
}

var TREE_IDS = [
    152, 153, 154, 155,
    170, 171, 172, 173, 174, 175,
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    251, 360, 380
];
var TREE_SET = {};
for(var ti = 0; ti < TREE_IDS.length; ++ti) TREE_SET[TREE_IDS[ti]] = true;

function isTreeTile(pTile) { return TREE_SET[pTile] === true; }

// Bring in just enough of the runtime to call Core.ApplyTreeRules.
var scripts = [
    "Run/Scripts/Common/Terrain.js",
    "Run/Scripts/MapGen/Random.js",
    "Run/Scripts/MapGen/Layers.js",
    "Run/Scripts/MapGen/Context.js",
    "Run/Scripts/MapGen/Render.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Core.js",
    "Run/Scripts/MapGen/Terrain/Smoothing/Data/IceData.js"
];
var combined = "";
for(var si = 0; si < scripts.length; ++si)
    combined += readScript(scripts[si]);
eval(combined);

var iceData = MapGen.Terrain.Smoothing.IceData;
if(!iceData || !iceData.bitmasks || !iceData.bitmasks.bm_cf1_ice_tree) {
    print("error: ice tree bitmask data not found");
    WScript.Quit(1);
}
var treeRules = iceData.bitmasks.bm_cf1_ice_tree;
var treeColumns = iceData.treeColumns;
var Core = MapGen.Terrain.Smoothing.Core;
var useColumns = !hasFlag("legacy-bm");

function buildCharsFromTiles(pMap) {
    var chars = MapGen.Layers.Create(pMap.width, pMap.height, "#");
    for(var y = 0; y < pMap.height; ++y) {
        for(var x = 0; x < pMap.width; ++x) {
            var tile = pMap.tiles[(y * pMap.width) + x];
            if(isTreeTile(tile))
                MapGen.Layers.Set(chars, x, y, "T");
        }
    }
    return chars;
}

function coverBitmask(pChars, pX, pY) {
    var offsets = [
        [-1, -1], [0, -1], [1, -1],
        [-1, 0], [1, 0],
        [-1, 1], [0, 1], [1, 1]
    ];
    var bits = "";
    for(var oi = 0; oi < offsets.length; ++oi) {
        var x = pX + offsets[oi][0];
        var y = pY + offsets[oi][1];
        var ch = MapGen.Layers.Get(pChars, x, y, "");
        bits += ch === "T" ? "1" : "0";
    }
    return bits;
}

function runTreePlacement(pMap, pChars) {
    var tiles = MapGen.Layers.Create(pMap.width, pMap.height, 0);
    var context = { Width: pMap.width, Height: pMap.height, Seed: 0, Layers: { occupied: MapGen.Layers.Create(pMap.width, pMap.height, 0) } };
    if(useColumns && treeColumns) {
        Core.ApplyTreeColumns(context, pChars, tiles, treeColumns, function() { return false; });
    } else {
        Core.ApplyTreeRules(context, pChars, tiles, treeRules, function() { return false; });
    }
    return tiles;
}

function compare(pName, pMap, pPlaced, pChars, pShowMismatches) {
    var total = 0;
    var matched = 0;
    var byOriginal = {};      // orig -> {placed -> count}
    var byVector = {};        // bm vector -> {match, total}
    var mismatchesShown = 0;
    var mismatchLimit = 25;

    for(var y = 0; y < pMap.height; ++y) {
        for(var x = 0; x < pMap.width; ++x) {
            if(MapGen.Layers.Get(pChars, x, y, "") !== "T") continue;

            var orig = pMap.tiles[(y * pMap.width) + x];
            var placed = MapGen.Layers.Get(pPlaced, x, y, 0);
            var bm = coverBitmask(pChars, x, y);

            if(!byVector[bm]) byVector[bm] = { match: 0, total: 0, miss: {} };
            byVector[bm].total += 1;

            ++total;
            if(orig === placed) {
                ++matched;
                byVector[bm].match += 1;
                continue;
            }

            if(!byOriginal[orig]) byOriginal[orig] = {};
            byOriginal[orig][placed] = (byOriginal[orig][placed] || 0) + 1;

            var key = orig + "->" + placed;
            byVector[bm].miss[key] = (byVector[bm].miss[key] || 0) + 1;

            if(pShowMismatches && mismatchesShown < mismatchLimit) {
                print("  mismatch " + pName + " (" + x + "," + y + ") bm=" + bm + " orig=" + orig + " placed=" + placed);
                ++mismatchesShown;
            }
        }
    }

    print("");
    print("=== " + pName + ": " + matched + "/" + total + " (" + Math.round(100 * matched / Math.max(1, total)) + "%) trees matched ===");

    // Confusion matrix: top original-tile mismatches.
    var confusion = [];
    for(var origKey in byOriginal) {
        if(!byOriginal.hasOwnProperty(origKey)) continue;
        var subtotal = 0;
        var topPlaced = -1;
        var topCount = 0;
        for(var pkey in byOriginal[origKey]) {
            if(!byOriginal[origKey].hasOwnProperty(pkey)) continue;
            var c = byOriginal[origKey][pkey];
            subtotal += c;
            if(c > topCount) { topCount = c; topPlaced = Number(pkey); }
        }
        confusion.push({ orig: Number(origKey), total: subtotal, topPlaced: topPlaced, topCount: topCount });
    }
    confusion.sort(function(a, b) { return b.total - a.total; });
    print("Top mismatched original tiles (orig: total-misses, most-frequent-replacement):");
    for(var ci = 0; ci < Math.min(15, confusion.length); ++ci) {
        var entry = confusion[ci];
        print("  " + entry.orig + ": " + entry.total + " misses, top -> " + entry.topPlaced + " (x" + entry.topCount + ")");
    }

    // Per-bm-vector match rate, sorted by total cells.
    var vectorList = [];
    for(var vkey in byVector) {
        if(!byVector.hasOwnProperty(vkey)) continue;
        vectorList.push({ vector: vkey, match: byVector[vkey].match, total: byVector[vkey].total, miss: byVector[vkey].miss });
    }
    vectorList.sort(function(a, b) { return b.total - a.total; });
    print("Per-bitmask match rate (top 12 by frequency):");
    for(var vi = 0; vi < Math.min(12, vectorList.length); ++vi) {
        var v = vectorList[vi];
        var pct = Math.round(100 * v.match / Math.max(1, v.total));
        print("  bm=" + v.vector + "  " + v.match + "/" + v.total + " (" + pct + "%)");
        // Top miss for this vector
        var topMiss = "";
        var topMissCount = 0;
        for(var mkey in v.miss) {
            if(!v.miss.hasOwnProperty(mkey)) continue;
            if(v.miss[mkey] > topMissCount) {
                topMissCount = v.miss[mkey];
                topMiss = mkey;
            }
        }
        if(topMissCount > 0)
            print("    top miss: " + topMiss + " (x" + topMissCount + ")");
    }

    return { total: total, matched: matched };
}

var mapsArg = argumentValue("maps", "mapm11");
var showMismatches = hasFlag("show-mismatches");
var mapNames = mapsArg.split(",");

var grandTotal = 0;
var grandMatched = 0;
for(var mi = 0; mi < mapNames.length; ++mi) {
    var name = mapNames[mi].toString();
    if(name.length === 0) continue;
    var path = fso.BuildPath(root, "Run\\Data\\Amiga\\" + name + ".map");
    if(!fso.FileExists(path)) {
        print("skip: " + path + " (not found)");
        continue;
    }

    var map = loadMap(path);
    var chars = buildCharsFromTiles(map);
    var placed = runTreePlacement(map, chars);
    var result = compare(name, map, placed, chars, showMismatches);
    grandTotal += result.total;
    grandMatched += result.matched;
}

print("");
print("=== overall: " + grandMatched + "/" + grandTotal + " (" + Math.round(100 * grandMatched / Math.max(1, grandTotal)) + "%) trees matched across " + mapNames.length + " map(s) ===");
