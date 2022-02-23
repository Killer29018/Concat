#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Tokens.hpp"

#include <vector>

class Compiler
{
private:
    static std::vector<Token>* m_Tokens;
    static std::unordered_map<std::string, std::vector<Token>> m_Macros;
public:
    static void addTokens(std::vector<Token>& tokens);
    static void startCompiler();
private:
    Compiler() {}
    ~Compiler() {}

    static void addBasicOpcode(OpCode& code, size_t& ip, OpCodeEnum opcode);
};

#endif
