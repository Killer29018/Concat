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
            printValue(i); break;
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
                operation(op);
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
                Value& a = pop();
                printf("%d", a.as.v_Int);
                ip++;
                break;
            }
        default:
            assert(false); // UNREACHABLE
        }
    }
}

void VM::printValue(size_t index)
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

void VM::operation(const OpCode& code)
{
    Value& a = pop();
    Value& b = pop();
 
    assert(a.type == b.type); // TYPES ARE EQUAL

    Value v;
    v.type = a.type;

    switch (code.code)
    {
    case OP_ADD:
        addValue(a, b, v);
        break;
    default:
        assert(false); // UNREACHABLE
    }

    m_Stack.push(v);
}
