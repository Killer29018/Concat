#include "VM.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <iostream>

#include "Error.hpp"

#include "Builder.hpp"

std::vector<OpCode> VM::m_OpCodes;
std::stack<SmartPointer> VM::m_Stack;
std::vector<uint8_t> VM::m_Memory;
std::unordered_map<uint32_t, size_t> VM::m_MemoryNames;
int32_t VM::m_CurrentVarIndex = -1;

constexpr int STRINGPADDING = 20;

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
    op.code = OP_INT;
    op.value = makeSmartPointer<vInt>(value);
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
            printf("%.4lu | %-*s |\n", i, STRINGPADDING, OpCodeString[m_OpCodes[i].code]);
            continue;
        }

        switch (m_OpCodes[i].value->type)
        {
        case TYPE_NULL:
            printf("%.4lu | %-*s |\n", i, STRINGPADDING, OpCodeString[m_OpCodes[i].code]);
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
        case OP_INT:
        case OP_CHAR:
        case OP_STRING:
        case OP_BOOL:
            {
                m_Stack.push(op.value);

                ip++;
                break;
            }

        case OP_CAST:
            {
                const SmartPointer& a = pop();
                SmartPointer rV;

                a->cast(rV, op);

                m_Stack.push(rV);

                ip++;
                break;
            }

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
        case OP_LNOT:
        case OP_LAND:
        case OP_LOR:
        case OP_RSHIFT:
        case OP_LSHIFT:
            operation(op, ip); break;

        case OP_READ_MEMORY_32:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const SmartPointer& a = pop();

                if (a->type != TYPE_MEM_PTR)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a->type]);
                }

                const SmartPointer rV = loadMemory(a, 4);
                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_WRITE_MEMORY_32:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const SmartPointer& value = pop();
                const SmartPointer& address = pop();

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
        case OP_ADD_WRITE_MEMORY_32:
        case OP_SUBTRACT_WRITE_MEMORY_32:
        case OP_MULTIPLY_WRITE_MEMORY_32:
        case OP_DIVIDE_WRITE_MEMORY_32:
            inplaceMemOperation(op); ip++; break;

        case OP_READ_MEMORY_8:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const SmartPointer& a = pop();
                SmartPointer rV;

                if (a->type == TYPE_MEM_PTR)
                {
                    rV = loadMemory(a, 1);

                }
                else if (a->type == TYPE_STRING)
                {
                    rV = makeSmartPointer<vChar>(get_vString(a)[0]);
                }
                else
                {
                    Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", 
                            ValueTypeString[TYPE_MEM_PTR], 
                            ValueTypeString[TYPE_STRING], 
                            ValueTypeString[a->type]);
                }

                m_Stack.push(rV);
                ip++;

                break;
            }
        case OP_WRITE_MEMORY_8:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const SmartPointer& value = pop();
                const SmartPointer& address = pop();

                if (address->type == TYPE_MEM_PTR)
                {
                    if (value->type == TYPE_INT)
                    {
                        writeMemory(address, value, 1);
                    }
                    else
                    {
                        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[address->type]);
                    }
                }
                else if (address->type == TYPE_STRING)
                {
                    if (value->type == TYPE_CHAR)
                    {
                        get_vString(address)[0] = get_vChar(value);
                    }
                    else
                    {
                        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_CHAR], ValueTypeString[address->type]);
                    }
                }
                else
                {
                    Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", 
                            ValueTypeString[TYPE_MEM_PTR], 
                            ValueTypeString[TYPE_STRING], 
                            ValueTypeString[address->type]);
                }

                ip++;
                break;
            }
        case OP_ADD_WRITE_MEMORY_8:
        case OP_SUBTRACT_WRITE_MEMORY_8:
        case OP_MULTIPLY_WRITE_MEMORY_8:
        case OP_DIVIDE_WRITE_MEMORY_8:
            inplaceMemOperation(op); ip++; break;

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

                const SmartPointer& a = pop();

                a->print(op);

                ip++;

                break;
            }
        case OP_DOT:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const SmartPointer& a = pop();

                a->dot(op);

                ip++;
                break;
            }

        case OP_INPUT:
            {
                std::string input;
                std::cin >> input;

                char* cInput = new char[input.length() + 1];
                strcpy(cInput, input.c_str());
                cInput[input.length()] = 0;

                SmartPointer a = makeSmartPointer<vString>(cInput);

                m_Stack.push(a);
                ip++;
                break;
            }

        case OP_DUP:
            {
                if (m_Stack.empty())
                    Error::stackTooSmallError(op, 1);

                const SmartPointer& a = pop();
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

                const SmartPointer& a = pop();
                const SmartPointer& b = pop();
                m_Stack.push(a);
                m_Stack.push(b);
                ip++;

                break;
            }
        case OP_OVER:
            {
                if (m_Stack.size() < 2)
                    Error::stackTooSmallError(op, 2);

                const SmartPointer& a = pop();
                const SmartPointer& b = pop();
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

                const SmartPointer& a = pop();
                const SmartPointer& b = pop();
                const SmartPointer& c = pop();

                m_Stack.push(b);
                m_Stack.push(a);
                m_Stack.push(c);
                ip++;

                break;
            }

        case OP_CREATE_MEM:
            {
                m_CurrentVarIndex = get_vMemPtr(op.value);
                m_MemoryNames.insert({ m_CurrentVarIndex, 0 });
                ip++;
                break;
            }
        case OP_LOAD_MEM:
            {
                size_t index = m_MemoryNames.at(get_vMemPtr(op.value));
                SmartPointer a = makeSmartPointer<vMemPtr>(index);
                m_Stack.push(a);
                ip++;
                break;
            }
        case OP_ENDMEM:
            {
                if (m_Stack.size() < 1)
                    Error::stackTooSmallError(op, 1);

                const SmartPointer& a = pop();

                if (m_CurrentVarIndex == -1)
                    assert(false && "Unreachable. Should be dealt with in Compiler");

                if (a->type != TYPE_INT)
                {
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
                }

                uint32_t index = addMemory(get_vInt(a));

                m_MemoryNames.at(m_CurrentVarIndex) = index;

                m_CurrentVarIndex = -1;
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

                const SmartPointer& a = pop();

                if (a->type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);

                bool boolTrue = get_vBool(a);

                if (get_vIpOffset(op.value) == 0)
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset);
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset + 1);
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
                    ip += get_vIpOffset(op.value);
                }

                break;
            }
        case OP_ELSEIF:
            {
                if (get_vIpOffset(op.value) == 0)
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset);
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += get_vIpOffset(op.value);

                ip++;
                break;
            }
        case OP_ELSE:
            {
                if (get_vIpOffset(op.value) == 0)
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset);
                                break;
                            }
                            else
                            {
                                ifCount--;
                            }
                        }
                    }
                }
                
                ip += get_vIpOffset(op.value);

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

                const SmartPointer& a = pop();

                if (a->type != TYPE_BOOL)
                    Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);

                bool boolTrue = get_vBool(a);

                if(get_vIpOffset(op.value) == 0)
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset + 1);
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
                    ip += get_vIpOffset(op.value);
                }
                break;
            }
        case OP_ENDWHILE: 
            {
                if(get_vIpOffset(op.value) == 0)
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
                                op.value = makeSmartPointer<vIpOffset>(ipOffset);
                                break;
                            }
                            else
                            {
                                endWhileCount--;
                            }
                        }
                    }
                }

                ip -= get_vIpOffset(op.value);
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
        printf("%.4lu | %-*s | %d\n", index, STRINGPADDING, OpCodeString[code.code], get_vInt(code.value));
        break;
    case TYPE_BOOL:
        printf("%.4lu | %-*s | %d\n", index, STRINGPADDING, OpCodeString[code.code], get_vBool(code.value));
        break;
    case TYPE_CHAR:
        printf("%.4lu | %-*s | %c\n", index, STRINGPADDING, OpCodeString[code.code], get_vChar(code.value));
        break;
    case TYPE_STRING:
        printf("%.4lu | %-*s | %s\n", index, STRINGPADDING, OpCodeString[code.code], get_vString(code.value));
        break;
    case TYPE_MEM_PTR:
        printf("%.4lu | %-*s | %d\n", index, STRINGPADDING, OpCodeString[code.code], get_vMemPtr(code.value));
        break;
    case TYPE_IP_OFFSET:
        printf("%.4lu | %-*s | %d\n", index, STRINGPADDING, OpCodeString[code.code], get_vIpOffset(code.value));
        break;
    break;
    default:
        assert(false && "Unreachable"); // UNREACHABLE
    }
}

void VM::operation(const OpCode& op, size_t& ip)
{
    if (m_Stack.size() == 0)
        Error::stackTooSmallError(op, 1);

    const SmartPointer& b = pop();

    SmartPointer v;

    switch (op.code)
    {
        case OP_INVERT:
        case OP_LNOT:
            runOperation(b, v, op); 
            break;
        
        default:
            if (m_Stack.size() < 1) // One value already removed
                Error::stackTooSmallError(op, 2);

            const SmartPointer& a = pop();
            runOperation(a, b, v, op); 
            break;
    }

    m_Stack.push(v);
    ip++;
}

const SmartPointer VM::loadMemory(const SmartPointer& address, size_t bytes)
{
    int32_t v = 0;
    for (size_t i = 0; i < bytes; i++)
    {
        v <<= 8;
        uint8_t element = m_Memory[get_vMemPtr(address) + i];
        v |= element;
    }

    return makeSmartPointer<vInt>(v);
}

void VM::writeMemory(const SmartPointer& address, const SmartPointer& value, size_t bytes)
{
    int32_t v = get_vInt(value);
    for (size_t i = 1; i <= bytes; i++)
    {
        m_Memory[get_vMemPtr(address) + (bytes - i)] = v & 0xFF;
        v >>= 8;
    }
}

void VM::inplaceMemOperation(const OpCode& op)
{
    if (m_Stack.size() < 2)
        Error::stackTooSmallError(op, 2);

    const SmartPointer& operand = pop();
    const SmartPointer& address = pop();

    if (address->type != TYPE_MEM_PTR)
    {
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_MEM_PTR], ValueTypeString[address->type]);
    }

    size_t size = 0;
    switch (op.code)
    {
        case OP_ADD_WRITE_MEMORY_32:
        case OP_SUBTRACT_WRITE_MEMORY_32:
        case OP_MULTIPLY_WRITE_MEMORY_32:
        case OP_DIVIDE_WRITE_MEMORY_32:
            size = 4; break;

        case OP_ADD_WRITE_MEMORY_8:
        case OP_SUBTRACT_WRITE_MEMORY_8:
        case OP_MULTIPLY_WRITE_MEMORY_8:
        case OP_DIVIDE_WRITE_MEMORY_8:
            size = 1; break;

        default:
            assert(false && "Not reachable"); break;
    }

    SmartPointer rV = loadMemory(address, size);
    OpCode changeOp = op;
    switch (op.code)
    {
        case OP_ADD_WRITE_MEMORY_32:
            changeOp.code = OP_ADD;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_SUBTRACT_WRITE_MEMORY_32:
            changeOp.code = OP_SUBTRACT;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_MULTIPLY_WRITE_MEMORY_32:
            changeOp.code = OP_MULTIPLY;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_DIVIDE_WRITE_MEMORY_32:
            changeOp.code = OP_DIVIDE;
            runOperation(rV, operand, rV, changeOp); break;

        case OP_ADD_WRITE_MEMORY_8:
            changeOp.code = OP_ADD;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_SUBTRACT_WRITE_MEMORY_8:
            changeOp.code = OP_SUBTRACT;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_MULTIPLY_WRITE_MEMORY_8:
            changeOp.code = OP_MULTIPLY;
            runOperation(rV, operand, rV, changeOp); break;
        case OP_DIVIDE_WRITE_MEMORY_8:
            changeOp.code = OP_DIVIDE;
            runOperation(rV, operand, rV, changeOp); break;

        default:
            assert(false && "Not reachable"); break;
    }

    writeMemory(address, rV, size);
}
