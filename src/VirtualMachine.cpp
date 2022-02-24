#include "VirtualMachine.hpp"

#include <cstdio>
#include <cstdlib>
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
        case OP_ADD: operation(op, ip); break;
        case OP_SUBTRACT: operation(op, ip); break;
        case OP_MULTIPLY: operation(op, ip); break;
        case OP_DIVIDE: operation(op, ip); break;
        case OP_MOD: operation(op, ip); break;

        case OP_EQUAL: operation(op, ip); break;
        case OP_NOT_EQUAL: operation(op, ip); break;
        case OP_GREATER: operation(op, ip); break;
        case OP_LESS: operation(op, ip); break;
        case OP_GREATER_EQUAL: operation(op, ip); break;
        case OP_LESS_EQUAL: operation(op, ip); break;

        case OP_PUSH_INT:
            {
                m_Stack.push(op.value);
                ip++;
                break;
            }

        case OP_CR:
            {
                printf("\n");
                ip++;
                break;
            }
        case OP_PRINT:
            {
                if (m_Stack.empty())
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();
                printf("%d", a.vInt);
                ip++;
                break;
            }
        case OP_DOT:
            {
                if (m_Stack.empty())
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();
                printf("%c", a.vInt);
                ip++;
                break;
            }

        case OP_DUP:
            {
                if (m_Stack.empty())
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();
                m_Stack.push(a);
                m_Stack.push(a);
                ip++;
                break;
            }
        case OP_DROP:
            {
                if (m_Stack.empty())
                    runtimeError("Not enough items on the stack", op);

                pop();
                ip++;
                break;
            }
        case OP_SWAP:
            {
                if (m_Stack.size() < 2)
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();
                const Value b = pop();
                m_Stack.push(a);
                m_Stack.push(b);
                ip++;

                break;
            }
        case OP_OVER:
            {
                if (m_Stack.size() < 2)
                    runtimeError("Not enough items on the stack", op);

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
                if (m_Stack.size() < 3)
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();
                const Value b = pop();
                const Value c = pop();

                m_Stack.push(b);
                m_Stack.push(a);
                m_Stack.push(c);
                ip++;

                break;
            }
        case OP_IF:
            ip++;
            break;
        case OP_THEN:
            {
                if (m_Stack.size() < 1)
                    runtimeError("Not enough items on the stack", op);

                const Value a = pop();

                if (a.type != TYPE_INT)
                    runtimeError("Invalid type", op);

                bool equal = a.vInt;

                if (equal)
                {
                    ip++;
                }
                else
                {
                    if (op.value.vIpOffset != 0)
                    {
                        ip += op.value.vIpOffset;
                    }
                    else
                    {
                        size_t ipOffset = 0;
                        size_t ifCount = 0;
                        while ((ip + ipOffset) < m_OpCodes.size())
                        {
                            ipOffset++;

                            if (m_OpCodes[(ip + ipOffset)].code == OP_IF)
                            {
                                ifCount++;
                            }
                            else if (m_OpCodes[(ip + ipOffset)].code == OP_ENDIF)
                            {
                                if (ifCount == 0)
                                {
                                    op.value.vIpOffset = ipOffset;
                                    ip += ipOffset;
                                    break;
                                }
                                else
                                    ifCount--;
                            }
                        }
                    }
                }

            break;
            }
        case OP_ENDIF:
            ip++;
            break;

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
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], code.value.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void VM::operation(const OpCode& op, size_t& ip)
{
    if (m_Stack.size() < 2)
        runtimeError("Not enough items on the stack", op);

    Value b = pop();
    Value a = pop();

    assert(a.type == b.type); // TYPES ARE EQUAL

    Value v;
    v.type = a.type;

    switch (op.code)
    {
    case OP_ADD:        value_add(a, b, v, op); break;
    case OP_SUBTRACT:   value_subtract(a, b, v, op); break;
    case OP_MULTIPLY:   value_multiply(a, b, v, op); break;
    case OP_DIVIDE:     value_divide(a, b, v, op); break;
    case OP_MOD:        value_mod(a, b, v, op); break;

    case OP_EQUAL:          value_equal(a, b, v, op); break;
    case OP_NOT_EQUAL:      value_not_equal(a, b, v, op); break;
    case OP_GREATER:        value_greater(a, b, v, op); break;
    case OP_LESS:           value_less(a, b, v, op); break;
    case OP_GREATER_EQUAL:  value_greater_equal(a, b, v, op); break;
    case OP_LESS_EQUAL:     value_less_equal(a, b, v, op); break;
    default:
        assert(false); // UNREACHABLE
    }

    m_Stack.push(v);
    ip++;
}

void VM::runtimeError(const char* msg, const OpCode& code)
{
    fprintf(stderr, "[RUNTIME ERROR] %ld:%ld %s\n", code.line, code.column, msg);
    exit(-1);
}
