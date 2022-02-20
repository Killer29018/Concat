#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include "Lexer.hpp"
#include "Compiler.hpp"
#include "VirtualMachine.hpp"

int main()
{
    // // ADD
    // VM::pushInt(34);
    // VM::pushInt(35);
    // VM::pushInt(31);
    // VM::addOpCode(OP_ADD);
    // VM::addOpCode(OP_ADD);
    // VM::addOpCode(OP_PRINT);
    // VM::addOpCode(OP_CR);

    // // SUBTRACT
    // VM::pushInt(69);
    // VM::pushInt(31);
    // VM::addOpCode(OP_SUBTRACT);
    // VM::addOpCode(OP_PRINT);
    // VM::addOpCode(OP_CR);

    // // MULTIPLY
    // VM::pushInt(5);
    // VM::pushInt(5);
    // VM::addOpCode(OP_MULTIPLY);
    // VM::addOpCode(OP_PRINT);
    // VM::addOpCode(OP_CR);

    // // DIVIDE
    // VM::pushInt(125);
    // VM::pushInt(5);
    // VM::addOpCode(OP_DIVIDE);
    // VM::addOpCode(OP_PRINT);
    // VM::addOpCode(OP_CR);

    // VM::printOpCodes();

    // Lexer::lexString("34 35 + print CR 34 31 - * / print  ");
    Lexer::lexFile("examples/basic.SBIMCL");
    Lexer::printTokens();

    Compiler::startCompiler();

    VM::printOpCodes();

    VM::simulate();
    Lexer::deallocate();
}
