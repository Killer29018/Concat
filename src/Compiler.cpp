#include "Compiler.hpp"

#include "VirtualMachine.hpp"

#include <cstring>
#include <cstdlib>

std::vector<Token>* Compiler::m_Tokens;
std::unordered_map<std::string, std::vector<Token>> Compiler::m_Macros;

void Compiler::addTokens(std::vector<Token>& tokens)
{
    m_Tokens = &tokens;
}

void Compiler::startCompiler()
{
    if (m_Tokens->size() == 0)
        return;

    OpCode code;
    size_t ip = 0;
    while (ip < m_Tokens->size())
    {
        Token& t = m_Tokens->at(ip);

        size_t length = t.endIndex - t.startIndex;
        char* word = new char[length];
        strncpy(word, t.startIndex, length);

        code.value = { TYPE_NULL, 0 };
        code.line = t.line;
        code.column = t.column;

        switch (t.type)
        {
        case TOKEN_ADD:
            code.code = OP_ADD;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_SUBTRACT:
            code.code = OP_SUBTRACT;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_MULTIPLY:
            code.code = OP_MULTIPLY;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_DIVIDE:
            code.code = OP_DIVIDE;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_CR:
            code.code = OP_CR;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_INT:
            {
            code.code = OP_PUSH_INT;
            int32_t value = atoi(word);
            code.value = { TYPE_INT, value };

            VM::addCode(code);
            ip++;
            break;
            }
        case TOKEN_PRINT:
            code.code = OP_PRINT;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_DUP:
            code.code = OP_DUP;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_DOT:
            code.code = OP_DOT;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_SWAP:
            code.code = OP_SWAP;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_OVER:
            code.code = OP_OVER;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_ROT:
            code.code = OP_ROT;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_MOD:
            code.code = OP_MOD;
            VM::addCode(code);
            ip++;
            break;
        case TOKEN_MACRO:
            {
                std::string wordString = word;
                if (m_Macros.find(wordString) != m_Macros.end())
                {
                    std::vector<Token>& tokens = m_Macros.at(word);

                    for (int i = tokens.size() - 1; i >= 0; i--)
                    {
                        m_Tokens->insert(m_Tokens->begin() + ip + 1, tokens[i]);
                    }
                    ip++;
                }
                else
                {
                    size_t ipOffset = 0;
                    std::vector<Token> macroTokens;

                    while ((ip + ipOffset) < m_Tokens->size())
                    {
                        ipOffset++;
                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_END)
                        {
                            ipOffset++;
                            break;
                        }
                        else
                        {
                            macroTokens.push_back(m_Tokens->at(ip + ipOffset));
                        }

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            printf("[COMPILER ERROR] %ld:%ld Macro Unexpectedly ended\n", t.line, t.column);
                        }
                    }

                    m_Macros.insert({ word, macroTokens });

                    ip += ipOffset;
                }

                break;
            }
        case TOKEN_END:
            assert(false); // UNREACHABLE - Should Be consumed
        default:
            assert(false); // UNREACHABLE
        }

        delete[] word;
    }
}
