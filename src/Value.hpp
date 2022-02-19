#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <cstdint>

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

void addValue(const Value& a, const Value& b, Value& rV);
void subtractValue(const Value& a, const Value& b, Value& rV);
void multiplyValue(const Value& a, const Value& b, Value& rV);
void divideValue(const Value& a, const Value& b, Value& rV);

#endif
