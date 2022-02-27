#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "Lexer.hpp"
#include "Compiler.hpp"
#include "VirtualMachine.hpp"

class Program
{
public:
    static bool printDebugTokens;
    static bool printDebugOpcodes;
    static bool run;
public:
    // static void startProgram(const char* )
    static void createProgram(bool runMode, const char* filename);
    static void runProgramFromFile(const char* filename);
    static void buildProgramFromFile(const char* filename);
private:
    Program() {}
    ~Program() {}

    static void createOpcodes(const char* filename);
};

#endif
