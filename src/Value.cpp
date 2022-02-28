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
        rV.as.vInt = a.as.vInt + b.as.vInt;
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vInt = a.as.vInt - b.as.vInt;
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vInt = a.as.vInt * b.as.vInt;
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vInt = a.as.vInt / b.as.vInt;
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vInt = a.as.vInt % b.as.vInt;
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vBool = (a.as.vInt == b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vBool == b.as.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vInt == b.as.vInt);
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vBool = (a.as.vInt != b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vBool != b.as.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vInt == b.as.vInt);
    }
    else
    {
        assert(false && "Types not checked properly");
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
        rV.as.vBool = (a.as.vInt > b.as.vInt);
    }
    else
    {
        assert(false && "Invalid Types");
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
        rV.as.vBool = (a.as.vInt < b.as.vInt);
    }
    else
    {
        assert(false && "Invalid Types");
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
        rV.as.vBool = (a.as.vInt >= b.as.vInt);
    }
    else
    {
        assert(false && "Invalid Types");
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
        rV.as.vBool = (a.as.vInt <= b.as.vInt);
    }
    else
    {
        assert(false && "Invalid Types");
    }
}

void Value::Invert(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_BOOL)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

    rV.type = TYPE_BOOL;
    rV.as.vBool = !a.as.vBool;
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
        rV.as.vBool = (a.as.vInt & b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vBool && b.as.vBool);
    }
    else
    {
        assert(false && "Invalid Types");
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
        rV.as.vBool = (a.as.vInt | b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.type = TYPE_BOOL;
        rV.as.vBool = (a.as.vBool || b.as.vBool);
    }
    else
    {
        assert(false && "Invalid Types");
    }
}

void Value::Lnot(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);

    rV.type = TYPE_INT;
    rV.as.vInt = ~(a.as.vInt);
}
