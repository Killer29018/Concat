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
    for (size_t i = 0; i < m_Tokens->size(); i++)
    {
        Token& t = m_Tokens->at(i);

        code.value = { TYPE_NULL, 0 };
        code.line = t.line;
        code.column = t.column;

        switch (t.type)
        {
        case TOKEN_ADD:
            code.code = OP_ADD;
            VM::addCode(code);
            break;
        case TOKEN_SUBTRACT:
            code.code = OP_SUBTRACT;
            VM::addCode(code);
            break;
        case TOKEN_MULTIPLY:
            code.code = OP_MULTIPLY;
            VM::addCode(code);
            break;
        case TOKEN_DIVIDE:
            code.code = OP_DIVIDE;
            VM::addCode(code);
            break;
        case TOKEN_CR:
            code.code = OP_CR;
            VM::addCode(code);
            break;
        case TOKEN_INT:
            {
            code.code = OP_PUSH_INT;
            char* temp = new char[t.endIndex - t.startIndex];
            strncpy(temp, t.startIndex, t.endIndex - t.startIndex);
            int32_t value = atoi(temp);
            code.value = { TYPE_INT, value };
            delete[] temp;

            VM::addCode(code);
            break;
            }
        case TOKEN_PRINT:
            code.code = OP_PRINT;
            VM::addCode(code);
            break;
        case TOKEN_DUP:
            code.code = OP_DUP;
            VM::addCode(code);
            break;
        case TOKEN_DOT:
            code.code = OP_DOT;
            VM::addCode(code);
            break;
        case TOKEN_SWAP:
            code.code = OP_SWAP;
            VM::addCode(code);
            break;
        case TOKEN_OVER:
            code.code = OP_OVER;
            VM::addCode(code);
            break;
        case TOKEN_ROT:
            code.code = OP_ROT;
            VM::addCode(code);
            break;
        case TOKEN_MOD:
            code.code = OP_MOD;
            VM::addCode(code);
            break;
        case TOKEN_MACRO:
            break;
        default:
            assert(false); // UNREACHABLE
        }
    }
}
