#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <vector>

#include "Value.hpp"

enum OpCodeEnum
{
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CR, // Carriage Return
    OP_PUSH_INT,
    OP_PRINT,
};

const std::vector<const char*> OpCodeString
{
    "OP_ADD",
    "OP_SUBTRACT",
    "OP_MULTIPLY",
    "OP_DIVIDE",
    "OP_CR",
    "OP_PUSH_INT",
    "OP_PRINT",
};

struct OpCode
{
    OpCodeEnum code;
    Value value;
};

#endif
