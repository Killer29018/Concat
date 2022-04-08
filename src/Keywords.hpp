#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include "Tokens.hpp"

const int KeywordCount = 69;
const std::unordered_map<std::string, TokenType> Keywords({
    { "true",       TOKEN_BOOL },
    { "false",      TOKEN_BOOL },

    { "int",        TOKEN_TYPE_INT, },
    { "bool",       TOKEN_TYPE_BOOL, },
    { "char",       TOKEN_TYPE_CHAR, },
    { "string",     TOKEN_TYPE_STRING, },
    { "memptr",     TOKEN_TYPE_MEMPTR, },

    { "cast(int)",  TOKEN_CAST },
    { "cast(bool)", TOKEN_CAST },
    { "cast(char)", TOKEN_CAST },
    { "cast(string)",TOKEN_CAST },

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
    { "invert",     TOKEN_INVERT },
    { "lnot",       TOKEN_LNOT },
    { "land",       TOKEN_LAND },
    { "and",        TOKEN_LAND }, 
    { "lor",        TOKEN_LOR },
    { "or",         TOKEN_LOR },
    { ">>",         TOKEN_RSHIFT},
    { "<<",         TOKEN_LSHIFT },

    { "mem",        TOKEN_MEM },
    { "endmem",     TOKEN_ENDMEM },

    { "const",      TOKEN_CONST },

    { "var",        TOKEN_CREATE_VAR },

    { "@",          TOKEN_READ_MEMORY_32 },
    { "!",          TOKEN_WRITE_MEMORY_32 },
    { "+!",         TOKEN_ADD_WRITE_MEMORY_32 },
    { "-!",         TOKEN_SUBTRACT_WRITE_MEMORY_32 },
    { "*!",         TOKEN_MULTIPLY_WRITE_MEMORY_32 },
    { "/!",         TOKEN_DIVIDE_WRITE_MEMORY_32 },

    { "@8",         TOKEN_READ_MEMORY_8 },
    { "!8",         TOKEN_WRITE_MEMORY_8 },
    { "+!8",        TOKEN_ADD_WRITE_MEMORY_8 },
    { "-!8",        TOKEN_SUBTRACT_WRITE_MEMORY_8 },
    { "*!8",        TOKEN_MULTIPLY_WRITE_MEMORY_8 },
    { "/!8",        TOKEN_DIVIDE_WRITE_MEMORY_8 },

    { "cr",         TOKEN_CR },
    { "print",      TOKEN_PRINT },
    { ".",          TOKEN_DOT },

    { "input",      TOKEN_INPUT },

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

    { "func",       TOKEN_FUNC },
    { "define",     TOKEN_DEFINE },
    { "-->",        TOKEN_FUNC_SEPERATOR },
    { "endfunc",    TOKEN_ENDFUNC },
});

#endif
