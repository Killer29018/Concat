#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Tokeniser -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include "VirtualMachine.hpp"

int main()
{
    VM::pushInt(34);
    VM::pushInt(35);
    VM::pushInt(31);
    VM::addOpCode(OP_ADD);
    VM::addOpCode(OP_ADD);
    VM::addOpCode(OP_PRINT);

    VM::printOpCodes();

    VM::simulate();
}
