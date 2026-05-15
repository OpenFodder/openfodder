/* global WScript, ActiveXObject */
var fso = new ActiveXObject("Scripting.FileSystemObject");
var bs = new ActiveXObject("ADODB.Stream");
bs.Type = 1; bs.Open();
bs.LoadFromFile("Run/random.map");
var bytes = bs.Read(); bs.Close();

var probes = [[60,31],[61,31],[62,31],[63,31],
              [60,32],[61,32],[62,32],[63,32],
              [60,33],[61,33],[62,33],[63,33],
              [60,34],[61,34],[62,34],[63,34]];

function getTile(x, y) {
    var off = (y * 96 + x) * 2;
    var b0 = bytes.charCodeAt ? bytes.charCodeAt(off) : 0;
    return b0; // map files use 1 byte per tile in some formats
}

// Try reading as binary blob
var fs = new ActiveXObject("Scripting.FileSystemObject");
var f = fs.GetFile("Run/random.map");
WScript.Echo("size=" + f.Size);

