#ifndef KEYWORDS_HPP
#define KEYWORDS_HPP

#include "Tokens.hpp"

const int KeywordCount = 12;
const std::unordered_map<std::string, TokenType> Keywords({
    { "+",      TOKEN_ADD },
    { "-",      TOKEN_SUBTRACT },
    { "*",      TOKEN_MULTIPLY },
    { "/",      TOKEN_DIVIDE },
    { "cr",     TOKEN_CR },
    { "print",  TOKEN_PRINT },
    { "dup",    TOKEN_DUP },
    { ".",      TOKEN_DOT },
    { "swap",   TOKEN_SWAP },
    { "over",   TOKEN_OVER },
    { "rot",    TOKEN_ROT },
    { "mod",    TOKEN_MOD },
});


#endif
