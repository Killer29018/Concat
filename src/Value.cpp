#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

void value_add(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for addition", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = a.vInt + b.vInt;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_subtract(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for subtraction", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = a.vInt - b.vInt;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_multiply(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for multiplication", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = a.vInt * b.vInt;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_divide(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for division", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = a.vInt / b.vInt;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_mod(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for modulos", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = a.vInt % b.vInt;
        break;
    default:
        assert(false); // UNREACHABLE
    }
}


void value_equal(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if equal", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt == b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_not_equal(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if not equal", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt != b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_greater(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if greater", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt > b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_less(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if less", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt < b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if greater or equal", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt >= b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}

void value_less_equal(const Value& a, const Value& b, Value& rV, const OpCode& code)
{
    if (a.type != TYPE_INT)
        VM::runtimeError("Type not supported for checking if less or equal", code);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = (a.vInt <= b.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}
