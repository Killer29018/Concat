#include "Program.hpp"

void Program::startProgramFromFile(const char* filename)
{
    assert(TokenString.size()  == TOKEN_COUNT);
    assert(OpCodeString.size() == OP_COUNT);

    Lexer::lexFile(filename);
    Compiler::startCompiler();
    Lexer::deallocate();
    VM::simulate();
}
