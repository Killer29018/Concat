#include "VirtualMachine.hpp"

#include <cstdio>
#include <cassert>

std::vector<OpCode> VM::m_OpCodes;
std::stack<Value> VM::m_Stack;

void VM::addOpCode(OpCodeEnum code)
{
    OpCode op;
    op.code = code;
    op.value = { TYPE_NULL, 0 };
    m_OpCodes.emplace_back(op);
}

void VM::addCode(const OpCode& code)
{
    m_OpCodes.push_back(code);
}

void VM::pushInt(int32_t value)
{
    OpCode op;
    op.code = OP_PUSH_INT;
    op.value = { TYPE_INT, value };
    m_OpCodes.emplace_back(op);
}

void VM::printOpCodes()
{
    printf("--- OPCODES ---\n");
    for (size_t i = 0; i < m_OpCodes.size(); i++)
    {
        switch (m_OpCodes[i].code)
        {
        case OP_PUSH_INT: 
            printValueDebug(i); break;
        default:
            printf("%.4lu | %.30s\n", i, OpCodeString[m_OpCodes[i].code]);
            break;
        }
    }
}

void VM::simulate()
{
    size_t ip = 0;
    
    while (ip < m_OpCodes.size())
    {
        OpCode& op = m_OpCodes[ip];
        switch (op.code)
        {
        case OP_ADD:
            {
                assert(!m_Stack.empty());

                const Value b = pop();
                const Value a = pop();
                operation(a, b, op);
                ip++;
                break;
            }
        case OP_SUBTRACT:
            {
                assert(!m_Stack.empty());

                const Value b = pop();
                const Value a = pop();
                operation(a, b, op);
                ip++;
                break;
            }
        case OP_MULTIPLY:
            {
                assert(!m_Stack.empty());

                const Value b = pop();
                const Value a = pop();
                operation(a, b, op);
                ip++;
                break;
            }
        case OP_DIVIDE:
            {
                assert(!m_Stack.empty());

                const Value b = pop();
                const Value a = pop();
                operation(a, b, op);
                ip++;
                break;
            }
        case OP_CR:
            {
                printf("\n");
                ip++;
                break;
            }
        case OP_PUSH_INT:
            {
                m_Stack.push(op.value);
                ip++;
                break;
            }
        case OP_PRINT:
            {
                assert(!m_Stack.empty());

                const Value a = pop();
                printf("%d", a.as.v_Int);
                ip++;
                break;
            }
        case OP_DUP:
            {
                assert(!m_Stack.empty());

                const Value a = pop();
                m_Stack.push(a);
                m_Stack.push(a);
                ip++;
                break;
            }
        case OP_DOT:
            {
                assert(!m_Stack.empty());

                const Value a = pop();
                printf("%c", a.as.v_Int);
                ip++;
                break;
            }
        case OP_SWAP:
            {
                assert(m_Stack.size() >= 2);

                const Value a = pop();
                const Value b = pop();
                m_Stack.push(a);
                m_Stack.push(b);
                ip++;

                break;
            }
        case OP_OVER:
            {
                assert(m_Stack.size() >= 2);

                const Value a = pop();
                const Value b = pop();
                m_Stack.push(b);
                m_Stack.push(a);
                m_Stack.push(b);
                ip++;

                break;
            }
        case OP_ROT:
            {
                assert(m_Stack.size() >= 3);

                const Value a = pop();
                const Value b = pop();
                const Value c = pop();

                m_Stack.push(b);
                m_Stack.push(a);
                m_Stack.push(c);
                ip++;

                break;
            }
        case OP_MOD:
            {
                assert(m_Stack.size() >= 2);
                const Value b = pop();
                const Value a = pop();

                operation(a, b, op);
                ip++;
                break;
            }
        default:
            assert(false); // UNREACHABLE
        }
    }
}

void VM::printValueDebug(size_t index)
{
    OpCode& code = m_OpCodes[index];
    switch (code.value.type)
    {
    case TYPE_INT:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], code.value.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void VM::operation(const Value& a, const Value& b, const OpCode& code)
{
    assert(a.type == b.type); // TYPES ARE EQUAL

    Value v;
    v.type = a.type;

    switch (code.code)
    {
    case OP_ADD:
        addValue(a, b, v);
        break;
    case OP_SUBTRACT:
        subtractValue(a, b, v);
        break;
    case OP_MULTIPLY:
        multiplyValue(a, b, v);
        break;
    case OP_DIVIDE:
        divideValue(a, b, v);
        break;
    case OP_MOD:
        modValue(a, b, v);
        break;
    default:
        assert(false); // UNREACHABLE
    }

    m_Stack.push(v);
}
