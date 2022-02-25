#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

// TODO: Type checking doesn't work if types are different
// TODO: Return Type should be set within the operations themselves

void Value::Add(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vInt = a.vInt + b.vInt;
    }
    else
    {
        assert(false);
    }
}

void Value::Subtract(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vInt = a.vInt - b.vInt;
    }
    else
    {
        assert(false);
    }
}

void Value::Multiply(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vInt = a.vInt * b.vInt;
    }
    else
    {
        assert(false);
    }
}

void Value::Divide(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vInt = a.vInt / b.vInt;
    }
    else
    {
        assert(false);
    }
}

void Value::Mod(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vInt = a.vInt % b.vInt;
    }
    else
    {
        assert(false);
    }
}


void Value::Equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
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

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt == b.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vBool == b.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt == b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
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

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt != b.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vBool != b.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt == b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Greater(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt > b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Less(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt < b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt >= b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    if (b.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vInt <= b.vInt);
    }
    else
    {
        assert(false);
    }
}

void Value::Invert(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_BOOL)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

    rV.vBool = !a.vBool;
}

void Value::Land(const Value& a, const Value& b, Value& rV, const OpCode& op)
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

    if (a.type != b.type)
        Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a.type], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vBool = (a.vInt & b.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vBool && b.vBool);
    }
    else
    {
        assert(false);
    }
}

void Value::Lor(const Value& a, const Value& b, Value& rV, const OpCode& op)
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

    if (a.type != b.type)
        Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a.type], ValueTypeString[b.type]);

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.vBool = (a.vInt | b.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.vBool = (a.vBool || b.vBool);
    }
    else
    {
        assert(false);
    }
}

void Value::Lnot(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);

    rV.type = TYPE_INT;
    rV.vInt = ~(a.vInt);
}
