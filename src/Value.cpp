#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

// TODO: Type checking doesn't work if types are different
// TODO: Return Type should be set within the operations themselves

void Value::add(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[b.type]);
    }

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.as.vInt = a.as.vInt + b.as.vInt;
    }
    else if ((a.type == TYPE_MEM_PTR && b.type == TYPE_INT) ||
             (a.type == TYPE_INT && b.type == TYPE_MEM_PTR))
    {
        rV.type = TYPE_MEM_PTR;
        rV.as.vInt = a.as.value + b.as.value;
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::subtract(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[b.type]);
    }

    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.type = TYPE_INT;
        rV.as.vInt = a.as.vInt - b.as.vInt;
    }
    else if ((a.type == TYPE_MEM_PTR && b.type == TYPE_INT) ||
             (a.type == TYPE_INT && b.type == TYPE_MEM_PTR))
    {
        rV.type = TYPE_MEM_PTR;
        rV.as.vInt = a.as.value - b.as.value;
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::multiply(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::divide(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::mod(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}


void Value::equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    if (a.type != b.type)
    {
        if (a.type == TYPE_MEM_PTR || b.type == TYPE_MEM_PTR)
            Error::runtimeError(op, "Memptr can only be compared to itself");
    }

    rV.type = TYPE_BOOL;
    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.as.vBool = (a.as.vInt == b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.as.vBool = (a.as.vBool == b.as.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.as.vBool = (a.as.value == b.as.value);
    }
    else if (a.type == TYPE_MEM_PTR) // Both are equal
    {
        rV.as.vBool = (a.as.vMemPtr == b.as.vMemPtr);
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::not_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b.type]);
    }

    if (a.type != b.type)
    {
        if (a.type == TYPE_MEM_PTR || b.type == TYPE_MEM_PTR)
            Error::runtimeError(op, "Memptr can only be compared to itself");
    }

    rV.type = TYPE_BOOL;
    if (a.type == TYPE_INT && b.type == TYPE_INT)
    {
        rV.as.vBool = (a.as.vInt != b.as.vInt);
    }
    else if (a.type == TYPE_BOOL && b.type == TYPE_BOOL)
    {
        rV.as.vBool = (a.as.vBool != b.as.vBool);
    }
    else if ((a.type == TYPE_BOOL || b.type == TYPE_BOOL) && 
             (a.type == TYPE_INT  || b.type == TYPE_INT ))
    {
        rV.as.vBool = (a.as.value != b.as.value);
    }
    else if (a.type == TYPE_MEM_PTR) // Types are equal
    {
         rV.as.vBool = (a.as.vMemPtr != b.as.vMemPtr);
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::greater(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::less(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::greater_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::less_equal(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::invert(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_BOOL)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a.type]);

    rV.type = TYPE_BOOL;
    rV.as.vBool = !a.as.vBool;
}

void Value::lnot(const Value& a, Value& rV, const OpCode& op)
{
    if (a.type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);

    rV.type = TYPE_INT;
    rV.as.vInt = ~(a.as.vInt);
}

void Value::land(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::lor(const Value& a, const Value& b, Value& rV, const OpCode& op)
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
        Error::runtimeError(op, "Invalid Types");
    }
}

void Value::rshift(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    }

    rV.type = TYPE_INT;
    rV.as.vInt = a.as.vInt >> b.as.vInt;
}

void Value::lshift(const Value& a, const Value& b, Value& rV, const OpCode& op)
{
    switch (a.type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    }

    switch (b.type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a.type]);
    }

    rV.type = TYPE_INT;
    rV.as.vInt = a.as.vInt << b.as.vInt;
}
