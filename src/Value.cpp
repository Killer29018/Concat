#include "Value.hpp"

void value_add(const Value& a, const Value& b, Value& rV)
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

void value_subtract(const Value& a, const Value& b, Value& rV)
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

void value_multiply(const Value& a, const Value& b, Value& rV)
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

void value_divide(const Value& a, const Value& b, Value& rV)
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

void value_mod(const Value& a, const Value& b, Value& rV)
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


void value_equal(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int == b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_not_equal(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int != b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_greater(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int > b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_less(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int < b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_greater_equal(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int >= b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_less_equal(const Value& a, const Value& b, Value& rV)
{
    if (a.type != TYPE_INT) 
        assert(false); // TYPE NOT SUPPORTED FOR ADDITION

    switch (a.type)
    {
    case TYPE_INT:
        rV.as.v_Int = (a.as.v_Int <= b.as.v_Int);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}
