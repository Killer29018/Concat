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
    OP_CR,
    OP_PUSH_INT,
    OP_PRINT,
    OP_DUP,
    OP_DOT,
    OP_SWAP,
    OP_OVER,
    OP_ROT,
    OP_MOD,

    OP_COUNT
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
    "OP_DUP",
    "OP_DOT",
    "OP_SWAP",
    "OP_OVER",
    "OP_ROT",
    "OP_MOD",
};

struct OpCode
{
    OpCodeEnum code;
    Value value;

    size_t line, column;
};

#endif
