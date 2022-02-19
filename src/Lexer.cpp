#include "Lexer.hpp"

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

std::vector<Token> Lexer::m_Tokens;

void Lexer::lexFile(const char* filePath)
{
    assert(false); // NOT IMPLEMENTED
}

void Lexer::lexString(char* inputString)
{
    parseString(inputString);
}

void Lexer::printTokens()
{
    for (size_t i = 0; i < m_Tokens.size(); i++)
    {
        Token& t = m_Tokens[i];

        int length = t.endIndex - t.startIndex;
        size_t line = t.line;
        printf("%.4lu | %.4lu:%.4lu | %-20s | %.*s\n", i, t.line, t.column, TokenString[t.type], length, t.startIndex);
    }
}

void Lexer::parseString(char* inputString)
{
    Token t;
    int currentLine = 1;
    t.startIndex = inputString;

    bool start = false;

    for (size_t i = 0; i < strlen(inputString); i++)
    {
        if (inputString[i] == '\n')
        {
            currentLine++;
            continue;
        }

        if (inputString[i] != ' ' && !start)
        {
            t.startIndex = inputString + i;
            t.column = t.startIndex - inputString;
            start = true;
            continue;
        }

        if (start && inputString[i] == ' ')
        {
            t.endIndex = (inputString + i);
            t.line = currentLine;
            start = false;
            getTokenType(inputString, t);

            m_Tokens.push_back(t);
            continue;
        }
    }

    if (start)
    {
        t.endIndex = inputString + strlen(inputString);
        t.line = currentLine;
        getTokenType(inputString, t);
        m_Tokens.push_back(t);
    }
}

void Lexer::getTokenType(char* inputString, Token& token)
{
    int length = token.endIndex - token.startIndex;

    if (length == 1)
    {
        switch (*token.startIndex)
        {
        case '+': token.type = TOKEN_ADD; break;
        case '-': token.type = TOKEN_SUBTRACT; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        }
    }
    else
    {
        if (!strncmp(token.startIndex, "CR", length))
        {
            token.type = TOKEN_CR;
        }
        else if (!strncmp(token.startIndex, "print", length))
        {
            token.type = TOKEN_PRINT;
        }
        else if (*token.startIndex >= '0' && *token.startIndex <= '9')
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
}
