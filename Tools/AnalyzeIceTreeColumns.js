/* global WScript, ActiveXObject */

// Per-column histogram of CF1 ice tree groups in a real map.
// For every (x, y_top..y_bot) tree column, classify by:
//   - height (cell count)
//   - leftPaired (tree at x-1, y_bot)
//   - rightPaired (tree at x+1, y_bot)
// And report frequency + the actual tile sequence used in that column.

var fso = new ActiveXObject("Scripting.FileSystemObject");
var root = fso.GetAbsolutePathName(".");

function print(m) { WScript.Echo(String(m)); }

function readBytesAsCodes(p) {
    var s = new ActiveXObject("ADODB.Stream");
    s.Type = 1; s.Open(); s.LoadFromFile(p);
    var bin = s.Read(); s.Close();
    var s2 = new ActiveXObject("ADODB.Stream");
    s2.Type = 1; s2.Open(); s2.Write(bin); s2.Position = 0;
    s2.Type = 2; s2.Charset = "iso-8859-1";
    var t = s2.ReadText(); s2.Close();
    var out = [];
    for(var i = 0; i < t.length; i++) out.push(t.charCodeAt(i) & 0xFF);
    return out;
}

function be(b, o) { return ((b[o] & 0xFF) << 8) | (b[o + 1] & 0xFF); }

function loadMap(p) {
    var b = readBytesAsCodes(p);
    var w = be(b, 0x54), h = be(b, 0x56);
    var tiles = [];
    for(var i = 0; i < w * h; i++) tiles.push(be(b, 0x60 + (i * 2)));
    return { width: w, height: h, tiles: tiles };
}

var TREE_IDS = [
    152,153,154,155, 170,171,172,173,174,175,
    190,191,192,193,194,195,196,197,198,199,
    210,211,212,213,214,215,216,217,218,219,
    230,231,232,233,234,235,236,237,238,239,
    251, 360, 380
];
var TREE_SET = {};
for(var i = 0; i < TREE_IDS.length; i++) TREE_SET[TREE_IDS[i]] = true;
function isTree(t) { return TREE_SET[t] === true; }

function tileAt(map, x, y) {
    if(x < 0 || y < 0 || x >= map.width || y >= map.height) return 0;
    return map.tiles[(y * map.width) + x];
}

var mapName = "mapm11";
for(var ai = 0; ai < WScript.Arguments.Length; ai++) {
    var v = String(WScript.Arguments.Item(ai));
    if(v.indexOf("--map=") === 0) mapName = v.substring(6);
}

var path = fso.BuildPath(root, "Run\\Data\\Amiga\\" + mapName + ".map");
var map = loadMap(path);

var visited = [];
for(var i = 0; i < map.width; i++) {
    var col = [];
    for(var j = 0; j < map.height; j++) col.push(false);
    visited.push(col);
}

var buckets = {};        // key "h{n}_{kind}" -> { count, samples: [seq, ...] }

function bucketName(height, leftPaired, rightPaired) {
    var kind = "isolated";
    if(leftPaired && rightPaired) kind = "interior";
    else if(rightPaired) kind = "leftEdge";
    else if(leftPaired) kind = "rightEdge";
    return "h" + height + "_" + kind;
}

function recordColumn(map, x, yTop, yBot) {
    var height = yBot - yTop + 1;
    var leftPaired = isTree(tileAt(map, x - 1, yBot));
    var rightPaired = isTree(tileAt(map, x + 1, yBot));
    var bn = bucketName(height, leftPaired, rightPaired);
    if(!buckets[bn]) buckets[bn] = { count: 0, samples: {}, total: 0 };
    var seq = [];
    for(var y = yTop; y <= yBot; y++) seq.push(tileAt(map, x, y));
    var key = seq.join(",");
    buckets[bn].count++;
    buckets[bn].total += height;
    buckets[bn].samples[key] = (buckets[bn].samples[key] || 0) + 1;
}

for(var x = 0; x < map.width; x++) {
    for(var y = 0; y < map.height; y++) {
        if(visited[x][y]) continue;
        if(!isTree(tileAt(map, x, y))) continue;
        var yTop = y;
        var yBot = y;
        while(yBot + 1 < map.height && isTree(tileAt(map, x, yBot + 1))) yBot++;
        for(var k = yTop; k <= yBot; k++) visited[x][k] = true;
        recordColumn(map, x, yTop, yBot);
    }
}

// Sort buckets by count
var keys = [];
for(var k in buckets) if(buckets.hasOwnProperty(k)) keys.push(k);
keys.sort(function(a, b) { return buckets[b].count - buckets[a].count; });

print("=== " + mapName + ": column buckets (sorted by frequency) ===");
print("");
for(var ki = 0; ki < keys.length; ki++) {
    var key = keys[ki];
    var bk = buckets[key];
    print(key + ": " + bk.count + " columns (" + bk.total + " cells)");
    // Top samples
    var samps = [];
    for(var sk in bk.samples) if(bk.samples.hasOwnProperty(sk)) samps.push({ seq: sk, count: bk.samples[sk] });
    samps.sort(function(a, b) { return b.count - a.count; });
    var shown = Math.min(5, samps.length);
    for(var si = 0; si < shown; si++) {
        print("    [" + samps[si].seq + "]  x" + samps[si].count);
    }
    if(samps.length > shown) print("    (+" + (samps.length - shown) + " other variants)");
    print("");
}
