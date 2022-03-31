#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include "Options.hpp"

#include "Program.hpp"

int main(int argc, char** argv)
{
    Options::getValues(argc, argv);

    Program::printDebugTokens = Options::printDebugTokens;
    Program::printDebugOpcodes = Options::printDebugOpcodes;
    Program::runBuild = Options::runBuild;

    Program::createProgram(Options::mode, Options::filePath.c_str());

    return 0;
}
