#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <vector>

#include <cstdint>

#include <cassert>

enum ValueType
{
    TYPE_NULL,
    TYPE_INT
};

struct Value
{
    ValueType type;
    union
    {
        int32_t v_Int;
    } as;
};

enum OpCodeEnum
{
    OP_ADD          = 0x00,
    OP_PUSH_INT     = 0x01,
    OP_PRINT        = 0x02,
};

const std::vector<const char*> OpCodeString
{
    "OP_ADD",
    "OP_PUSH_INT",
    "OP_PRINT"
};

struct OpCode
{
    OpCodeEnum code;
    Value value;
};

#define AS_INT(a) a.as.v_Int

inline void addValue(Value& a, Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = a.as.v_Int + b.as.v_Int;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

#endif
