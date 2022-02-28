#include "Compiler.hpp"

#include <cstring>
#include <cstdlib>

#include "VirtualMachine.hpp"
#include "Error.hpp"

std::vector<Token>* Compiler::m_Tokens;
std::unordered_map<std::string, std::vector<Token>> Compiler::m_Macros;
std::unordered_map<std::string, VALUE_TYPE> Compiler::m_Variables;
bool Compiler::m_Error = false;

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
                addBasicOpcode(code, ip, OP_ADD); break;
            case TOKEN_SUBTRACT: 
                addBasicOpcode(code, ip, OP_SUBTRACT); break;
            case TOKEN_MULTIPLY: 
                addBasicOpcode(code, ip, OP_MULTIPLY); break;
            case TOKEN_DIVIDE: 
                addBasicOpcode(code, ip, OP_DIVIDE); break;
            case TOKEN_MOD: 
                addBasicOpcode(code, ip, OP_MOD); break;

            case TOKEN_EQUAL: 
                addBasicOpcode(code, ip, OP_EQUAL); break;
            case TOKEN_NOT_EQUAL: 
                addBasicOpcode(code, ip, OP_NOT_EQUAL); break;
            case TOKEN_GREATER: 
                addBasicOpcode(code, ip, OP_GREATER); break;
            case TOKEN_LESS: 
                addBasicOpcode(code, ip, OP_LESS); break;
            case TOKEN_GREATER_EQUAL: 
                addBasicOpcode(code, ip, OP_GREATER_EQUAL); break;
            case TOKEN_LESS_EQUAL: 
                addBasicOpcode(code, ip, OP_LESS_EQUAL); break;

            case TOKEN_INVERT: 
                addBasicOpcode(code, ip, OP_INVERT); break;
            case TOKEN_LAND: 
                addBasicOpcode(code, ip, OP_LAND); break;
            case TOKEN_LOR: 
                addBasicOpcode(code, ip, OP_LOR); break;
            case TOKEN_LNOT: 
                addBasicOpcode(code, ip, OP_LNOT); break;

            case TOKEN_VAR:
                {
                    std::string wordString = word;
                    if (m_Variables.find(wordString) != m_Variables.end())
                    {
                        code.code = OP_VAR;
                        VALUE_TYPE ptr = m_Variables.at(word);
                        code.value = { TYPE_MEM_PTR, ptr };
                        VM::addOpCode(code);
                        ip++;

                        break;
                    }
                    else
                    {
                        if (ip == m_Tokens->size() - 1)
                        {
                            Error::compilerError(t, "Unexpected end when declaring variable");
                            m_Error = true;
                            ip++;
                            break;
                        }

                        Token& next = m_Tokens->at(ip + 1);
                        if (next.type != TOKEN_INT)
                        {
                            Error::compilerError(next, "int was expected while creating variable '%s'", word);
                            m_Error = true;
                            ip++;
                            break;
                        }

                        size_t length = next.endIndex - next.startIndex;
                        char* nextWord = new char[length];
                        strncpy(nextWord, next.startIndex, length);

                        VALUE_TYPE value = atoi(nextWord);
                        delete[] nextWord;

                        VALUE_TYPE ptr = VM::addMemory(value);

                        code.code = OP_VAR;
                        code.value = { TYPE_MEM_PTR, ptr };

                        m_Variables[wordString] = ptr;
                        VM::addOpCode(code);

                        ip += 2;
                        break;
                    }
                }
            case TOKEN_READ_MEMORY:
                addBasicOpcode(code, ip, OP_READ_MEMORY); break;
            case TOKEN_WRITE_MEMORY:
                addBasicOpcode(code, ip, OP_WRITE_MEMORY); break;

            case TOKEN_INT:
                {
                    code.code = OP_PUSH_INT;
                    VALUE_TYPE value = atoi(word);
                    code.value = { TYPE_INT, value };

                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_TRUE:
                {
                    code.code = OP_TRUE;
                    code.value = { TYPE_BOOL, 1 };

                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_FALSE:
                {
                    code.code = OP_FALSE;
                    code.value = { TYPE_BOOL, 0 };

                    VM::addOpCode(code);
                    ip++;
                    break;
                }

            case TOKEN_CR: 
                addBasicOpcode(code, ip, OP_CR); break;
            case TOKEN_PRINT: 
                addBasicOpcode(code, ip, OP_PRINT); break;
            case TOKEN_DOT: 
                addBasicOpcode(code, ip, OP_DOT); break;

            case TOKEN_DUP: 
                addBasicOpcode(code, ip, OP_DUP); break;
            case TOKEN_DROP: 
                addBasicOpcode(code, ip, OP_DROP); break;
            case TOKEN_SWAP: 
                addBasicOpcode(code, ip, OP_SWAP); break;
            case TOKEN_OVER: 
                addBasicOpcode(code, ip, OP_OVER); break;
            case TOKEN_ROT: 
                addBasicOpcode(code, ip, OP_ROT); break;

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
                        break;
                    }
                    size_t ipOffset = 0;
                    std::vector<Token> macroTokens;

                    while ((ip + ipOffset) < m_Tokens->size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            Error::compilerError(t, "macro has no corresponding endmacro");
                            m_Error = true;
                        }

                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_ENDMACRO)
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
                    break;
                }

            case TOKEN_ENDMACRO: 
                addBasicOpcode(code, ip, OP_IF); break;

            case TOKEN_IF: 
                addBasicOpcode(code, ip, OP_IF); break;
            case TOKEN_THEN:
                {
                    size_t ipOffset = 0;
                    size_t ifCount = 0;

                    // INFO: Check for correspoding if or elseif
                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        if (m_Tokens->at(ip - ipOffset).type == TOKEN_THEN || 
                            m_Tokens->at(ip - ipOffset).type == TOKEN_ENDIF)
                        {
                            ifCount++;
                        }
                        else if (m_Tokens->at(ip - ipOffset).type == TOKEN_IF ||
                            m_Tokens->at(ip - ipOffset).type == TOKEN_ELSEIF)
                        {
                            if (ifCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                        if ((ip - ipOffset) == 0)
                        {
                            Error::compilerError(t, "then has no corresponding if");
                            m_Error = true;
                        }
                    }

                    ipOffset = 0;
                    ifCount = 0;

                    while ((ip + ipOffset) < m_Tokens->size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            Error::compilerError(t, "then has no corresponding endif");
                            m_Error = true;
                        }

                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_IF)
                        {
                            ifCount++;
                        }
                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_ENDIF ||
                            m_Tokens->at(ip + ipOffset).type == TOKEN_ELSEIF)
                        {
                            if (ifCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }

                    code.code = OP_THEN;
                    code.value = { TYPE_IP_OFFSET, 0 };
                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_ELSEIF: 
               {
                    size_t ipOffset = 0;
                    size_t ifCount = 0;

                    // INFO: Check for correspoding if
                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        if (m_Tokens->at(ip - ipOffset).type == TOKEN_ENDIF)
                        {
                            ifCount++;
                        }
                        else if (m_Tokens->at(ip - ipOffset).type == TOKEN_IF)
                        {
                            if (ifCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }

                        if ((ip - ipOffset) == 0)
                        {
                            Error::compilerError(t, "elseif has no corresponding if");
                            m_Error = true;
                        }
                    }

                    ipOffset = 0;
                    ifCount = 0;

                    // INFO: Check for corresponding then
                    while ((ip + ipOffset) < m_Tokens->size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            Error::compilerError(t, "elseif has no corresponding then");
                            m_Error = true;
                        }

                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_IF ||
                            m_Tokens->at(ip + ipOffset).type == TOKEN_ELSEIF)
                        {
                            ifCount++;
                        }
                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_THEN)
                        {
                            if (ifCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }


                   code.code = OP_ELSEIF;
                   code.value = { TYPE_IP_OFFSET, 0 };
                   VM::addOpCode(code);
                   ip++;
                   break;
               }
            case TOKEN_ELSE:
               {
                   size_t ipOffset = 0;
                   size_t ifCount = 0;

                   // INFO: Check for correspoding if
                   while ((ip - ipOffset) > 0)
                   {
                       ipOffset++;

                       if (m_Tokens->at(ip - ipOffset).type == TOKEN_ENDIF)
                       {
                           ifCount++;
                       }
                       else if (m_Tokens->at(ip - ipOffset).type == TOKEN_IF)
                       {
                           if (ifCount == 0)
                           {
                               break;
                           }
                           else
                           {
                               ifCount--;
                           }
                       }
                       if ((ip - ipOffset) == 0)
                       {
                           Error::compilerError(t, "else has no corresponding if");
                            m_Error = true;
                       }
                   }

                   code.code = OP_ELSE;
                   code.value = { TYPE_IP_OFFSET, 0 };
                   VM::addOpCode(code);
                   ip++;
                   break;
               }
            case TOKEN_ENDIF: 
               addBasicOpcode(code, ip, OP_ENDIF); break;

            case TOKEN_WHILE: 
               addBasicOpcode(code, ip, OP_WHILE); break;
            case TOKEN_DO:
                {
                    size_t ipOffset = 0;
                    size_t whileCount = 0;

                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        // INFO: Check for correspoding while
                        if (m_Tokens->at(ip - ipOffset).type == TOKEN_DO || 
                            m_Tokens->at(ip - ipOffset).type == TOKEN_ENDWHILE)
                        {
                            whileCount++;
                        }
                        else if (m_Tokens->at(ip - ipOffset).type == TOKEN_WHILE)
                        {
                            if (whileCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                whileCount--;
                            }
                        }
                        if ((ip - ipOffset) == 0)
                        {
                            Error::compilerError(t, "do has no corresponding while");
                            m_Error = true;
                        }
                    }

                    ipOffset = 0;
                    whileCount = 0;

                    while ((ip + ipOffset) < m_Tokens->size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens->size())
                        {
                            Error::compilerError(t, "while has no corresponding endwhile");
                            m_Error = true;
                        }

                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_WHILE)
                        {
                            whileCount++;
                        }
                        if (m_Tokens->at(ip + ipOffset).type == TOKEN_ENDWHILE)
                        {
                            if (whileCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                whileCount--;
                            }
                        }
                    }

                    code.code = OP_DO;
                    code.value = { TYPE_IP_OFFSET, 0 };
                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_ENDWHILE:;
                {
                    int32_t ipOffset = 0;
                    size_t endwhileCount = 0;

                    while (true)
                    {
                        ipOffset++;

                        if ((ip - ipOffset) == 0 && m_Tokens->at(0).type != TOKEN_WHILE)
                        {
                            Error::compilerError(t, "Could not find starting While");
                            m_Error = true;
                        }

                        if (m_Tokens->at(ip - ipOffset).type == TOKEN_ENDWHILE)
                        {
                            endwhileCount++;
                        }
                        else if (m_Tokens->at(ip - ipOffset).type == TOKEN_WHILE)
                        {
                            if (endwhileCount == 0)
                            {
                                break;
                            }
                            else
                            {
                                endwhileCount--;
                            }
                        }
                    }

                    code.code = OP_ENDWHILE;
                    code.value = { TYPE_IP_OFFSET, 0 };
                    VM::addOpCode(code);
                    ip++;
                    break;
                }

            default:
                assert(false && "Unreachable"); // UNREACHABLE
        }

        delete[] word;
    }

    if (m_Error)
        exit(-1);
}

void Compiler::addBasicOpcode(OpCode& code, size_t& ip, OpCodeEnum opcode)
{
    code.code = opcode;
    VM::addOpCode(code);
    ip++;
}
