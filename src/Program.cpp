#include "Program.hpp"

void Program::startProgramFromFile(const char* filename)
{
    Lexer::lexFile(filename);
    Compiler::startCompiler();
    Lexer::deallocate();
    VM::simulate();
}
