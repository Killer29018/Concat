#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include "Program.hpp"

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("Usage: SBIMCL <filename>");
        return -1;
    }
    else if (argc == 2)
    {
        Program::startProgramFromFile(argv[1]);
    }

    return 0;
}
