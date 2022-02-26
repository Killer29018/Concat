import subprocess
import os
import sys

def getFileOutput(file):
    process = subprocess.Popen(['./SBIMCL', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout, stderr

def build(path):
    if not os.path.isdir(path):
        print("Invalid path")
        exit(-1)

    files = [os.path.splitext(f)[0] for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and os.path.splitext(os.path.join(path, f))[1] == ".SBIMCL"]
    for f in files:
        path = os.path.join(path, "")
        testFileName = path + f + "_TEST.txt"
        programFileName = path + f + ".SBIMCL"

        stdout,stderr = getFileOutput(programFileName)

        output = ""
        output += ("-" * 50) + "\n"
        output += f + ".SBIMCL\n"
        output += "--stdout--\n"
        output += str(stdout) + "\n"
        output += "--stderr--\n"
        output += str(stderr) + "\n"
        output += ("-" * 50) + "\n"
        output += "\n"

        with open(testFileName, "w") as test:
            test.write(output)

            print(f"Built test for {programFileName}")

def run(path):
    if not os.path.isdir(path):
        print("Invalid path")
        exit(-1)

    files = [os.path.splitext(f)[0] for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and os.path.splitext(os.path.join(path, f))[1] == ".SBIMCL"]
    failed = []
    totalCount = len(files)
    passedCount = 0
    for f in files:
        path = os.path.join(path, "")
        testFileName = path + f + "_TEST.txt"
        programFileName = path + f + ".SBIMCL"

        stdout,stderr = getFileOutput(programFileName)

        output = ""
        output += ("-" * 50) + "\n"
        output += f + ".SBIMCL\n"
        output += "--stdout--\n"
        output += str(stdout) + "\n"
        output += "--stderr--\n"
        output += str(stderr) + "\n"
        output += ("-" * 50) + "\n"
        output += "\n"

        fileoutput = ""
        with open(testFileName, "r") as test:
            fileoutput = test.read()

        if output == fileoutput:
            print(f"FILE: {programFileName} Passed")
            passedCount += 1
        else:
            print(f"FILE: {programFileName} Failed")
            failed.append(programFileName)

    print(f"{passedCount} out of {totalCount} succeeded")
    if passedCount != totalCount:
        print("Failed tests:")
        for f in failed:
            print(f)

    # pass



def printUsage():
    print("Usage:")
    print("test.py <mode> <folderpath>")
    print("     <mode>")
    print("         h, help         print this menu")
    print("         run             Run all tests in <folderpath>")
    print("         build           Build tests for files in <folderpath>")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        printUsage()
        exit()

    mode = sys.argv[1]
    folderpath = sys.argv[2]
    if mode == "h" or mode == "help":
        printUsage()
    elif mode == "run":
        run(folderpath)
    elif mode == "build":
        build(folderpath)
    else:
        printUsage()
