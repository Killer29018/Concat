#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Tokens.hpp"

#include <vector>

class Compiler
{
private:
    static std::vector<Token>* m_Tokens;
public:
    static void addTokens(std::vector<Token>& tokens);
    static void startCompiler();
private:
    Compiler() {}
    ~Compiler() {}
};

#endif
