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
    TYPE_IP_OFFSET,
};

struct Value
{
    ValueType type;
    union
    {
        int32_t vInt;
        int32_t vIpOffset;
    };
};

const std::vector<const char*> ValueTypeString
{
    "NULL",
    "INT",
    "IpOffset"
};

void value_add(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_subtract(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_multiply(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_divide(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_mod(const Value& a, const Value& b, Value& rV, const OpCode& op);

void value_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_greater(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_less(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op);

void value_invert(const Value& a, Value& rV, const OpCode& op);
void value_land(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_lor(const Value& a, const Value& b, Value& rV, const OpCode& op);
void value_lnot(const Value& a, Value& rV, const OpCode& op);

#endif
