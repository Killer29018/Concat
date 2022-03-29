#include "Value.hpp"

#include "../OpCodes.hpp"
#include "../VM.hpp"

#include "../Error.hpp"

#include "../SmartPointer.hpp"

#include <cstdlib>
#include <cstring>

void runOperation(const SmartPointer& a, SmartPointer& rV, const OpCode& op)
{
    switch (op.code)
    {
    case OP_INVERT:
        a->invert(rV, op); break;
    case OP_LNOT:
        a->lnot(rV, op); break;

    default:
        assert(false && "Not possible");
    }
}

void runOperation(const SmartPointer& a, const SmartPointer& b, SmartPointer& rV, const OpCode& op)
{
    switch (op.code)
    {
    case OP_ADD: a->add(b, rV, op); break;
    case OP_SUBTRACT: a->subtract(b, rV, op); break;
    case OP_MULTIPLY: a->multiply(b, rV, op); break;
    case OP_DIVIDE: a->divide(b, rV, op); break;
    case OP_MOD: a->mod(b, rV, op); break;

    case OP_EQUAL: a->equal(b, rV, op); break;
    case OP_NOT_EQUAL: a->notEqual(b, rV, op); break;
    case OP_GREATER: a->greater(b, rV, op); break;
    case OP_LESS: a->less(b, rV, op); break;
    case OP_GREATER_EQUAL: a->greaterEqual(b, rV, op); break;
    case OP_LESS_EQUAL: a->lessEqual(b, rV, op); break;
    case OP_LAND: a->land(b, rV, op); break;
    case OP_LOR: a->lor(b, rV, op); break;
    case OP_RSHIFT: a->rShift(b, rV, op); break;
    case OP_LSHIFT: a->lShift(b, rV, op); break;

    default:
        assert(false && "Not possible");
    }
}

void Value::print(const OpCode& op) const
{
    Error::operationError(op, "Print", type);
}
 
void Value::dot(const OpCode& op) const
{
    Error::operationError(op, "Dot", type);
}


void Value::cast(SmartPointer& rV, const OpCode& op) const
{
    Error::castError(op, type);
}
 

void Value::add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Add", type);
}
 
void Value::subtract(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Subtract", type);
}
 
void Value::multiply(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Multiply", type);
}
 
void Value::divide(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Divide", type);
}
 
void Value::mod(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Mod", type);
}
 
void Value::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Equal", type);
}
 
void Value::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Not Equal", type);
}
 
void Value::greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Greater", type);
}
 
void Value::less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Less", type);
}
 
void Value::greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Greater Equal", type);
}
 
void Value::lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Less Equal", type);
}
 
void Value::land(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Land", type);
}
 
void Value::lor(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Lor", type);
}
 
void Value::rShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Right Shift", type);
}
 
void Value::lShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Left Shift", type);
}

void Value::invert(SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "Invert", type);
}
 
void Value::lnot(SmartPointer& rV, const OpCode& op) const
{
    Error::operationError(op, "lnot", type);
}
 
