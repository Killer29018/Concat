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
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    switch (a.type)
    {
        case TYPE_INT:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt == b.vInt);
            break;
        case TYPE_BOOL:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vBool == b.vBool);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    switch (a.type)
    {
        case TYPE_INT:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt != b.vInt);
            break;
        case TYPE_BOOL:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vBool != b.vBool);
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
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt > b.vInt);
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
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt < b.vInt);
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
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt >= b.vInt);
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
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vInt <= b.vInt);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_invert(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_BOOL)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

    switch (a.type)
    {
        case TYPE_BOOL:
            rV.vBool = !a.vBool;
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_land(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt & b.vInt);
            break;
        case TYPE_BOOL:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vBool && b.vBool);
            break;
        default:
            assert(false); // UNREACHABLE
    }
}

void value_lor(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    switch (a.type)
    {
        case TYPE_INT:
            rV.vInt = (a.vInt | b.vInt);
            break;
        case TYPE_BOOL:
            rV.type = TYPE_BOOL;
            rV.vBool = (a.vBool || b.vBool);
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
