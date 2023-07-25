import sys, json, os

def getArgs(args):
    return (args[0], f'({args[1]},{args[2]},{args[3]},{args[4]},{args[5]},{args[6]},)')

def openFile(filename):
    gotos = {}
    gotosExists = os.path.exists(filename)
    if (gotosExists):
        with open(filename) as json_file:
            gotos =  json.load(json_file)
            json_file.close()
    return gotos


def writeInFile(filename, gotos, args):
    jumpname, coordonnees = getArgs(args)
    if (jumpname == "j"):
        i = 1
        tmp = "j"+str(i)
        while tmp in gotos.keys():
            i+=1
            tmp = "j"+str(i)
        jumpname = tmp
    gotos[jumpname] = coordonnees
    print(f'Save position of jump : {jumpname} ({args[1]}, {args[2]}, {args[3]})')
    with open(filename, 'w') as json_file:
        x = json.dumps(gotos, indent=4)
        x += "\n"
        json_file.write(x)
        json_file.close()


def main(args):
    if len(args) == 8:
        mapname = args[0].split('.')[0]
        saveFile = f"gotos/{mapname}.json"
        gotos = openFile(saveFile)
        writeInFile(saveFile, gotos, args[1:])
    else:
        print("NC")

if __name__ == "__main__":
    args = sys.argv[1:]
    main(args)
