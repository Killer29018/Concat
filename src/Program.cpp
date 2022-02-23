#include "Program.hpp"

#include "Keywords.hpp"

bool Program::printDebugTokens =false;
bool Program::printDebugOpcodes = false;

void Program::startProgramFromFile(const char* filename)
{
    assert(TokenString.size()  == TOKEN_COUNT);
    assert(Keywords.size()     == KeywordCount);
    assert(OpCodeString.size() == OP_COUNT);

    Lexer::lexFile(filename);

    if (printDebugTokens)
        Lexer::printTokens();

    Compiler::startCompiler();


    Lexer::deallocate();

    if (printDebugOpcodes)
        VM::printOpCodes();

    VM::simulate();
}
