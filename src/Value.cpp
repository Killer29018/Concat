#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

void value_add(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = a.vInt + b.vInt;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_subtract(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = a.vInt - b.vInt;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_multiply(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = a.vInt * b.vInt;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_divide(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = a.vInt / b.vInt;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_mod(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = a.vInt % b.vInt;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}


void value_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt == b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt != b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_greater(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt > b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_less(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt < b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt >= b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt <= b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_invert(const Value& a, Value& rV, const OpCode& op)
{
    // TODO Add bools
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);

    switch (a.type)
    {
        case TYPE_INT:
            if (a.vInt != 0) rV.vInt = 0;
            else rV.vInt = 1;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_land(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt & b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_lor(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt | b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_lnot(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);

    switch (a.type)
    {
    case TYPE_INT:
        rV.vInt = ~(a.vInt);
        break;
    default:
        assert(false); // UNREACHABLE
    }
}
