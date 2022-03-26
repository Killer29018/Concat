import subprocess
import os
import sys

def buildFile(file):
    process = subprocess.Popen(['./Concat', 'build', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout, stderr

def getFileOutput(file):
    process = subprocess.Popen(['./Concat', 'run', file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout, stderr

def getOutput(file, stdout, stderr, bStdout, bStderr):
    output = ""
    output += ("-" * 50) + "\n"
    output += file + ".CONCAT\n"

    output += "--stdout--\n"
    output += stdout.decode("utf-8") + "\n"
    output += "--stderr--\n"
    output += stderr.decode("utf-8") + "\n"

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
    files = [os.path.splitext(f)[0] for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and os.path.splitext(os.path.join(path, f))[1] == ".CONCAT"]
    totalCount = len(files)
    for f in files:
        count += 1
        path = os.path.join(path, "")
        testFileName = path + f + "_TEST.txt"
        programFileName = path + f + ".CONCAT"
        builtFileName = programFileName + "_BIN"

        print(f"Checking {f}")

        print(f"    Creating {builtFileName}")
        buildFile(programFileName)

        print(f"    Running {programFileName}")
        stdout, stderr = getFileOutput(programFileName)
        print(f"    Running {builtFileName}")
        bStdout, bStderr = getFileOutput(builtFileName)

        output = getOutput(f, stdout, stderr, bStdout, bStderr)

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
             os.path.splitext(os.path.join(path, f))[1] == ".CONCAT" and
             os.path.exists(os.path.join(path, "") + os.path.splitext(f)[0] + "_TEST.txt")]

    failed = []
    totalCount = len(files)
    count = 0
    passedCount = 0
    totalCount = len(files)
    for f in files:
        count += 1
        path = os.path.join(path, "")
        testFileName = path + f + "_TEST.txt"
        programFileName = path + f + ".CONCAT"
        builtFileName = programFileName + "_BIN"

        buildFile(programFileName)

        stdout, stderr = getFileOutput(programFileName)
        bStdout, bStderr = getFileOutput(builtFileName)

        output = getOutput(f, stdout, stderr, bStdout, bStderr)

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
