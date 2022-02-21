#include "Lexer.hpp"

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <fstream>

#include "Compiler.hpp"

std::vector<Token> Lexer::m_Tokens;
char* Lexer::m_InputString;

void Lexer::lexFile(const char* filePath)
{
    std::ifstream file;
    file.open(filePath, std::ios::out);

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();

    m_InputString = new char[fileSize];

    file.seekg(0, std::ios::beg);
    file.read(m_InputString, fileSize);

    file.close();

    parseString();
}

void Lexer::lexString(const char* inputString)
{
    m_InputString = new char[strlen(inputString)];
    strcpy(m_InputString, inputString);
    parseString();
}

void Lexer::printTokens()
{
    printf("--- TOKENS ---\n");
    for (size_t i = 0; i < m_Tokens.size(); i++)
    {
        Token& t = m_Tokens[i];

        int length = t.endIndex - t.startIndex;
        size_t line = t.line;
        printf("%.4lu | %.4lu:%.4lu | %-20s | %.*s\n", i, t.line, t.column, TokenString[t.type], length, t.startIndex);
    }
}

void Lexer::deallocate()
{
    delete[] m_InputString;
}

void Lexer::parseString()
{
    Token t;
    int currentLine = 1;
    int currentColumn = 1;
    t.startIndex = m_InputString;

    bool start = false;

    for (size_t i = 0; i < strlen(m_InputString); i++)
    {
        if (!isDelimiter(m_InputString[i]) && !start)
        {
            t.startIndex = m_InputString + i;
            t.column = currentColumn;
            start = true;
        }
        else if (start && isDelimiter(m_InputString[i]))
        {
            t.endIndex = (m_InputString + i);
            t.line = currentLine;
            start = false;
            getTokenType(t);

            m_Tokens.push_back(t);
        }

        if (m_InputString[i] == '\n')
        {
            currentLine++;
            currentColumn = 0;
        }

        currentColumn++;
    }

    if (start)
    {
        t.endIndex = m_InputString + strlen(m_InputString);
        t.line = currentLine;
        getTokenType(t);
        m_Tokens.push_back(t);
    }

    Compiler::addTokens(m_Tokens);
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
    else
    {
        parseWord(token);
    }


    delete[] word;
}

void Lexer::parseWord(Token& token)
{
    int length = token.endIndex - token.startIndex;
    if (*token.startIndex >= '0' && *token.startIndex <= '9')
    {
        char* end;
        strtol(token.startIndex, &end, 0);
        if (end != token.endIndex)
        {
            fprintf(stderr, "%lu:%lu Failed to pass integer constant %.*s\n", token.line, token.column, length, token.startIndex);
            exit(-1);
        }
        else
        {
            token.type = TOKEN_INT;
        }
    }
    else
    {
        fprintf(stderr, "%lu:%lu Unknown word %.*s\n", token.line, token.column, length, token.startIndex);
        exit(-1);
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
