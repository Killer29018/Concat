#include "VirtualMachine.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "Error.hpp"

#include "Builder.hpp"

std::vector<OpCode> VM::m_OpCodes;
std::stack<Value> VM::m_Stack;
std::vector<uint8_t> VM::m_Memory;

void VM::addOpCode(OpCodeEnum code)
{
    OpCode op;
    op.code = code;
    op.value = { TYPE_NULL, 0 };
    m_OpCodes.emplace_back(op);
}

void VM::addOpCode(const OpCode& code)
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

uint32_t VM::addMemory(uint32_t bytes)
{
    size_t p = m_Memory.size();
    for (uint32_t i = 0; i < bytes; i++)
        m_Memory.push_back(0);
    return p;
}

void VM::printOpCodes()
{
    printf("--- OPCODES ---\n");
    for (size_t i = 0; i < m_OpCodes.size(); i++)
    {
        switch (m_OpCodes[i].code)
        {
        case OP_PUSH_INT: 
        case OP_VAR:
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
                            
        case OP_INVERT:
            {
                const Value a = pop();
                Value rV;

                Value::Invert(a, rV, op);
                m_Stack.push(rV);
                ip++;
                break;
            }
        case OP_LAND: operation(op, ip); break;
        case OP_LOR: operation(op, ip); break;
        case OP_LNOT:
            {
                const Value a = pop();
                Value rV;

                Value::Lnot(a, rV, op);
                m_Stack.push(rV);
                ip++;
                break;
            }
        case OP_VAR:
            {
                Value rV = op.value;
                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_READ_MEMORY:
            {
                const Value a = pop();

                if (a.type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a.type]);
                }

                Value rV = loadMemory(a);
                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_WRITE_MEMORY:
            {
                const Value address = pop();
                const Value value = pop();

                if (address.type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[address.type]);
                }

                if (value.type != TYPE_INT)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[value.type]);
                }

                writeMemory(address, value);
                ip++;
                break;
            }

        case OP_PUSH_INT:
        case OP_TRUE:
        case OP_FALSE:
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
                    Error::stackTooSmallError(op, 1);

                const Value a = pop();
                printf("%d", a.as.vInt);
                ip++;
                break;
            }
        case OP_DOT:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value a = pop();
                printf("%c", a.as.vInt);
                ip++;
                break;
            }

        case OP_DUP:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value a = pop();
                m_Stack.push(a);
                m_Stack.push(a);
                ip++;
                break;
            }
        case OP_DROP:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                pop();
                ip++;
                break;
            }
        case OP_SWAP:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

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
                    Error::stackTooSmallError(op, 2);

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
                    Error::stackTooSmallError(op, 3);

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
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value a = pop();

                if (a.type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

                bool boolTrue = a.as.vBool;

                if (op.value.as.vIpOffset == 0)
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
                                op.value.as.vIpOffset = ipOffset;
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                        else if (m_OpCodes[(ip + ipOffset)].code == OP_ELSEIF ||
                                 m_OpCodes[(ip + ipOffset)].code == OP_ELSE)
                        {
                            if (ifCount == 0)
                            {
                                op.value.as.vIpOffset = ipOffset + 1;
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }

                if (boolTrue)
                {
                    ip++;
                }
                else
                {
                    ip += op.value.as.vIpOffset;
                }

                break;
            }
        case OP_ELSEIF:
            {
                if (op.value.as.vIpOffset == 0)
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
                                op.value.as.vIpOffset = ipOffset;
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += op.value.as.vIpOffset;

                ip++;
                break;
            }
        case OP_ELSE:
            {
                if (op.value.as.vIpOffset == 0)
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
                                op.value.as.vIpOffset = ipOffset;
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += op.value.as.vIpOffset;

                ip++;
                break;
            }
        case OP_ENDIF:
            ip++;
            break;

        case OP_WHILE:
            ip++;
            break;
        case OP_DO:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value a = pop();

                if (a.type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

                bool boolTrue = a.as.vBool;

                if(op.value.as.vIpOffset == 0)
                {
                    size_t ipOffset = 0;
                    size_t whileCount = 0;
                    while ((ip + ipOffset) < m_OpCodes.size())
                    {
                        ipOffset++;

                        if (m_OpCodes[(ip + ipOffset)].code == OP_WHILE)
                        {
                            whileCount++;
                        }
                        else if (m_OpCodes[(ip + ipOffset)].code == OP_ENDWHILE)
                        {
                            if (whileCount == 0)
                            {
                                op.value.as.vIpOffset = ipOffset + 1;
                                break;
                            }
                            else
                            {
                                whileCount--;
                            }
                        }
                    }
                }

                if (boolTrue)
                {
                    ip++;
                }
                else
                {
                    ip += op.value.as.vIpOffset;
                }
                break;
            }
        case OP_ENDWHILE: 
            {
                if(op.value.as.vIpOffset == 0)
                {
                    size_t ipOffset = 0;
                    size_t endWhileCount = 0;
                    while ((ip - ipOffset) > 0)
                    {
                        ipOffset++;

                        if (m_OpCodes[(ip - ipOffset)].code == OP_ENDWHILE)
                        {
                            endWhileCount++;
                        }
                        else if (m_OpCodes[(ip - ipOffset)].code == OP_WHILE)
                        {
                            if (endWhileCount == 0)
                            {
                                op.value.as.vIpOffset = ipOffset;
                                break;
                            }
                            else
                            {
                                endWhileCount--;
                            }
                        }
                    }
                }

                ip -= op.value.as.vIpOffset;
                break;
            }

        default:
            Error::runtimeError(op, "Opcode not supported %s", OpCodeString[op.code]);
            assert(false && "Unreachable"); // UNREACHABLE
        }
    }
}

void VM::build(const char* filename)
{
    Builder::buildCompiled(filename, &m_OpCodes);
}

void VM::printValueDebug(size_t index)
{
    OpCode& code = m_OpCodes[index];
    switch (code.value.type)
    {
    case TYPE_INT:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], code.value.as.vInt);
        break;
    case TYPE_MEM_PTR:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], code.value.as.vMemPtr);
        break;
    default:
        assert(false && "Unreachable"); // UNREACHABLE
    }
}

void VM::operation(const OpCode& op, size_t& ip)
{
    if (m_Stack.size() < 2)
        Error::stackTooSmallError(op, 2);

    Value b = pop();
    Value a = pop();

    Value v;

    switch (op.code)
    {
    case OP_ADD:        Value::Add(a, b, v, op); break;
    case OP_SUBTRACT:   Value::Subtract(a, b, v, op); break;
    case OP_MULTIPLY:   Value::Multiply(a, b, v, op); break;
    case OP_DIVIDE:     Value::Divide(a, b, v, op); break;
    case OP_MOD:        Value::Mod(a, b, v, op); break;

    case OP_EQUAL:          Value::Equal(a, b, v, op); break;
    case OP_NOT_EQUAL:      Value::Not_equal(a, b, v, op); break;
    case OP_GREATER:        Value::Greater(a, b, v, op); break;
    case OP_LESS:           Value::Less(a, b, v, op); break;
    case OP_GREATER_EQUAL:  Value::Greater_equal(a, b, v, op); break;
    case OP_LESS_EQUAL:     Value::Less_equal(a, b, v, op); break;

    case OP_LAND:  Value::Land(a, b, v, op); break;
    case OP_LOR:   Value::Lor(a, b, v, op); break;
    default:
        assert(false && "Unreachable"); // UNREACHABLE
    }

    m_Stack.push(v);
    ip++;
}

Value VM::loadMemory(const Value& address)
{
    VALUE_TYPE v = 0;
    for (int i = 0; i < 4; i++)
    {
        v <<= 8;
        uint8_t element = m_Memory[address.as.vMemPtr + i];
        v |= element;
    }

    return Value(TYPE_INT, v);
}

void VM::writeMemory(const Value& address, const Value& value)
{
    VALUE_TYPE v = value.as.vInt;
    for (int i = 1; i <= 4; i++)
    {
        m_Memory[address.as.vMemPtr + (4 - i)] = v & 0xFF;
        v >>= 8;
    }
}
