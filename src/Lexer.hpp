#ifndef LEXER_HPP
#define LEXER_HPP

#include "VirtualMachine.hpp"

#include "Tokens.hpp"

#include <vector>

class Lexer
{
private:
    static std::vector<Token> m_Tokens;
public:
    static void lexFile(const char* filePath);
    static void lexString(char* inputString);

    static void printTokens();
private:
    Lexer() {}
    ~Lexer() {}

    static void parseString(char* inputString);
    static void getTokenType(char* inputString, Token& token);
};

#endif
