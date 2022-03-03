#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <vector>
#include <stack>
#include <unordered_map>

#include "OpCodes.hpp"

class VM
{
private:
    static std::vector<OpCode> m_OpCodes;
    static std::stack<Value> m_Stack;

    static std::vector<uint8_t> m_Memory;
    static std::unordered_map<VALUE_TYPE, size_t> m_Variables;
    static int32_t m_CurrentVarIndex;
public:
    static void addOpCode(OpCodeEnum code);
    static void addOpCode(const OpCode& code);
    static void pushInt(int32_t value);
    static uint32_t addMemory(uint32_t bytes);
    static size_t getMemorySize() { return m_Memory.size(); }

    static void printOpCodes();

    static void simulate();

    static void build(const char* filename);
private:
    VM() = default;
    ~VM() = default;

    static void printValueDebug(size_t index);

    static void operation(const OpCode& op, size_t& ip);

    static Value loadMemory(const Value& address, size_t bytes);
    static void writeMemory(const Value& address, const Value& value, size_t bytes);

    static Value pop() { Value v = m_Stack.top(); m_Stack.pop(); return v; }

};

#endif
