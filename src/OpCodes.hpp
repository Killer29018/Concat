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
    OP_MOD,

    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,

    OP_INVERT,
    OP_LAND,
    OP_LOR,
    OP_LNOT,

    OP_PUSH_INT,

    OP_CR,
    OP_PRINT,
    OP_DOT,

    OP_DUP,
    OP_DROP,
    OP_SWAP,
    OP_OVER,
    OP_ROT,

    OP_IF,
    OP_THEN,
    OP_ENDIF,

    OP_COUNT
};

const std::vector<const char*> OpCodeString
{
    "OP_ADD",
    "OP_SUBTRACT",
    "OP_MULTIPLY",
    "OP_DIVIDE",
    "OP_MOD",

    "OP_EQUAL",
    "OP_NOT_EQUAL",
    "OP_GREATER",
    "OP_LESS",
    "OP_GREATER_EQUAL",
    "OP_LESS_EQUAL",

    "OP_INVERT",
    "OP_LAND",
    "OP_LOR",
    "OP_LNOT",

    "OP_PUSH_INT",

    "OP_CR",
    "OP_PRINT",
    "OP_DOT",

    "OP_DUP",
    "OP_DROP",
    "OP_SWAP",
    "OP_OVER",
    "OP_ROT",

    "OP_IF",
    "OP_THEN",
    "OP_ENDIF",
};

struct OpCode
{
    OpCodeEnum code;
    Value value;

    size_t line;
    size_t column;
};

#endif
