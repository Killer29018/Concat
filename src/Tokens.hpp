#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <vector>

#include <unordered_map>

enum TokenType
{
    TOKEN_ADD,
    TOKEN_SUBTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MOD,

    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS_EQUAL,

    TOKEN_INVERT,
    TOKEN_LNOT,
    TOKEN_LAND,
    TOKEN_LOR,
    TOKEN_RSHIFT,
    TOKEN_LSHIFT,

    TOKEN_VAR,
    TOKEN_ENDVAR,

    TOKEN_READ_MEMORY_32,
    TOKEN_WRITE_MEMORY_32,

    TOKEN_READ_MEMORY_8,
    TOKEN_WRITE_MEMORY_8,

    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_CR,
    TOKEN_PRINT,
    TOKEN_DOT,

    TOKEN_DUP,
    TOKEN_DROP,
    TOKEN_SWAP,
    TOKEN_OVER,
    TOKEN_ROT,

    TOKEN_MACRO,
    TOKEN_ENDMACRO,

    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSEIF,
    TOKEN_ELSE,
    TOKEN_ENDIF,

    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_ENDWHILE,

    TOKEN_COUNT
};

const std::vector<const char*> TokenString
{
    "TOKEN_ADD",
    "TOKEN_SUBTRACT",
    "TOKEN_MULTIPLY",
    "TOKEN_DIVIDE",
    "TOKEN_MOD",

    "TOKEN_EQUAL",
    "TOKEN_NOT_EQUAL",
    "TOKEN_GREATER",
    "TOKEN_LESS",
    "TOKEN_GREATER_EQUAL",
    "TOKEN_LESS_EQUAL",

    "TOKEN_INVERT",
    "TOKEN_LNOT",
    "TOKEN_LAND",
    "TOKEN_LOR",
    "TOKEN_RSHIFT",
    "TOKEN_LSHIFT",

    "TOKEN_VAR",
    "TOKEN_ENDVAR",

    "TOKEN_READ_MEMORY_32",
    "TOKEN_WRITE_MEMORY_32",

    "TOKEN_READ_MEMORY_8",
    "TOKEN_WRITE_MEMORY_8",

    "TOKEN_INT",
    "TOKEN_CHAR",
    "TOKEN_STRING",
    "TOKEN_TRUE",
    "TOKEN_FALSE",

    "TOKEN_CR",
    "TOKEN_PRINT",
    "TOKEN_DOT",

    "TOKEN_DUP",
    "TOKEN_DROP",
    "TOKEN_SWAP",
    "TOKEN_OVER",
    "TOKEN_ROT",

    "TOKEN_MACRO",
    "TOKEN_ENDMACRO",

    "TOKEN_IF",
    "TOKEN_THEN",
    "TOKEN_ELSEIF",
    "TOKEN_ELSE",
    "TOKEN_ENDIF",

    "TOKEN_WHILE",
    "TOKEN_DO",
    "TOKEN_ENDWHILE",
};

struct Token
{
    TokenType type;
    size_t line;
    size_t column;
    char* startIndex;
    char* endIndex;
};

#endif
