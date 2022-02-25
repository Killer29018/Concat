#include "Compiler.hpp"

#include <cstring>
#include <cstdlib>

#include "VirtualMachine.hpp"
#include "Error.hpp"

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
        case TOKEN_ADD: addBasicOpcode(code, ip, OP_ADD); break;
        case TOKEN_SUBTRACT: addBasicOpcode(code, ip, OP_SUBTRACT); break;
        case TOKEN_MULTIPLY: addBasicOpcode(code, ip, OP_MULTIPLY); break;
        case TOKEN_DIVIDE: addBasicOpcode(code, ip, OP_DIVIDE); break;
        case TOKEN_MOD: addBasicOpcode(code, ip, OP_MOD); break;

        case TOKEN_EQUAL: addBasicOpcode(code, ip, OP_EQUAL); break;
        case TOKEN_NOT_EQUAL: addBasicOpcode(code, ip, OP_NOT_EQUAL); break;
        case TOKEN_GREATER: addBasicOpcode(code, ip, OP_GREATER); break;
        case TOKEN_LESS: addBasicOpcode(code, ip, OP_LESS); break;
        case TOKEN_GREATER_EQUAL: addBasicOpcode(code, ip, OP_GREATER_EQUAL); break;
        case TOKEN_LESS_EQUAL: addBasicOpcode(code, ip, OP_LESS_EQUAL); break;

        case TOKEN_INVERT: addBasicOpcode(code, ip, OP_INVERT); break;
        case TOKEN_LAND: addBasicOpcode(code, ip, OP_LAND); break;
        case TOKEN_LOR: addBasicOpcode(code, ip, OP_LOR); break;
        case TOKEN_LNOT: addBasicOpcode(code, ip, OP_LNOT); break;

        case TOKEN_INT:
            {
                code.code = OP_PUSH_INT;
                int32_t value = atoi(word);
                code.value = { TYPE_INT, value };

                VM::addCode(code);
                ip++;
                break;
            }
        case TOKEN_TRUE:
            {
                code.code = OP_TRUE;
                code.value = { TYPE_BOOL, 1 };

                VM::addCode(code);
                ip++;
                break;
            }
        case TOKEN_FALSE:
            {
                code.code = OP_FALSE;
                code.value = { TYPE_BOOL, 0 };

                VM::addCode(code);
                ip++;
                break;
            }

        case TOKEN_CR: addBasicOpcode(code, ip, OP_CR); break;
        case TOKEN_PRINT: addBasicOpcode(code, ip, OP_PRINT); break;
        case TOKEN_DOT: addBasicOpcode(code, ip, OP_DOT); break;

        case TOKEN_DUP: addBasicOpcode(code, ip, OP_DUP); break;
        case TOKEN_DROP: addBasicOpcode(code, ip, OP_DROP); break;
        case TOKEN_SWAP: addBasicOpcode(code, ip, OP_SWAP); break;
        case TOKEN_OVER: addBasicOpcode(code, ip, OP_OVER); break;
        case TOKEN_ROT: addBasicOpcode(code, ip, OP_ROT); break;

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

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            Error::compilerError(t, "Macro unexpectedely ended");
                        }

                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_END_MACRO)
                        {
                            ipOffset++;
                            break;
                        }
                        else
                        {
                            macroTokens.push_back(m_Tokens->at(ip + ipOffset));
                        }

                    }

                    m_Macros.insert({ word, macroTokens });

                    ip += ipOffset;
                }

                break;
            }
        case TOKEN_IF: addBasicOpcode(code, ip, OP_IF); break;
        case TOKEN_THEN: // TODO Check for corresponding end
           code.code = OP_THEN;
           code.value = { TYPE_IP_OFFSET, 0 };
           VM::addCode(code);
           ip++;
           break;
        case TOKEN_ENDIF: addBasicOpcode(code, ip, OP_ENDIF); break;

        default:
            assert(false); // UNREACHABLE
        }

        delete[] word;
    }
}

void Compiler::addBasicOpcode(OpCode& code, size_t& ip, OpCodeEnum opcode)
{
    code.code = opcode;
    VM::addCode(code);
    ip++;
}
