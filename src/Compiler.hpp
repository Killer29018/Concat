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
    static std::vector<Token> m_Tokens;
    static std::unordered_map<std::string, std::vector<Token>> m_Macros;
    static std::set<std::string> m_Memory;
    static std::unordered_map<std::string, uint32_t> m_Variables;
    static std::unordered_map<std::string, uint32_t> m_Functions;
    static bool m_Error;
    static size_t m_Ip;

    static std::vector<std::pair<std::string, std::string>> m_Patterns;
public:
    static void addToken(Token& token);
    static void printTokens();
    static void startCompiler();

    static Token* getTopToken() { return &m_Tokens.at(m_Tokens.size() - 1); }
    static void popBackToken() { m_Tokens.pop_back(); m_Ip--; }

private:
    Compiler() {}
    ~Compiler() {}

    static void addBasicOpcode(OpCode& code, size_t& ip, OpCodeEnum opcode);

    static char parseEscapeCharacter(const char* word);
    static std::string parseEscapeSequence(const char* word);

    static ValueType convertTokenToValue(TokenType type);
    static bool isTokenType(TokenType type);
};

#endif
