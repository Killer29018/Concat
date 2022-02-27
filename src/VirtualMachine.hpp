#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <vector>
#include <stack>

#include "OpCodes.hpp"

class VM
{
private:
    static std::vector<OpCode> m_OpCodes;
    static std::stack<Value> m_Stack;
public:
    static void addOpCode(OpCodeEnum code);
    static void addOpCode(const OpCode& code);
    static void pushInt(int32_t value);

    static void printOpCodes();

    static void simulate();

    static void build(const char* filename);
private:
    VM() = default;
    ~VM() = default;

    static void printValueDebug(size_t index);

    static void operation(const OpCode& op, size_t& ip);

    static Value pop() { Value v = m_Stack.top(); m_Stack.pop(); return v; }
};

#endif
