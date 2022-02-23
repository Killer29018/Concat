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

// void addValue(const Value& a, const Value& b, Value& rV);
// void subtractValue(const Value& a, const Value& b, Value& rV);
// void multiplyValue(const Value& a, const Value& b, Value& rV);
// void divideValue(const Value& a, const Value& b, Value& rV);
// void modValue(const Value& a, const Value& b, Value& rV);

void value_add(const Value& a, const Value& b, Value& rV);
void value_subtract(const Value& a, const Value& b, Value& rV);
void value_multiply(const Value& a, const Value& b, Value& rV);
void value_divide(const Value& a, const Value& b, Value& rV);
void value_mod(const Value& a, const Value& b, Value& rV);

void value_equal(const Value& a, const Value& b, Value& rV);
void value_not_equal(const Value& a, const Value& b, Value& rV);
void value_greater(const Value& a, const Value& b, Value& rV);
void value_less(const Value& a, const Value& b, Value& rV);
void value_greater_equal(const Value& a, const Value& b, Value& rV);
void value_less_equal(const Value& a, const Value& b, Value& rV);

#endif
