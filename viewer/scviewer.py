import webbrowser
import sys
import html
import socket
import threading

from profile import Profile
from flask import Flask, send_file, request, render_template, send_from_directory

if len(sys.argv) == 1:
    sys.exit(0)

app = Flask(__name__)
profile = Profile(sys.argv[1])

def getWordLenAprox(entry):
    freq = entry["freq"]
    iwl = int(entry["iwl"])
    wl = int(entry["wl"])

    if iwl <= 0:
        idx = -iwl - wl - 64
    else:
        idx = -wl + iwl - 64

    for i in range(idx, idx + wl):
        if freq[i] != 0:
            return wl
        wl -= 1

    return wl

def getWordLen(entry):
    bitmap = entry["bitmap"]
    wl = int(entry["wl"])
    iwl = int(entry["iwl"])
    size = len(bitmap)

    i = 0
    while i < size and bitmap[i] == 0:
        wl -= 1
        iwl -= 1
        i += 1

    if wl == 0:
        return 0, 0

    i = size - 1
    while i >= 0 and bitmap[i] == 0:
        wl -= 1
        i -= 1

    return wl, iwl

def getMarker(entry, line, pos):
    style = ""
    text = ""

    if int(entry["utilization"]) < 100:
        style = "style='background-color:red;cursor:pointer'"
        wl, iwl = getWordLen(entry)

        if wl == int(entry["wl"]):
            text = "One or more bits were not used during simulation. This may be a logic problem or the data used in your simulation is not representative. If this is not the case, try to split the computation into small chunks, then combine the bits to improve hardware utilization."
        else:
            text = "Some bits are never utilized. This may be a logic problem or the data used in your simulation is not representative. If this is not the case, change the precision to WL: " + str(wl) + " and IWL: " + str(iwl) + " to improve hardware utilization."
    elif (len(entry["freq"]) - entry["freq"].count(0)) < len(entry["bitmap"]):
        wl = getWordLenAprox(entry) + 1

        if wl < (int(entry["wl"])):
            style = "style='background-color:yellow;cursor:pointer'"
            text = "The analysis shows that some bits did not affect the overall accuracy of the application. To improve hardware utilization, you can reduce the word length (WL) to a value bigger than " + str(wl) + ", in exchange for a reduction in precision."
        else:
            style = "style='background-color:green;cursor:pointer'"
            text = "No problem found"
    else:
        style = "style='background-color:green;cursor:pointer'"
        text = "No problem found"

    return "<b class='line' id='" + str(pos) + "'><div " + style + "><div class='tooltip'>" + line + "<span class='tooltiptext'>" + text + "</span></div></div></b>"

def mark_lines(lines, entries):
    i = 0
    for entry in entries:
        idx = entry["line"]
        if not lines[idx-1].startswith("<b class"):
            lines[idx-1] = getMarker(entry, lines[idx-1], i)
        else:
            lines[idx-1] = getMarker(entry, "Note ID" + str(i) + ": multiple instantiations in this line. Check the application log for a more precise cut", i) + lines[idx-1]

        i += 1

def removeSpecialCharacters(data):
    return html.escape(data)

def pickAnUnusedPort():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('localhost', 0))
    _, port = s.getsockname()
    s.close()
    return port

@app.route("/")
def index():
    files = []
    for file in profile.files:
        files.append(file["path"])
    return render_template("index.html", file_list=files)

@app.route('/<string:file>')
def default():
    return send_file("index.html", mimetype='text/html')

@app.route('/loadinfo', methods=['POST'])
def loadinfo():
    idx = request.form["idx"]
    print (idx)
    return profile.getLineInfo(int(idx))

@app.route('/loadfile', methods=['POST'])
def loadfile():
    fpath = request.form["file"]

    with open(fpath, 'r') as file:
        lines = list(map(removeSpecialCharacters, file.readlines()))
        mark_lines(lines, profile.selectEntry(fpath))
    return "".join(lines)

@app.route('/js/<path:path>')
def sendjs(path):
    return send_from_directory('js', path)

@app.route('/libs/<path:path>')
def sendlibs(path):
    return send_from_directory('libs', path)

@app.route('/css/<path:path>')
def sendcss(path):
    return send_from_directory('css', path)

if __name__ == "__main__":
    app.logger.disabled = True
    openPort = pickAnUnusedPort()
    threading.Timer(1, lambda: webbrowser.open("http://localhost:" + str(openPort))).start()
    app.run(port=openPort, debug=False)
