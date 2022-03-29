#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include "Program.hpp"

class Options
{
public:
    static bool printDebugTokens;
    static bool printDebugOpcodes;
    static bool runBuild;
    static RunMode mode;
    static std::string filePath;

public:
    static void getValues(int argc, char** argv);
    static void printHelp();
private:
    Options() {}

    static void parseSingle(char* input);
    static void parseWord(char* input);
};

#endif
