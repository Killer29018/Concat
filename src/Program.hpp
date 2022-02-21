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
public:
    // static void startProgram(const char* )
    static void startProgramFromFile(const char* filename);
private:
    Program() {}
    ~Program() {}
};

#endif
