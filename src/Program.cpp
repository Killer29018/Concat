#include "Program.hpp"

void Program::startProgramFromFile(const char* filename)
{
    assert(TokenString.size()  == TOKEN_COUNT);
    assert(Keywords.size()     == KeywordCount);
    assert(OpCodeString.size() == OP_COUNT);

    Lexer::lexFile(filename);

#ifdef PRINT_DEBUG_TOKENS
    Lexer::printTokens();
#endif

    Compiler::startCompiler();


    Lexer::deallocate();

#ifdef PRINT_DEBUG_TOKENS
    VM::printOpCodes();
#endif

    VM::simulate();
}
