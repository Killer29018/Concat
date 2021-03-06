#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <vector>

#include "Values/Value.hpp"
#include "SmartPointer.hpp"

enum OpCodeEnum : uint16_t
{
    OP_INT,
    OP_BOOL,
    OP_CHAR,
    OP_STRING,

    OP_CAST,

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

    OP_READ_MEMORY_32,
    OP_WRITE_MEMORY_32,
    OP_ADD_WRITE_MEMORY_32,
    OP_SUBTRACT_WRITE_MEMORY_32,
    OP_MULTIPLY_WRITE_MEMORY_32,
    OP_DIVIDE_WRITE_MEMORY_32,

    OP_READ_MEMORY_8,
    OP_WRITE_MEMORY_8,
    OP_ADD_WRITE_MEMORY_8,
    OP_SUBTRACT_WRITE_MEMORY_8,
    OP_MULTIPLY_WRITE_MEMORY_8,
    OP_DIVIDE_WRITE_MEMORY_8,

    OP_CR,
    OP_PRINT,
    OP_DOT,

    OP_INPUT,

    OP_DUP,
    OP_DROP,
    OP_SWAP,
    OP_OVER,
    OP_ROT,

    OP_CREATE_MEM,
    OP_LOAD_MEM,
    OP_ENDMEM,

    OP_CREATE_CONST,
    OP_CONST,

    OP_VAR,

    OP_IF,
    OP_THEN,
    OP_ELSEIF,
    OP_ELSE,
    OP_ENDIF,

    OP_WHILE,
    OP_DO,
    OP_ENDWHILE,

    OP_FUNC,
    OP_CALLFUNC,
    OP_ENDFUNC,

    OP_COUNT
};

const std::vector<const char*> OpCodeString
{
    "OP_INT",
    "OP_BOOL",
    "OP_CHAR",
    "OP_STRING",

    "OP_CAST",

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

    "OP_READ_MEMORY_32",
    "OP_WRITE_MEMORY_32",
    "OP_ADD_WRITE_MEMORY_32",
    "OP_SUBTRACT_WRITE_MEMORY_32",
    "OP_MULTIPLY_WRITE_MEMORY_32",
    "OP_DIVIDE_WRITE_MEMORY_32",

    "OP_READ_MEMORY_8",
    "OP_WRITE_MEMORY_8",
    "OP_ADD_WRITE_MEMORY_8",
    "OP_SUBTRACT_WRITE_MEMORY_8",
    "OP_MULTIPLY_WRITE_MEMORY_8",
    "OP_DIVIDE_WRITE_MEMORY_8",

    "OP_CR",
    "OP_PRINT",
    "OP_DOT",
    
    "OP_INPUT",

    "OP_DUP",
    "OP_DROP",
    "OP_SWAP",
    "OP_OVER",
    "OP_ROT",

    "OP_CREATE_MEM",
    "OP_LOAD_MEM",
    "OP_ENDMEM",

    "OP_CREATE_CONST",
    "OP_CONST",

    "OP_VAR",

    "OP_IF",
    "OP_THEN",
    "OP_ENDIF",
    "OP_ELSE",
    "OP_ENDIF",

    "OP_WHILE",
    "OP_DO",
    "OP_ENDWHILE",

    "OP_FUNC",
    "OP_CALLFUNC",
    "OP_ENDFUNC",
};

struct OpCode
{
    OpCodeEnum code;    // 2

    size_t sourceIndex;
    size_t line;        // 4
    size_t column;      // 4

    // std::shared_ptr<Value> value;
    SmartPointer value;
};

#endif
