#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include "Tokens.hpp"

const int KeywordCount = 23;
const std::unordered_map<std::string, TokenType> Keywords({
    { "+",          TOKEN_ADD },
    { "-",          TOKEN_SUBTRACT },
    { "*",          TOKEN_MULTIPLY },
    { "/",          TOKEN_DIVIDE },
    { "mod",        TOKEN_MOD },

    { "==",         TOKEN_EQUAL},
    { "!=",         TOKEN_NOT_EQUAL},
    { ">",          TOKEN_GREATER},
    { "<",          TOKEN_LESS},
    { ">=",         TOKEN_GREATER_EQUAL},
    { "<=",         TOKEN_LESS_EQUAL},

    { "cr",         TOKEN_CR },
    { "print",      TOKEN_PRINT },
    { ".",          TOKEN_DOT },

    { "dup",        TOKEN_DUP },
    { "swap",       TOKEN_SWAP },
    { "over",       TOKEN_OVER },
    { "rot",        TOKEN_ROT },

    { "macro",      TOKEN_MACRO },
    { "endmacro",   TOKEN_END_MACRO },
    { "if",         TOKEN_IF },
    { "then",       TOKEN_THEN },
    { "endif",      TOKEN_ENDIF },
});


#endif
