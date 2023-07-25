import sys, json, os
import re

def natural_sort(l): 
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [convert(c) for c in re.split('([0-9]+)', key)]
    return sorted(l, key=alphanum_key)


def openFile(filename):
    gotos = {}
    gotosExists = os.path.exists(filename)
    if (gotosExists):
        with open(filename) as json_file:
            gotos =  json.load(json_file)
            json_file.close()
    return gotos

def notValid(gotos : dict):
    if (len(gotos) != 0):
        splitIntoSmallerString([k for k in natural_sort(gotos)], 75)
    else:
        print("This map doesn't have goto locations yet.")

def splitIntoSmallerString(gotos,maxLength : int) :
    print("goto list : ")
    fleche = "      |---> "
    s = fleche
    for e in gotos:
        new_e = e + ", "
        new_s = s + new_e
        if (len(new_s) <= maxLength):
            s = new_s
        else:
            print(s)
            s = fleche+new_e
    print(s[0: len(s)-2])

def main(args):
    mapname = args[0].split('.')[0]
    saveFile = f"gotos/{mapname}.json"
    gotos = openFile(saveFile)
    if len(args) > 1:
        jumpname = str(args[1])
        if (jumpname in gotos.keys()):
            print(gotos[jumpname])
            return
    notValid(gotos)


if __name__ == "__main__":
    args = sys.argv[1:]
    main(args)