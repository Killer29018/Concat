#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "Lexer.hpp"
#include "Compiler.hpp"
#include "VM.hpp"

enum class RunMode
{
    RUN,
    BUILD
};

class Program
{
public:
    static bool printDebugTokens;
    static bool printDebugOpcodes;
    static bool runBuild;
public:
    // static void startProgram(const char* )
    static void createProgram(RunMode mode, const char* filename);
    static void runProgramFromFile(const char* filename, bool runningBuild = false);
    static void buildProgramFromFile(const char* filename);
private:
    Program() {}
    ~Program() {}

    static void createOpcodes(const char* filename);
};

#endif
