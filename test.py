import subprocess
import os
import sys

programName = "./concat"
programExt = ".concat"
binExt = "_bin"
inputExt = "_INPUT.txt"
testExt = "_TEST.txt"

def checkForInput(file, process):
    inputs = ""
    if os.path.exists(file):
        with open(file, "r") as test:
            for line in test:
                inputs += line
    return inputs

def buildFile(file):
    process = subprocess.Popen([programName, 'build', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout, stderr

def buildRunFile(file, inputFile):
    process = subprocess.Popen([programName, 'build', '-r', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    inputs = checkForInput(inputFile, process)
    stdout, stderr = process.communicate(input=inputs.encode())
    return stdout, stderr

def runFile(file, inputFile):
    process = subprocess.Popen([programName, 'run', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    inputs = checkForInput(inputFile, process)
    stdout, stderr = process.communicate(input=inputs.encode())
    return stdout, stderr

def getOutput(file, stdout, stderr, bStdout, bStderr, brStdout, brStderr):
    output = ""
    output += ("-" * 50) + "\n"
    output += file + programExt + "\n"

    output += "--stdout--\n"
    output += stdout.decode("utf-8") + "\n"
    output += "--stderr--\n"
    output += stderr.decode("utf-8") + "\n"

    output += "--build run stdout--\n"
    output += brStdout.decode("utf-8") + "\n"
    output += "--build run stderr--\n"
    output += brStderr.decode("utf-8") + "\n"

    output += "--build stdout--\n"
    output += bStdout.decode("utf-8") + "\n"
    output += "--build stderr--\n"
    output += bStderr.decode("utf-8") + "\n"


    output += ("-" * 50) + "\n"
    output += "\n"
    return output

def build(path):
    if not os.path.isdir(path):
        print("Invalid path")
        exit(-1)

    count = 0;
    files = [os.path.splitext(f)[0] for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and os.path.splitext(os.path.join(path, f))[1] == programExt]
    totalCount = len(files)
    for f in files:
        count += 1
        path = os.path.join(path, "")
        testFileName = path + f + testExt
        inputFileName = path + f + inputExt
        programFileName = path + f + programExt
        builtFileName = programFileName + binExt

        print(f"Checking {f}")

        print(f"    Creating {builtFileName}")
        buildFile(programFileName)

        print(f"    Running {programFileName}")
        stdout, stderr = runFile(programFileName, inputFileName)
        print(f"    Running Compiled {builtFileName}")
        bStdout, bStderr = runFile(builtFileName, inputFileName)
        print(f"    Building + Running {programFileName}")
        brStdout, brStderr = buildRunFile(programFileName, inputFileName)

        output = getOutput(f, stdout, stderr, bStdout, bStderr, brStdout, brStderr)

        with open(testFileName, "w") as test:
            test.write(output)

            print(f"Built test for {programFileName} ({count} / {totalCount})")
            print("")

    print("Created {} Test{}".format(count, "" if count == 1 else "s"))

def run(path):
    if not os.path.isdir(path):
        print("Invalid path")
        exit(-1)

    files = [os.path.splitext(f)[0] 
             for f in os.listdir(path) 
             if os.path.isfile(os.path.join(path, f)) and 
             os.path.splitext(os.path.join(path, f))[1] == programExt and
             os.path.exists(os.path.join(path, "") + os.path.splitext(f)[0] + testExt)]

    failed = []
    totalCount = len(files)
    count = 0
    passedCount = 0
    totalCount = len(files)
    for f in files:
        count += 1
        path = os.path.join(path, "")
        testFileName = path + f + testExt
        inputFileName = path + f + inputExt
        programFileName = path + f + programExt
        builtFileName = programFileName + binExt

        buildFile(programFileName)

        stdout, stderr = runFile(programFileName, inputFileName)
        bStdout, bStderr = runFile(builtFileName, inputFileName)
        brStdout, brStderr = buildRunFile(programFileName, inputFileName)

        output = getOutput(f, stdout, stderr, bStdout, bStderr, brStdout, brStderr)

        fileoutput = ""
        with open(testFileName, "r") as test:
            fileoutput = test.read()

        print(f"Checking {f}")
        if output == fileoutput:
            print(f"    PASSED ({count} / {totalCount})")
            passedCount += 1
        else:
            print(f"    FAILED ({count} / {totalCount})")
            failed.append([programFileName, output, fileoutput])

    print(f"{passedCount} out of {totalCount} succeeded")
    if passedCount != totalCount:
        print("Failed tests:")
        for f in failed:
            print(f"{'*' * 50}")
            print(f"{f[0]}")
            print(f"Output\n{f[1]}")
            print(f"Expected\n{f[2]}")
            print(f"{'*' * 50}\n")

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
