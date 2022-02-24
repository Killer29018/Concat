#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include "Tokens.hpp"

const int KeywordCount = 30;
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

    // And or and not provide same usage as there bitwise counterpart
    { "and",        TOKEN_LAND}, 
    { "or",         TOKEN_LOR},
    { "invert",     TOKEN_INVERT},
    { "land",       TOKEN_LAND},
    { "lor",        TOKEN_LOR},
    { "lnot",       TOKEN_LNOT},

    { "cr",         TOKEN_CR },
    { "print",      TOKEN_PRINT },
    { ".",          TOKEN_DOT },

    { "dup",        TOKEN_DUP },
    { "drop",       TOKEN_DROP },
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
