/* global WScript, ActiveXObject */
// Print tile IDs at given (x,y) coords for a .map file.
// Usage: cscript //nologo Tools\PeekTilesAt.js Run\random.map 14,47 16,48 19,48 20,47

var fso = new ActiveXObject("Scripting.FileSystemObject");

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

var args = WScript.Arguments;
if(args.Length < 2) { WScript.Echo("usage: PeekTilesAt.js <map> <x,y> [<x,y>...]"); WScript.Quit(1); }

var path = String(args.Item(0));
var b = readBytesAsCodes(path);
var w = be(b, 0x54), h = be(b, 0x56);

WScript.Echo("map=" + path + " size=" + w + "x" + h);

for(var i = 1; i < args.Length; i++) {
    var pair = String(args.Item(i)).split(",");
    var x = parseInt(pair[0], 10);
    var y = parseInt(pair[1], 10);
    if(x < 0 || y < 0 || x >= w || y >= h) {
        WScript.Echo("(" + x + "," + y + "): out of bounds");
        continue;
    }
    var tile = be(b, 0x60 + ((y * w + x) * 2));
    // Also show 3x3 neighborhood
    var nb = [];
    for(var dy = -1; dy <= 1; dy++) {
        var row = [];
        for(var dx = -1; dx <= 1; dx++) {
            var nx = x + dx, ny = y + dy;
            if(nx < 0 || ny < 0 || nx >= w || ny >= h) row.push("---");
            else row.push(("" + be(b, 0x60 + ((ny * w + nx) * 2))).replace(/^/, "   ").slice(-3));
        }
        nb.push(row.join(" "));
    }
    WScript.Echo("(" + x + "," + y + "): tile=" + tile);
    for(var k = 0; k < 3; k++) WScript.Echo("    " + nb[k]);
}
