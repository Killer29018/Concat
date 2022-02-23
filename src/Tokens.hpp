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
    TOKEN_CR,
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_DUP,
    TOKEN_DOT,
    TOKEN_SWAP,
    TOKEN_OVER,
    TOKEN_ROT,
    TOKEN_MOD,

    TOKEN_COUNT
};

const std::vector<const char*> TokenString
{
    "TOKEN_ADD",
    "TOKEN_SUBTRACT",
    "TOKEN_MULTIPLY",
    "TOKEN_DIVIDE",
    "TOKEN_CR",
    "TOKEN_INT",
    "TOKEN_PRINT",
    "TOKEN_DUP",
    "TOKEN_DOT",
    "TOKEN_SWAP",
    "TOKEN_OVER",
    "TOKEN_ROT",
    "TOKEN_MOD",
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
