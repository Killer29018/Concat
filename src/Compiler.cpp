#include "Compiler.hpp"

#include <cstring>
#include <cstdlib>

#include "VM.hpp"
#include "Error.hpp"

#include "Lexer.hpp"

std::vector<Token> Compiler::m_Tokens;
std::set<std::string> Compiler::m_Memory;
std::unordered_map<std::string, uint32_t> Compiler::m_Functions;
std::unordered_map<std::string, uint32_t> Compiler::m_GlobalVariables;
std::unordered_map<std::string, uint32_t> Compiler::m_GlobalConstants;
bool Compiler::m_Error = false;
size_t Compiler::m_Ip = 0;

bool Compiler::m_InFunction = false;
size_t Compiler::m_CurrentFunctionIndex = 0;

std::vector<std::pair<std::string, std::string>> Compiler::m_Patterns =
{
    { "\\\\", "\\"},    /* \\ '\' */
    { "\\a",  "\a"},    /* \a '\a' */
    { "\\b",  "\b"},    /* \b '\b' */
    { "\\t",  "\t"},    /* \t '\t' */
    { "\\n",  "\n"},    /* \n '\n' */
    { "\\v",  "\v"},    /* \v '\v' */
    { "\\f",  "\f"},    /* \f '\f' */
    { "\\r",  "\r"},    /* \r '\r' */
};

void Compiler::addToken(Token& token)
{
    m_Tokens.insert(m_Tokens.begin() + m_Ip, token);
    m_Ip++;
}

void Compiler::printTokens()
{
    printf("--- TOKENS ---\n");
    for (size_t i = 0; i < m_Tokens.size(); i++)
    {
        Token& t = m_Tokens[i];

        int length = t.endIndex - t.startIndex;
        printf("%.4lu | '%-10s':%.4lu:%.4lu | %-20s | %.*s\n", i, Lexer::filenames[t.sourceIndex].c_str(), t.line, t.column, TokenString[t.type], length, t.startIndex);
    }
}

void Compiler::startCompiler()
{
    if (m_Tokens.size() == 0)
        return;

    OpCode code;
    size_t ip = 0;
    while (ip < m_Tokens.size())
    {
        Token& t = m_Tokens.at(ip);

        size_t length = t.endIndex - t.startIndex;
        char* word = new char[length + 1];
        strncpy(word, t.startIndex, length);
        word[length] = 0x00;

        std::string stringWord(word);

        // code.value = { TYPE_NULL, 0 };
        code.value = nullptr;
        code.line = t.line;
        code.column = t.column;
        code.sourceIndex = t.sourceIndex;

        switch (t.type)
        {
            case TOKEN_INT:
                {
                    code.code = OP_INT;
                    int32_t value = atoi(word);
                    code.value = makeSmartPointer<vInt>(value);

                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_CHAR:
                {
                    code.code = OP_CHAR;
                    char c = *(word + 1);
                    if (length == 4) c = parseEscapeCharacter(word);

                    code.value = makeSmartPointer<vChar>(c);

                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_STRING:
                {
                    code.code = OP_STRING;

                    std::string parsed = parseEscapeSequence(word);
                    char* c = (char*)malloc(sizeof(char) * (parsed.size()));
                    memset(c, 0, parsed.size() - 1);
                    strncpy(c, parsed.c_str(), parsed.size() - 1);
                    c[parsed.size() - 1] = 0x00;

                    code.value = makeSmartPointer<vString>(c);

                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_BOOL:
                {
                    code.code = OP_BOOL;
                    if (stringWord == "true")
                        code.value = makeSmartPointer<vBool>(true);
                    else
                        code.value = makeSmartPointer<vBool>(false);

                    VM::addOpCode(code);
                    ip++;
                    break;
                }

            case TOKEN_CAST:
                {
                    code.code = OP_CAST;
                    if (stringWord == "cast(int)")
                        code.value = makeSmartPointer<vInt>(0);
                    else if (stringWord == "cast(bool)")
                        code.value = makeSmartPointer<vBool>(0);
                    else if (stringWord == "cast(char)")
                        code.value = makeSmartPointer<vChar>(0);
                    else if (stringWord == "cast(string)")
                        code.value = makeSmartPointer<vString>(nullptr);

                    VM::addOpCode(code);
                    ip++;
                    break;
                }

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
            case TOKEN_RSHIFT: 
                addBasicOpcode(code, ip, OP_RSHIFT); break;
            case TOKEN_LSHIFT: 
                addBasicOpcode(code, ip, OP_LSHIFT); break;

            case TOKEN_READ_MEMORY_32:
                addBasicOpcode(code, ip, OP_READ_MEMORY_32); break;
            case TOKEN_WRITE_MEMORY_32:
                addBasicOpcode(code, ip, OP_WRITE_MEMORY_32); break;
            case TOKEN_ADD_WRITE_MEMORY_32:
                addBasicOpcode(code, ip, OP_ADD_WRITE_MEMORY_32); break;
            case TOKEN_SUBTRACT_WRITE_MEMORY_32:
                addBasicOpcode(code, ip, OP_SUBTRACT_WRITE_MEMORY_32); break;
            case TOKEN_MULTIPLY_WRITE_MEMORY_32:
                addBasicOpcode(code, ip, OP_MULTIPLY_WRITE_MEMORY_32); break;
            case TOKEN_DIVIDE_WRITE_MEMORY_32:
                addBasicOpcode(code, ip, OP_DIVIDE_WRITE_MEMORY_32); break;

            case TOKEN_READ_MEMORY_8:
                addBasicOpcode(code, ip, OP_READ_MEMORY_8); break;
            case TOKEN_WRITE_MEMORY_8:
                addBasicOpcode(code, ip, OP_WRITE_MEMORY_8); break;
            case TOKEN_ADD_WRITE_MEMORY_8:
                addBasicOpcode(code, ip, OP_ADD_WRITE_MEMORY_8); break;
            case TOKEN_SUBTRACT_WRITE_MEMORY_8:
                addBasicOpcode(code, ip, OP_SUBTRACT_WRITE_MEMORY_8); break;
            case TOKEN_MULTIPLY_WRITE_MEMORY_8:
                addBasicOpcode(code, ip, OP_MULTIPLY_WRITE_MEMORY_8); break;
            case TOKEN_DIVIDE_WRITE_MEMORY_8:
                addBasicOpcode(code, ip, OP_DIVIDE_WRITE_MEMORY_8); break;

            case TOKEN_CR: 
                addBasicOpcode(code, ip, OP_CR); break;
            case TOKEN_PRINT: 
                addBasicOpcode(code, ip, OP_PRINT); break;
            case TOKEN_DOT: 
                addBasicOpcode(code, ip, OP_DOT); break;

            case TOKEN_INPUT: 
                addBasicOpcode(code, ip, OP_INPUT); break;

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

            case TOKEN_MEM:
                {
                    if (m_Memory.find(word) != m_Memory.end())
                    {
                        size_t index = std::distance(m_Memory.begin(), m_Memory.find(word));
                        // addBasicOpCode(code, ip, OP_LOAD_VAR);
                        code.code = OP_LOAD_MEM;
                        code.value = makeSmartPointer<vMemPtr>(index);
                        VM::addOpCode(code);
                        ip++;
                    }
                    else
                    {
                        if (m_InFunction)
                        {
                            Error::compilerError(t, "Cannot define mememory in Func");
                        }

                        if (ip == m_Tokens.size() - 1)
                        {
                            Error::compilerError(t, "Unexpected end when declaring memory");
                            m_Error = true;
                            ip++;
                            break;
                        }

                        size_t ipOffset = 0;
                        while ((ip + ipOffset) < m_Tokens.size())
                        {
                            ipOffset++;

                            if ((ip + ipOffset) == m_Tokens.size())
                            {
                                Error::compilerError(t, "mem %s has no corresponding endmem", word);
                                m_Error = true;
                                break;
                            }

                            if (m_Tokens.at(ip + ipOffset).type == TOKEN_ENDMEM)
                            {
                                m_Memory.emplace(word);
                                size_t index = std::distance(m_Memory.begin(), m_Memory.find(word));
                                // addBasicOpCode(code, ip, OP_LOAD_VAR);
                                code.code = OP_CREATE_MEM;
                                code.value = makeSmartPointer<vMemPtr>(index);
                                VM::addOpCode(code);
                                break;
                            }
                        }

                        ip++;
                    }
                    break;
                }
            case TOKEN_ENDMEM:
                {
                    if (ip == 0)
                    {
                        Error::compilerError(t, "could not find var before endmem");
                        m_Error = true;
                        ip++;
                        break;
                    }

                    size_t ipOffset = 0;
                    while (true)
                    {
                        ipOffset++;

                        if ((ip - ipOffset) == 0 && m_Tokens.at(ip - ipOffset).type != TOKEN_MEM)
                        {
                            Error::compilerError(t, "endmem has no corresponding mem");
                            m_Error = true;
                            break;
                        }

                        if (m_Tokens.at(ip - ipOffset).type == TOKEN_MEM)
                        {
                            break;
                        }
                        else if (m_Tokens.at(ip - ipOffset).type == TOKEN_ENDMEM)
                        {
                            Error::compilerError(t, "unexpected endmem before mem");
                            m_Error = true;
                            break;
                        }
                    }

                    ip++;
                    code.code = OP_ENDMEM;
                    VM::addOpCode(code);
                    break;
                }

            case TOKEN_CONST:
                {
                    if (m_InFunction)
                    {
                        Error::compilerError(t, "Cannot define constants in Func");
                    }

                    if (ip == m_Tokens.size() - 1)
                    {
                        Error::compilerError(t, "Expected Type but found EOF instead");
                        m_Error = true;
                    }

                    Token* token = &m_Tokens.at(ip + 1);

                    if (!isValue(token->type))
                    {
                        Error::compilerError(t, "Expected type but found %s instead", word);
                        exit(-1);
                    }
                    else
                    {
                        int index = VM::addGlobalConstant();
                        m_GlobalConstants[stringWord] = index;
                        code.code = OP_CREATE_CONST;
                        code.value = makeSmartPointer<vConst>(index);
                        VM::addOpCode(code);
                    }

                    ip++;
                    break;
                }
            case TOKEN_CALL_CONST:
                {
                    if (m_GlobalConstants.find(stringWord) != m_GlobalConstants.end())
                    {
                        uint32_t index = m_GlobalConstants[stringWord];
                        code.code = OP_CONST;
                        code.value = makeSmartPointer<vConst>(index);
                        VM::addOpCode(code);
                    }

                    ip++;
                    break;
                }

            case TOKEN_CREATE_VAR:
                {
                    if (m_InFunction)
                    {
                        Error::compilerError(t, "Cannot create variable in Func");
                        // Local variable
                    }

                    if (ip == m_Tokens.size() - 1)
                    {
                        Error::compilerError(t, "Expected type but found EOF instead");
                        m_Error = true;
                    }

                    Token* nextToken = &m_Tokens.at(ip + 1);

                    if (!isValueType(nextToken->type))
                    {
                        Error::compilerError(t, "Unexpected token, Expected TYPE but found %s instead", TokenString[nextToken->type]);
                        m_Error = true;

                        ip++;
                        break;
                    }

                    ValueType type = convertTokenToValue(nextToken->type);

                    Value* v = createValue(type);
                    uint32_t position = VM::addGlobalVariable(SmartPointer(v));

                    m_GlobalVariables[stringWord] = position;

                    ip += 2;
                    break;
                }
            case TOKEN_VAR:
                {
                    auto position = m_GlobalVariables.find(stringWord);
                    if (position == m_GlobalVariables.end())
                    {
                        Error::compilerError(t, "Could not find variable %s", word);
                        m_Error = true;

                        ip++;
                        break;
                    }

                    code.code = OP_VAR;
                    code.value = makeSmartPointer<vVar>(position->second);
                    VM::addOpCode(code);

                    ip++;
                    break;
                }

            case TOKEN_IF: 
                addBasicOpcode(code, ip, OP_IF); break;
            case TOKEN_THEN:
                {
                    size_t ipOffset = 0;
                    uint32_t ifCount = 0;

                    // INFO: Check for correspoding if or elseif
                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        if (m_Tokens.at(ip - ipOffset).type == TOKEN_ENDIF)
                        {
                            ifCount++;
                        }
                        else if (m_Tokens.at(ip - ipOffset).type == TOKEN_IF ||
                            m_Tokens.at(ip - ipOffset).type == TOKEN_ELSEIF)
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

                    while ((ip + ipOffset) < m_Tokens.size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens.size())
                        {
                            Error::compilerError(t, "then has no corresponding endif");
                            m_Error = true;
                        }

                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_IF)
                        {
                            ifCount++;
                        }
                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_ENDIF ||
                            m_Tokens.at(ip + ipOffset).type == TOKEN_ELSEIF)
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
                    // code.value = { TYPE_IP_OFFSET, 0 };
                    code.value = makeSmartPointer<vIpOffset>(0);
                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_ELSEIF: 
               {
                    size_t ipOffset = 0;
                    uint32_t ifCount = 0;

                    // INFO: Check for correspoding if
                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        if (m_Tokens.at(ip - ipOffset).type == TOKEN_ENDIF)
                        {
                            ifCount++;
                        }
                        else if (m_Tokens.at(ip - ipOffset).type == TOKEN_IF)
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
                    while ((ip + ipOffset) < m_Tokens.size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens.size())
                        {
                            Error::compilerError(t, "elseif has no corresponding then");
                            m_Error = true;
                        }

                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_IF ||
                            m_Tokens.at(ip + ipOffset).type == TOKEN_ELSEIF)
                        {
                            ifCount++;
                        }
                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_THEN)
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
                   // code.value = { TYPE_IP_OFFSET, 0 };
                   code.value = makeSmartPointer<vIpOffset>(0);
                   VM::addOpCode(code);
                   ip++;
                   break;
               }
            case TOKEN_ELSE:
               {
                   size_t ipOffset = 0;
                   uint32_t ifCount = 0;

                   // INFO: Check for correspoding if
                   while ((ip - ipOffset) > 0)
                   {
                       ipOffset++;

                       if (m_Tokens.at(ip - ipOffset).type == TOKEN_ENDIF)
                       {
                           ifCount++;
                       }
                       else if (m_Tokens.at(ip - ipOffset).type == TOKEN_IF)
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
                   // code.value = { TYPE_IP_OFFSET, 0 };
                   code.value = makeSmartPointer<vIpOffset>(0);
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
                    uint32_t whileCount = 0;

                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        // INFO: Check for correspoding while
                        if (m_Tokens.at(ip - ipOffset).type == TOKEN_DO || 
                            m_Tokens.at(ip - ipOffset).type == TOKEN_ENDWHILE)
                        {
                            whileCount++;
                        }
                        else if (m_Tokens.at(ip - ipOffset).type == TOKEN_WHILE)
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

                    while ((ip + ipOffset) < m_Tokens.size())
                    {
                        ipOffset++;

                        if ((ip + ipOffset) == m_Tokens.size())
                        {
                            Error::compilerError(t, "while has no corresponding endwhile");
                            m_Error = true;
                        }

                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_WHILE)
                        {
                            whileCount++;
                        }
                        if (m_Tokens.at(ip + ipOffset).type == TOKEN_ENDWHILE)
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
                    // code.value = { TYPE_IP_OFFSET, 0 };
                    code.value = makeSmartPointer<vIpOffset>(0);
                    VM::addOpCode(code);
                    ip++;
                    break;
                }
            case TOKEN_ENDWHILE:;
                {
                    size_t ipOffset = 0;
                    uint32_t endwhileCount = 0;

                    while (true)
                    {
                        ipOffset++;

                        if ((ip - ipOffset) == 0 && m_Tokens.at(0).type != TOKEN_WHILE)
                        {
                            Error::compilerError(t, "Could not find starting While");
                            m_Error = true;
                        }

                        if (m_Tokens.at(ip - ipOffset).type == TOKEN_ENDWHILE)
                        {
                            endwhileCount++;
                        }
                        else if (m_Tokens.at(ip - ipOffset).type == TOKEN_WHILE)
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
                    // code.value = { TYPE_IP_OFFSET, 0 };
                    code.value = makeSmartPointer<vIpOffset>(0);
                    VM::addOpCode(code);
                    ip++;
                    break;
                }

            case TOKEN_FUNC:
                {
                    if (m_InFunction)
                    {
                        Error::compilerError(t, "Can not define Func in another Func");
                        exit(-1);
                    }

                    bool inputs = true;
                    std::vector<ValueType> inputTypes;
                    std::vector<ValueType> outputTypes;

                    bool run = true;

                    int32_t ipOffset = 0;

                    while (run)
                    {
                        ipOffset++;

                        Token& token = m_Tokens.at(ip + ipOffset);

                        switch (token.type)
                        {
                        case TOKEN_DEFINE:
                            run = false;
                            break;

                        case (TOKEN_ENDFUNC):
                            Error::compilerError(t, "Unexpected endfunc, Expected define before");
                            exit(-1);
                        case TOKEN_FUNC:
                            Error::compilerError(t, "Func can not be defined within another func");
                            exit(-1);

                        case TOKEN_FUNC_SEPERATOR:
                            {
                                inputs = false;
                                break;
                            }

                        default:
                            {
                                if (isValueType(token.type))
                                {
                                    ValueType type = convertTokenToValue(token.type);

                                    if (inputs)
                                        inputTypes.emplace_back(type);
                                    else
                                        outputTypes.emplace_back(type);

                                    break;
                                }
                                Error::compilerError(t, "Unexpected token when defining func");
                                exit(-1);
                            }
                        }

                    }

                    code.code = OP_FUNC;
                    uint32_t index = VM::addFunction();
                    code.value = makeSmartPointer<vFunc>(inputTypes, outputTypes, index);
                    VM::addFunctionDefinition(code);
                    VM::addOpCode(code);

                    m_Functions[stringWord] = index;

                    m_InFunction = true;
                    m_CurrentFunctionIndex = index;

                    ip += ipOffset + 1;
                    break;
                }

            case TOKEN_ENDFUNC:
                {
                    if (!m_InFunction)
                    {
                        Error::compilerError(t, "Unexpected End Function");
                        exit(-1);
                    }

                    code.code = OP_ENDFUNC;
                    VM::addOpCode(code);
                    m_InFunction = false;
                    m_CurrentFunctionIndex = 0;

                    ip++;
                    break;
                }
            case TOKEN_CALLFUNC:
                {
                    code.code = OP_CALLFUNC;
                    code.value = makeSmartPointer<vFunc>(m_Functions[stringWord]);
                    VM::addOpCode(code);

                    ip++;
                    break;
                }

            case TOKEN_INCLUDE:
                {
                    ip++;
                    break;
                }

            default:
                {
                    if (isValueType(t.type))
                    {
                        Error::compilerError(t, "Unexpected type");
                        exit(-1);
                    }
                    else
                    {
                        assert(false && "Unreachable"); // UNREACHABLE
                    }
                }
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

std::string stringReplace(const std::string& s, const std::string& findS, const std::string replaceS)
{
    std::string result = s;

    auto pos = s.find(findS);
    if (pos == std::string::npos)
        return result;

    result.replace(pos, findS.length(), replaceS);
    return stringReplace(result, findS, replaceS);
}

std::string Compiler::parseEscapeSequence(const char* word)
{
    std::string result(word + 1);

    for (const auto& p : m_Patterns)
    {
        result = stringReplace(result, p.first, p.second);
    }

    return result;
}

char Compiler::parseEscapeCharacter(const char* word)
{
    std::string result(word + 1);

    for (const auto& p : m_Patterns)
    {
        result = stringReplace(result, p.first, p.second);
    }

    return result[0];
}

ValueType Compiler::convertTokenToValue(TokenType type)
{
    // INFO: Only works if sequences are the same within both Tokens and Opcodes
    return (ValueType)(TYPE_INT + (type - TOKEN_TYPE_INT));
}

bool Compiler::isValueType(TokenType type)
{
    switch (type)
    {
    case TOKEN_TYPE_INT:
    case TOKEN_TYPE_BOOL:
    case TOKEN_TYPE_CHAR:
    case TOKEN_TYPE_STRING:
    case TOKEN_TYPE_MEMPTR:
        return true;

    default:
        return false;
    }
}

bool Compiler::isValue(TokenType type)
{
    switch (type)
    {
    case TOKEN_INT:
    case TOKEN_BOOL:
    case TOKEN_CHAR:
    case TOKEN_STRING:
        return true;

    default:
        return false;
    }
}
