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
    TYPE_IP_OFFSET,
};

typedef int32_t VALUE_TYPE;

struct Value
{
public:
    ValueType type;
    union
    {
        VALUE_TYPE vInt;
        VALUE_TYPE vBool;
        VALUE_TYPE vIpOffset;
    } as;
public:
    static void Add(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Subtract(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Multiply(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Divide(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Mod(const Value& a, const Value& b, Value& rV, const OpCode& op);

    static void Equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Greater(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Less(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);

    static void Invert(const Value& a, Value& rV, const OpCode& op);
    static void Land(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Lor(const Value& a, const Value& b, Value& rV, const OpCode& op);
    static void Lnot(const Value& a, Value& rV, const OpCode& op);
};

const std::vector<const char*> ValueTypeString
{
    "NULL",
    "INT",
    "BOOL",
    "IpOffset"
};


#endif
