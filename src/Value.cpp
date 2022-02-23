#include "Value.hpp"

void addValue(const Value& a, const Value& b, Value& rV)
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

void subtractValue(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = a.as.v_Int - b.as.v_Int;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void multiplyValue(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = a.as.v_Int * b.as.v_Int;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void divideValue(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = a.as.v_Int / b.as.v_Int;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void modValue(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = a.as.v_Int % b.as.v_Int;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}
