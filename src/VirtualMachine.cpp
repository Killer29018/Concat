#include "VirtualMachine.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "Error.hpp"

#include "Builder.hpp"

std::vector<OpCode> VM::m_OpCodes;
std::stack<const Value*> VM::m_Stack;
std::vector<uint8_t> VM::m_Memory;
std::unordered_map<uint32_t, size_t> VM::m_MemoryNames;
int32_t VM::m_CurrentVarIndex = -1;


void VM::addOpCode(OpCodeEnum code)
{
    OpCode op;
    op.code = code;
    op.value = nullptr;
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
    op.value = new vInt(value);
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
        if (!m_OpCodes[i].value)
        {
            printf("%.4lu | %.30s\n", i, OpCodeString[m_OpCodes[i].code]);
            continue;
        }

        switch (m_OpCodes[i].value->type)
        {
        case TYPE_NULL:
            printf("%.4lu | %.30s\n", i, OpCodeString[m_OpCodes[i].code]);
            break;
        default:
            printValueDebug(i); break;
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
        case OP_SUBTRACT: 
        case OP_MULTIPLY: 
        case OP_DIVIDE: 
        case OP_MOD: 
            operation(op, ip); break;

        case OP_EQUAL: 
        case OP_NOT_EQUAL: 
        case OP_GREATER: 
        case OP_LESS: 
        case OP_GREATER_EQUAL: 
        case OP_LESS_EQUAL: 
            operation(op, ip); break;
                            
        case OP_INVERT:
            {
                const Value* a = pop();
                Value* rV;

                value_invert(a, &rV, op);
                m_Stack.push(rV);
                ip++;
                break;
            }
        case OP_LNOT:
            {
                const Value* a = pop();
                Value* rV;

                value_lnot(a, &rV, op);
                m_Stack.push(rV);
                ip++;
                break;
            }
        case OP_LAND:
        case OP_LOR:
        case OP_RSHIFT:
        case OP_LSHIFT:
            operation(op, ip); break;

        case OP_CREATE_MEM:
            {
                m_CurrentVarIndex = as_vMemPtr(op.value);
                m_MemoryNames.insert({ m_CurrentVarIndex, 0 });
                ip++;
                break;
            }
        case OP_LOAD_MEM:
            {
                size_t index = m_MemoryNames.at(as_vMemPtr(op.value));
                Value* a = new vMemPtr(index);
                m_Stack.push(a);
                ip++;
                break;
            }
        case OP_ENDMEM:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const Value* a = pop();

                if (m_CurrentVarIndex == -1)
                    assert(false && "Unreachable. Should be dealt with in Compiler");

                if (a->type != TYPE_INT)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
                }

                uint32_t index = addMemory(as_vInt(a));

                m_MemoryNames.at(m_CurrentVarIndex) = index;

                m_CurrentVarIndex = -1;
                ip++;
                break;
            }

        case OP_READ_MEMORY_32:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const Value* a = pop();

                if (a->type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a->type]);
                }

                Value* rV = loadMemory(a, 4);
                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_WRITE_MEMORY_32:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const Value* address = pop();
                const Value* value = pop();

                if (address->type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[address->type]);
                }

                if (value->type != TYPE_INT)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[value->type]);
                }

                writeMemory(address, value, 4);
                ip++;
                break;
            }
        case OP_READ_MEMORY_8:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const Value* a = pop();

                if (a->type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a->type]);
                }

                Value* rV = loadMemory(a, 1);
                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_WRITE_MEMORY_8:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const Value* address = pop();
                const Value* value = pop();

                if (address->type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[address->type]);
                }

                if (value->type != TYPE_INT)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[value->type]);
                }

                writeMemory(address, value, 1);
                ip++;
                break;
            }

        case OP_PUSH_INT:
        case OP_PUSH_CHAR:
        case OP_PUSH_CSTRING:
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

                const Value* a = pop();

                switch (a->type)
                {
                    case TYPE_INT:
                        printf("%d", as_vInt(a)); break;
                    case TYPE_BOOL:
                        printf("%d", as_vBool(a)); break;
                    case TYPE_CHAR:
                        printf("%c", as_vChar(a)); break;
                    case TYPE_CSTRING:
                        printf("%s", as_vCString(a)); break;
                    case TYPE_MEM_PTR:
                        printf("%d", as_vMemPtr(a)); break;
                    default:
                        assert(false && "Not Reachable");
                }
                ip++;

                break;
            }
        case OP_DOT:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value* a = pop();

                switch (a->type)
                {
                case TYPE_INT:
                    printf("%c", as_vInt(a)); break;
                case TYPE_BOOL:
                    printf("%i", as_vBool(a)); break;
                case TYPE_CHAR:
                    printf("%i", as_vChar(a)); break;
                case TYPE_CSTRING:
                    printf("%c", as_vCString(a)[0]); break;

                default:
                    Error::runtimeError(op, "Invalid Type");
                }

                ip++;
                break;
            }

        case OP_DUP:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const Value* a = pop();
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

                const Value* a = pop();
                const Value* b = pop();
                m_Stack.push(a);
                m_Stack.push(b);
                ip++;

                break;
            }
        case OP_OVER:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const Value* a = pop();
                const Value* b = pop();
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

                const Value* a = pop();
                const Value* b = pop();
                const Value* c = pop();

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

                const Value* a = pop();

                if (a->type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);

                bool boolTrue = as_vBool(a);

                if (as_vIpOffset(op.value) == 0)
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
                                op.value = new vIpOffset(ipOffset);
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
                                op.value = new vIpOffset(ipOffset + 1);
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
                    ip += as_vIpOffset(op.value);
                }

                break;
            }
        case OP_ELSEIF:
            {
                if (as_vIpOffset(op.value) == 0)
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
                                op.value = new vIpOffset(ipOffset);
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += as_vIpOffset(op.value);

                ip++;
                break;
            }
        case OP_ELSE:
            {
                if (as_vIpOffset(op.value) == 0)
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
                                op.value = new vIpOffset(ipOffset);
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += as_vIpOffset(op.value);

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

                const Value* a = pop();

                if (a->type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);

                bool boolTrue = as_vBool(a);

                if(as_vIpOffset(op.value) == 0)
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
                                op.value = new vIpOffset(ipOffset + 1);
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
                    ip += as_vIpOffset(op.value);
                }
                break;
            }
        case OP_ENDWHILE: 
            {
                if(as_vIpOffset(op.value) == 0)
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
                                op.value = new vIpOffset(ipOffset);
                                break;
                            }
                            else
                            {
                                endWhileCount--;
                            }
                        }
                    }
                }

                ip -= as_vIpOffset(op.value);
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
    switch (code.value->type)
    {
    case TYPE_INT:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], as_vInt(code.value));
        break;
    case TYPE_BOOL:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], as_vBool(code.value));
        break;
    case TYPE_CHAR:
        printf("%.4lu | %.30s | %c\n", index, OpCodeString[code.code], as_vChar(code.value));
        break;
    case TYPE_MEM_PTR:
        printf("%.4lu | %.30s | %d\n", index, OpCodeString[code.code], as_vMemPtr(code.value));
        break;
    break;
    default:
        assert(false && "Unreachable"); // UNREACHABLE
    }
}

void VM::operation(const OpCode& op, size_t& ip)
{
    if (m_Stack.size() < 2)
        Error::stackTooSmallError(op, 2);

    const Value* b = pop();
    const Value* a = pop();

    Value* v = nullptr;

    switch (op.code)
    {
    case OP_ADD:
        value_add(a, b, &v, op); break;
    case OP_SUBTRACT:
        value_subtract(a, b, &v, op); break;
    case OP_MULTIPLY:
        value_multiply(a, b, &v, op); break;
    case OP_DIVIDE:
        value_divide(a, b, &v, op); break;
    case OP_MOD:
        value_mod(a, b, &v, op); break;

    case OP_EQUAL:
        value_equal(a, b, &v, op); break;
    case OP_NOT_EQUAL:
        value_not_equal(a, b, &v, op); break;
    case OP_GREATER:
        value_greater(a, b, &v, op); break;
    case OP_LESS:
        value_less(a, b, &v, op); break;
    case OP_GREATER_EQUAL:
        value_greater_equal(a, b, &v, op); break;
    case OP_LESS_EQUAL:
        value_less_equal(a, b, &v, op); break;

    case OP_LAND:
        value_land(a, b, &v, op); break;
    case OP_LOR:
        value_lor(a, b, &v, op); break;
    case OP_RSHIFT:
        value_rshift(a, b, &v, op); break;
    case OP_LSHIFT:
        value_lshift(a, b, &v, op); break;
    default:
        assert(false && "Unreachable"); // UNREACHABLE
    }

    m_Stack.push(v);
    ip++;
}

Value* VM::loadMemory(const Value* address, size_t bytes)
{
    int32_t v = 0;
    for (size_t i = 0; i < bytes; i++)
    {
        v <<= 8;
        uint8_t element = m_Memory[as_vMemPtr(address) + i];
        v |= element;
    }

    return new vInt(v);
}

void VM::writeMemory(const Value* address, const Value* value, size_t bytes)
{
    int32_t v = as_vInt(value);
    for (size_t i = 1; i <= bytes; i++)
    {
        m_Memory[as_vMemPtr(address) + (bytes - i)] = v & 0xFF;
        v >>= 8;
    }
}
