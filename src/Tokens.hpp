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
    TOKEN_LAND,
    TOKEN_LOR,
    TOKEN_LNOT,

    TOKEN_INT,
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
    TOKEN_END_MACRO,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ENDIF,

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
    "TOKEN_LAND",
    "TOKEN_LOR",
    "TOKEN_LNOT",

    "TOKEN_INT",
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
    "TOKEN_END_MACRO",
    "TOKEN_IF",
    "TOKEN_THEN",
    "TOKEN_ENDIF",
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
