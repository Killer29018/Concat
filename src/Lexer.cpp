#include "Lexer.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <fstream>
#include <sstream>

#include "Error.hpp"
#include "Compiler.hpp"
#include "Keywords.hpp"

std::vector<Token> Lexer::m_Tokens;
std::unordered_set<std::string> Lexer::m_Macros;
std::unordered_set<std::string> Lexer::m_Var;
std::string Lexer::m_InputString = "";
bool Lexer::m_Error = false;

void Lexer::lexFile(const char* filePath)
{
    std::ifstream file;
    file.open(filePath, std::ios::out);

    std::stringstream ss;
    ss << file.rdbuf();
    m_InputString = ss.str();


    file.close();

    parseString();
}

void Lexer::lexString(const char* inputString)
{
    m_InputString = inputString;
    parseString();
}

void Lexer::printTokens()
{
    printf("--- TOKENS ---\n");
    for (size_t i = 0; i < m_Tokens.size(); i++)
    {
        Token& t = m_Tokens[i];

        int length = t.endIndex - t.startIndex;
        printf("%.4lu | %.4lu:%.4lu | %-20s | %.*s\n", i, t.line, t.column, TokenString[t.type], length, t.startIndex);
    }
}

void Lexer::deallocate()
{
}

void Lexer::parseString()
{
    Token t;
    int currentLine = 1;
    int currentColumn = 1;
    t.startIndex = &m_InputString[0];

    bool start = false;
    bool comments = false;
    bool multiLineComment = false;
    bool end = false;

    for (size_t i = 0; i < m_InputString.size();)
    {
        bool skip = checkComments(&start, &end, &comments, &multiLineComment, i);

        if (!isDelimiter(m_InputString[i]) && !start && !comments && !skip)
        {
            t.startIndex = &m_InputString[0] + i;
            t.column = currentColumn;
            start = true;
        }
        else if ((start && isDelimiter(m_InputString[i])) || end)
        {
            t.endIndex = (&m_InputString[0] + i);
            t.line = currentLine;
            start = false;
            end = false;
            getTokenType(t);

            m_Tokens.push_back(t);
        }

        if (m_InputString[i] == '\n')
        {
            currentLine++;
            currentColumn = 0;
        }

        currentColumn++;

        if (skip)
        {
            i++;
            currentColumn++;
        }

        i++;
    }

    if (start)
    {
        t.endIndex = &m_InputString[0] + m_InputString.length();
        t.line = currentLine;
        getTokenType(t);
        m_Tokens.push_back(t);
    }

    if (!m_Error)
        Compiler::addTokens(m_Tokens);
    else
        exit(-1);
}

void Lexer::getTokenType(Token& token)
{
    int length = token.endIndex - token.startIndex;
    char* word = new char[length + 1];
    strncpy(word, token.startIndex, length);
    word[length] = 0;

    if (Keywords.find(word) != Keywords.end())
    {
        token.type = Keywords.at(word);
    }
    else if (m_Macros.find(word) != m_Macros.end())
    {
        token.type = TOKEN_MACRO;
    }
    else
    {
        parseWord(token, word);
    }

    delete[] word;
}

void Lexer::parseWord(Token& token, const char* word)
{
    int length = token.endIndex - token.startIndex;
    if (*token.startIndex >= '0' && *token.startIndex <= '9')
    {
        char* end;
        strtol(token.startIndex, &end, 10);
        if (end != token.endIndex)
        {
            Error::compilerError(token, "Failed to pass integer constant %.*s", length, token.startIndex);
            m_Error = true;
        }
        else
        {
            token.type = TOKEN_INT;
        }
    }
    else
    {
        Token* top = &m_Tokens.at(m_Tokens.size() - 1);
        if (top->type == TOKEN_MACRO)
        {
            token.type = TOKEN_MACRO;
            m_Macros.emplace(word);
            m_Tokens.pop_back();
        }
        else if (top->type == TOKEN_VAR)
        {
            token.type = TOKEN_VAR;
            m_Var.emplace(word);
            m_Tokens.pop_back();
        }
        else
        {
            if (m_Var.find(word) != m_Var.end())
            {
                token.type = TOKEN_VAR;
            }
            else
            {
                Error::compilerError(token, "Unknown word %.*s", length, token.startIndex);
                m_Error = true;
            }
        }
    }
}

bool Lexer::isDelimiter(char c)
{
    switch (c)
    {
    case ' ':
    case '\n':
        return true;
    default:
        return false;
    }
}

bool Lexer::checkComments(bool* start, bool* end, bool* comments, bool* multiLineComment, size_t index)
{
    bool skip = false;
    if (m_InputString[index] == '/' && index < m_InputString.size())
    {
        if (m_InputString[index + 1] == '/')
        {
            (*comments) = true;
            if (*start)
                (*end) = true;

            skip = true;
        }
        else if (m_InputString[index + 1] == '*')
        {
            (*comments) = true;
            (*multiLineComment) = true;

            if (*start)
                (*end) = true;

            skip = true;
        }
    }
    else if (m_InputString[index] == '*' && index < m_InputString.size() && (*multiLineComment))
    {
        if (m_InputString[index + 1] == '/')
        {
            (*comments) = false;
            (*multiLineComment) = false;
            skip = true;
        }
    }

    if (m_InputString[index] == '\n')
    {
        if (!(*multiLineComment))
            *(comments) = false;
    }

    return skip;
}

