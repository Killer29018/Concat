#ifndef LEXER_HPP
#define LEXER_HPP

#include "VM.hpp"

#include "Tokens.hpp"

#include <vector>
#include <unordered_set>
#include <filesystem>

class Lexer
{
private:
    static std::unordered_set<std::string> m_Macros;
    static std::unordered_set<std::string> m_Var;
    static std::string m_InputString;
    static std::filesystem::path m_CurrentPath;
    static size_t m_StringOffset;
    static bool m_Error;
public:
    static void lexFile(const char* filePath);
    static void lexString(const char* inputString);

    static void deallocate();
private:
    Lexer() {}
    ~Lexer() {}

    static void addFile(const char* filePath);

    static void parseString();
    static bool getTokenType(Token& token);
    static bool parseWord(Token& token, const char* word);
    static bool isDelimiter(char c);

    static bool checkComments(bool* start, bool* end, bool* comment, bool* multiLineComment, size_t index);
};

#endif
