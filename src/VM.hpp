#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <vector>
#include <stack>
#include <unordered_map>

#include "SmartPointer.hpp"
#include "OpCodes.hpp"

class VM
{
private:
    static std::vector<OpCode> m_OpCodes;
    static std::stack<SmartPointer> m_Stack;
    static std::stack<size_t> m_ReturnStack;
    static std::stack<vFunc*> m_ReturnFuncStack;

    static std::vector<uint8_t> m_Memory;
    static std::unordered_map<uint32_t, size_t> m_MemoryNames;
    static std::vector<size_t> m_Functions;
    static std::unordered_map<size_t, SmartPointer> m_FunctionDefinitions;

    static std::vector<SmartPointer> m_GlobalVariables;
    static std::vector<SmartPointer> m_GlobalConstants;

    static std::vector<std::vector<SmartPointer>> m_LocalVariables;
    static std::vector<std::vector<SmartPointer>> m_LocalConstants;

    static int32_t m_CurrentMemIndex;
    static uint32_t m_CurrentFuncIndex;
public:
    static void addOpCode(OpCodeEnum code);
    static void addOpCode(const OpCode& code);
    static void pushInt(int32_t value);
    static uint32_t addMemory(uint32_t bytes);
    static uint32_t addFunction();
    static void addFunctionDefinition(OpCode& code);
    static uint32_t addGlobalVariable(SmartPointer value);
    static uint32_t addGlobalConstant();

    static uint32_t addLocalVariable(uint32_t funcIndex, SmartPointer value);
    static uint32_t addLocalConstant(uint32_t funcIndex);

    static size_t getMemorySize() { return m_Memory.size(); }
    static std::vector<SmartPointer>& getVariables() { return m_GlobalVariables; }
    static std::vector<std::vector<SmartPointer>>& getLocalVariables() { return m_LocalVariables; }
    static vFunc* getFunction(size_t index) { return (vFunc*)(m_FunctionDefinitions[index].get()); }
    static size_t getFunctionsSize() { return m_Functions.size(); }

    static void printOpCodes();

    static void simulate();

    static void build(const char* filename);

    static void clear() { m_OpCodes.clear(); m_Memory.clear(); m_MemoryNames.clear(); m_Functions.clear(); m_FunctionDefinitions.clear(); m_GlobalConstants.clear(); m_GlobalVariables.clear(); m_LocalVariables.clear(); m_LocalConstants.clear(); }
private:
    VM() = default;
    ~VM() = default;

    static void printValueDebug(size_t index);

    static void operation(const OpCode& op, size_t& ip);

    static const SmartPointer loadMemory(const SmartPointer& address, size_t bytes);
    static void writeMemory(const SmartPointer& address, const SmartPointer& value, size_t bytes);

    static void writeMemoryVar(const SmartPointer& address, const SmartPointer& value, const OpCode& op);

    static void inplaceMemOperation(const OpCode& op);

    static SmartPointer& getVariable(const SmartPointer& value);
    static void writeVariable(const SmartPointer& target, const SmartPointer& value);

    static const SmartPointer pop();
    static const SmartPointer popDirect();
};

#endif
