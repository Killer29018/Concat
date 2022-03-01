#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include "Tokens.hpp"

const int KeywordCount = 40;
const std::unordered_map<std::string, TokenType> Keywords({
    { "+",          TOKEN_ADD },
    { "-",          TOKEN_SUBTRACT },
    { "*",          TOKEN_MULTIPLY },
    { "/",          TOKEN_DIVIDE },
    { "mod",        TOKEN_MOD },

    { "==",         TOKEN_EQUAL },
    { "!=",         TOKEN_NOT_EQUAL },
    { ">",          TOKEN_GREATER },
    { "<",          TOKEN_LESS },
    { ">=",         TOKEN_GREATER_EQUAL },
    { "<=",         TOKEN_LESS_EQUAL },

    // And or and not provide same usage as there bitwise counterpart
    { "and",        TOKEN_LAND }, 
    { "or",         TOKEN_LOR },
    { "invert",     TOKEN_INVERT },
    { "land",       TOKEN_LAND },
    { "lor",        TOKEN_LOR },
    { "lnot",       TOKEN_LNOT },

    { "var",        TOKEN_VAR },

    { "!",          TOKEN_READ_MEMORY_32 },
    { "@",          TOKEN_WRITE_MEMORY_32 },

    { "true",       TOKEN_TRUE },
    { "false",      TOKEN_FALSE },

    { "cr",         TOKEN_CR },
    { "print",      TOKEN_PRINT },
    { ".",          TOKEN_DOT },

    { "dup",        TOKEN_DUP },
    { "drop",       TOKEN_DROP },
    { "swap",       TOKEN_SWAP },
    { "over",       TOKEN_OVER },
    { "rot",        TOKEN_ROT },

    { "macro",      TOKEN_MACRO },
    { "endmacro",   TOKEN_ENDMACRO },

    { "if",         TOKEN_IF },
    { "then",       TOKEN_THEN },
    { "elseif",     TOKEN_ELSEIF },
    { "else",       TOKEN_ELSE },
    { "endif",      TOKEN_ENDIF },

    { "while",      TOKEN_WHILE },
    { "do",         TOKEN_DO },
    { "endwhile",   TOKEN_ENDWHILE },
});


#endif
