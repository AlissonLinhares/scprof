import os
import json

class Profile(object):
    def __init__(self, path):
        self.curr = []
        self.files = {}

        with open(path, 'r') as file:
            self.__dict__ = json.load(file)

            # Fixing visualization for absolute path.
            dir = os.path.dirname(path)
            if dir:
                for file in self.files:
                    file["path"] = dir + "/" + file["path"]

    def selectEntry(self, fpath):
        self.curr = []
        for file in self.files:
            if file["path"] == fpath:
                pEntry = {}
                wl, iwl = 0, 0
                for entry in file["entry"]:
                    if pEntry == {}:
                        pEntry = entry
                        wl, iwl = entry["wl"], entry["iwl"]
                    elif pEntry["line"] == entry["line"] and wl == entry["wl"] and iwl == entry["iwl"]:
                        # Testing if all declaration has the same datatype.
                        self.mergeArrayData(pEntry, entry)
                    else:
                        self.curr.append(pEntry)
                        pEntry = entry
                        wl, iwl = entry["wl"], entry["iwl"]
                self.curr.append(pEntry)
                return self.curr
        return {}

    def mergeArrayData(self, tgt, src):
        tb, tc = tgt["bitmap"], tgt["changes"]
        sb, sc = src["bitmap"], src["changes"]
        for i in range(0, len(tb)):
            tb[i] = sb[i] | tb[i]
            tc[i] = sc[i] + tc[i]

    def getLineInfo(self, line):
        return self.curr[line]
