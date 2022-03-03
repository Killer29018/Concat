#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <cstdint>

#include <vector>

struct OpCode;

enum ValueType
{
    TYPE_NULL,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_MEM_PTR,

    TYPE_IP_OFFSET,

    TYPE_COUNT
};

typedef int32_t VALUE_TYPE;

struct Value
{
public:
    ValueType type;
    union
    {
        VALUE_TYPE value;
        VALUE_TYPE vInt;
        VALUE_TYPE vBool;
        VALUE_TYPE vChar;
        VALUE_TYPE vMemPtr;

        VALUE_TYPE vIpOffset;
    } as;

    Value() = default;

    Value(ValueType t, VALUE_TYPE value)
        : type(t)
    {
        switch (type)
        {
        case TYPE_COUNT:
        case TYPE_NULL: break;
        case TYPE_INT: as.vInt = value; break;
        case TYPE_BOOL: as.vBool = value; break;
        case TYPE_MEM_PTR: as.vMemPtr = value; break;
        case TYPE_CHAR: as.vChar = value; break;

        case TYPE_IP_OFFSET: as.vIpOffset = value; break;
        }

    }
public:
    static void add(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void subtract(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void multiply(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void divide(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void mod(const Value& a, const Value& b, Value& rV, const OpCode& op);

    static void equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void greater(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void less(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);

    static void invert(const Value& a, Value& rV, const OpCode& op);
    static void lnot(const Value& a, Value& rV, const OpCode& op);
    static void land(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void lor(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void rshift(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void lshift(const Value& a, const Value& b, Value& rv, const OpCode& op);
};

const std::vector<const char*> ValueTypeString
{
    "NULL",
    "INT",
    "BOOL",
    "CHAR",
    "MEM_PTR",
    "IP_OFFSET",
};


#endif
