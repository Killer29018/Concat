#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"

void vChar::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v == get_vChar(v2)); break;

    default:
        Error::operationError(op, "Equal", type, v2->type);
    }
}

void vChar::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v != get_vChar(v2)); break;

    default:
        Error::operationError(op, "Not Equal", type, v2->type);
    }
}

void vChar::greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v > get_vChar(v2)); break;

    default:
        Error::operationError(op, "Greater", type, v2->type);
    }
}

void vChar::less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v < get_vChar(v2)); break;

    default:
        Error::operationError(op, "Less", type, v2->type);
    }
}

void vChar::greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v >= get_vChar(v2)); break;

    default:
        Error::operationError(op, "Greater Equal", type, v2->type);
    }
}

void vChar::lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v <= get_vChar(v2)); break;

    default:
        Error::operationError(op, "Less Equal", type, v2->type);
    }
}

