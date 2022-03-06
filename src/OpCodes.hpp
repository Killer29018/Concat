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
    OP_LNOT,
    OP_LAND,
    OP_LOR,
    OP_RSHIFT,
    OP_LSHIFT,

    OP_CREATE_VAR,
    OP_LOAD_VAR,
    OP_ENDVAR,

    OP_READ_MEMORY_32,
    OP_WRITE_MEMORY_32,

    OP_READ_MEMORY_8,
    OP_WRITE_MEMORY_8,

    OP_PUSH_INT,
    OP_PUSH_CHAR,
    OP_PUSH_STRING,
    OP_TRUE,
    OP_FALSE,

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
    OP_ELSEIF,
    OP_ELSE,
    OP_ENDIF,

    OP_WHILE,
    OP_DO,
    OP_ENDWHILE,

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
    "OP_LNOT",
    "OP_LAND",
    "OP_LOR",
    "OP_LSHIFT",
    "OP_RSHIFT",

    "OP_CREATE_VAR",
    "OP_LOAD_VAR",
    "OP_ENDVAR",

    "OP_READ_MEMORY_32",
    "OP_WRITE_MEMORY_32",

    "OP_READ_MEMORY_8",
    "OP_WRITE_MEMORY_8",

    "OP_PUSH_INT",
    "OP_PUSH_CHAR",
    "OP_PUSH_STRING",
    "OP_TRUE",
    "OP_FALSE",

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
    "OP_ELSE",
    "OP_ENDIF",

    "OP_WHILE",
    "OP_DO",
    "OP_ENDWHILE",
};

struct OpCode
{
    OpCodeEnum code;    // 2

    size_t line;        // 4
    size_t column;      // 4

    Value* value;
};

#endif
