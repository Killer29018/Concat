#include "Options.hpp"

#include <iostream>
#include <filesystem>
#include <cstring>

bool Options::printDebugTokens = false;
bool Options::printDebugOpcodes = false;
bool Options::runBuild = false;
RunMode Options::mode = RunMode::RUN;
std::string Options::filePath = "";

void Options::getValues(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Both mode and filepath are required\n");
        exit(-1);
    }

    if (!strcmp(argv[1], "run"))
    {
        mode = RunMode::RUN;
    }
    else if (!strcmp(argv[1], "build"))
    {
        mode = RunMode::BUILD;
    }
    else
    {
        printf("Unrecognised Mode\n");
        exit(-1);
    }

    if (std::filesystem::is_regular_file(argv[argc - 1]))
    {
        filePath = argv[argc - 1];
    }
    else
    {
        printf("File not recognised");
        exit(-1);
    }

    for (int i = 2; i < argc - 1; i++)
    {
        char* currentString = argv[i];
        if (currentString[0] == '-' && currentString[1] == '-')
        {
            parseWord(currentString);
        }
        else if (currentString[0] == '-' && currentString[1] != '-')
        {
            parseSingle(currentString);
        }
        else
        {
            printf("Unrecognised flag %s\n", currentString);
            exit(-1);
        }
    }
}

void Options::printHelp()
{
    std::cout << "Usage: \n";
    std::cout << "  Concatenative - Imperitive Language\n";
    std::cout << "  Concat <MODE> [<OPTIONS>] <filepath>\n";
    std::cout << "  MODE\n";
    std::cout << "    run                   Run either a source file or Compiled File\n";
    std::cout << "    build                 Build a source file into a Compiled file\n";
    std::cout << "  OPTIONS\n";
    std::cout << "    -t, --print-tokens    Print Debug Tokens\n";
    std::cout << "    -o, --print-opcodes   Print Debug Opcodes\n";
    std::cout << "    -r, --run             When building run the program as well\n";
    std::cout << "    -h, --help            Print Help\n";

    exit(0);
}

void Options::parseSingle(char* input)
{
    for (size_t i = 1; i < strlen(input); i++)
    {
        switch (input[i])
        {
        case 't':
            printDebugTokens = true; break;
        case 'o':
            printDebugOpcodes = true; break;
        case 'r':
            runBuild = true; break;
        case 'h':
            printHelp(); break;

        default:
            printf("Unrecognised flag %s\n", input);
            exit(-1);
        }
    }
}

void Options::parseWord(char* input)
{
    if (!strcmp(input + 2, "print-tokens"))
    {
        printDebugTokens = true;
    }
    else if (!strcmp(input + 2, "print-opcodes"))
    {
        printDebugOpcodes = true;
    }
    else if (!strcmp(input + 2, "run"))
    {
        runBuild = true;
    }
    else if (!strcmp(input + 2, "help"))
    {
        printHelp();
    }
    else
    {
        printf("Unrecognised flag %s\n", input);
        exit(-1);
    }

}
