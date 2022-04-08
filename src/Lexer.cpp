#include "Lexer.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <fstream>
#include <sstream>

#include "Error.hpp"
#include "Compiler.hpp"
#include "Keywords.hpp"

std::vector<std::string> Lexer::filenames;

std::unordered_set<std::string> Lexer::m_Macros;
std::unordered_set<std::string> Lexer::m_Mem;
std::unordered_set<std::string> Lexer::m_Var;
std::unordered_set<std::string> Lexer::m_Func;
std::unordered_set<std::string> Lexer::m_IncludedFiles;
std::unordered_set<std::string> Lexer::m_Constants;
bool Lexer::m_Error = false;

size_t Lexer::m_InputStringLocation;
std::vector<std::string> Lexer::m_Sources;
std::string* Lexer::m_InputString;

std::filesystem::path Lexer::m_CurrentPath;
size_t Lexer::m_CurrentLine = 0;
size_t Lexer::m_CurrentColumn;

void Lexer::lexFile(const char* filePath)
{
    addFile(filePath);
    parseString();
}

void Lexer::deallocate()
{
}

void Lexer::addFile(const char* filePath)
{
    m_CurrentPath = std::filesystem::path(filePath);
    std::ifstream file;
    file.open(filePath, std::ios::out);

    std::stringstream ss;
    ss << file.rdbuf();

    filenames.emplace_back(filePath);
    m_Sources.emplace_back(ss.str());
    m_InputStringLocation = m_Sources.size() - 1;
    m_InputString = &m_Sources.at(m_Sources.size() - 1);

    file.close();
}

void Lexer::parseString()
{
    Token t;
    t.startIndex = &(*m_InputString)[0];

    m_CurrentLine = 1;
    m_CurrentColumn = 1;

    bool start = false;
    bool comments = false;
    bool multiLineComment = false;
    bool end = false;

    bool parseCharacter = false;
    bool parseString = false;
    size_t cp = 0;

    t.sourceIndex = m_InputStringLocation;

    while (cp != m_InputString->size())
    {
        bool skip = checkComments(&start, &end, &comments, &multiLineComment, cp);

        if ((*m_InputString)[cp] == '\'')
            parseCharacter = !parseCharacter;
        else if ((*m_InputString)[cp] == '"')
            parseString = !parseString;

        if ((!isDelimiter((*m_InputString)[cp]) || parseCharacter || parseString) && !start && !comments && !skip)
        {
            t.startIndex = &(*m_InputString)[0] + cp;
            t.column = m_CurrentColumn;
            start = true;
        }
        else if (!parseCharacter && !parseString && ((start && isDelimiter((*m_InputString)[cp])) || end))
        {
            t.endIndex = (&(*m_InputString)[0] + cp);
            t.line = m_CurrentLine;
            bool add = getTokenType(t);

            start = false;
            end = false;

            parseCharacter = false;
            parseString = false;

            if (add)
                Compiler::addToken(t);
        }

        if ((*m_InputString)[cp] == '\n' && !parseCharacter && !parseString)
        {
            m_CurrentLine++;
            m_CurrentColumn = 0;
        }

        m_CurrentColumn++;

        if (skip)
        {
            cp++;
            m_CurrentColumn++;
        }

        cp++;
    }

    if (start)
    {
        t.endIndex = &(*m_InputString)[0] + m_InputString->length();
        t.line = m_CurrentLine;
        getTokenType(t);
        Compiler::addToken(t);
    }

    if (m_Error)
        exit(-1);
}

bool Lexer::getTokenType(Token& token)
{
    bool add = true;
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
    else if (m_Mem.find(word) != m_Mem.end())
    {
        token.type = TOKEN_MEM;
    }
    else if (m_Var.find(word) != m_Var.end())
    {
        token.type = TOKEN_VAR;
    }
    else if (m_Func.find(word) != m_Func.end())
    {
        token.type = TOKEN_CALLFUNC;
    }
    else if (m_Constants.find(word) != m_Constants.end())
    {
        token.type = TOKEN_CALL_CONST;
    }
    else
    {
        add = parseWord(token, word);
    }

    delete[] word;

    return add;
}

bool Lexer::parseWord(Token& token, const char* word)
{
    bool add = true;
    int length = token.endIndex - token.startIndex;
    if (*token.startIndex >= '0' && *token.startIndex <= '9') // Int
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
    else if (*token.startIndex == '\'') // Char
    {
        if (length < 3 || length > 4)
        {
            Error::compilerError(token, "Illformed character literal %.*s", length, token.startIndex);
            m_Error = true;
        }

        if (length == 4)
        {
            if (*(token.startIndex + 1) != '\\')
            {
                Error::compilerError(token, "Illformed character literal %.*s", length, token.startIndex);
                m_Error = true;
            }
        }

        token.type = TOKEN_CHAR;
    }
    else if (*token.startIndex == '"')
    {
        if (*(token.endIndex - 1) == '"') // String
        {
            token.type = TOKEN_STRING;
        }
    }
    else if (!strcmp(word, "include")) // Include a file
    {
        add = false;
        Token* t = Compiler::getTopToken();

        if (t->type != TOKEN_STRING)
        {
            Error::compilerError(*t, "Expected a string for include");
        }

        int length = t->endIndex - t->startIndex - 2;

        char* includeWord = (char*)malloc(sizeof(char) * (length + 1));
        includeWord[length] = 0;
        strncpy(includeWord, t->startIndex + 1, length);

        std::filesystem::path parent = m_CurrentPath.parent_path().relative_path();
        std::filesystem::path newFile = parent.append(includeWord);

        if (!std::filesystem::exists(newFile))
        {
            Error::compilerError(token, "Failed to include \"%s\"\n", newFile.filename().generic_string().c_str());
            exit(-1);
        }

        if (m_IncludedFiles.find(newFile) == m_IncludedFiles.end())
        {
            std::filesystem::path currentPath = m_CurrentPath;
            size_t line = m_CurrentLine;
            size_t column = m_CurrentColumn;
            size_t currentString = m_InputStringLocation;

            Compiler::popBackToken();
            Lexer::lexFile(newFile.generic_string().c_str());

            m_CurrentPath = currentPath;
            m_CurrentLine = line;
            m_CurrentColumn = column;

            m_InputStringLocation = currentString;
            m_InputString = &m_Sources[m_InputStringLocation];

            m_IncludedFiles.emplace(newFile.generic_string());
        }
        else
        {
            Compiler::popBackToken();
        }

        delete[] includeWord;
    }
    else // Mem, Var, Macro, Func, Const
    {
        Token* top = Compiler::getTopToken();
        if (top->type == TOKEN_MACRO)
        {
            token.type = TOKEN_MACRO;
            m_Macros.emplace(word);
            Compiler::popBackToken();
        }
        else if (top->type == TOKEN_MEM)
        {
            token.type = TOKEN_MEM;
            m_Mem.emplace(word);
            Compiler::popBackToken();
        }
        else if (top->type == TOKEN_CREATE_VAR)
        {
            token.type = TOKEN_CREATE_VAR;
            m_Var.emplace(word);
            Compiler::popBackToken();
        }
        else if (top->type == TOKEN_FUNC)
        {
            token.type = TOKEN_FUNC;
            m_Func.emplace(word);
            Compiler::popBackToken();
        }
        else if (top->type == TOKEN_CONST)
        {
            token.type = TOKEN_CONST;
            m_Constants.emplace(word);
            Compiler::popBackToken();
        }
        else
        {
            Error::compilerError(token, "Unknown word %.*s", length, token.startIndex);
            m_Error = true;
        }
    }

    return add;
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
    if ((*m_InputString)[index] == '/' && index < m_InputString->size())
    {
        if ((*m_InputString)[index + 1] == '/')
        {
            (*comments) = true;
            if (*start)
                (*end) = true;

            skip = true;
        }
        else if ((*m_InputString)[index + 1] == '*')
        {
            (*comments) = true;
            (*multiLineComment) = true;

            if (*start)
                (*end) = true;

            skip = true;
        }
    }
    else if ((*m_InputString)[index] == '*' && index < m_InputString->size() && (*multiLineComment))
    {
        if ((*m_InputString)[index + 1] == '/')
        {
            (*comments) = false;
            (*multiLineComment) = false;
            skip = true;
        }
    }

    if ((*m_InputString)[index] == '\n')
    {
        if (!(*multiLineComment))
            *(comments) = false;
    }

    return skip;
}

