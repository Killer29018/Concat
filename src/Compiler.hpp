#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "OpCodes.hpp"
#include "Tokens.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

class Compiler
{
private:
    static std::vector<Token>* m_Tokens;
    static std::unordered_map<std::string, std::vector<Token>> m_Macros;
    static std::set<std::string> m_Variables;
    static bool m_Error;
public:
    static void addTokens(std::vector<Token>& tokens);
    static void startCompiler();
private:
    Compiler() {}
    ~Compiler() {}

    static void addBasicOpcode(OpCode& code, size_t& ip, OpCodeEnum opcode);

    static char parseEscapeCharacter(const char* word, size_t length);
};

#endif
