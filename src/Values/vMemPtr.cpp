#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"

void vMemPtr::print(const OpCode& op) const
{
    printf("%d", v);
}

void vMemPtr::add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vMemPtr>(v + get_vInt(v2)); break;

    default:
        Error::operationError(op, "Add", type, v2->type);
    }
}

void vMemPtr::subtract(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vMemPtr>(v - get_vInt(v2)); break;
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vMemPtr>(v - get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Subtract", type, v2->type);
    }
}

void vMemPtr::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v == get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Equal", type, v2->type);
    }
}

void vMemPtr::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v != get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Not Equal", type, v2->type);
    }
}

void vMemPtr::greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v > get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Greater", type, v2->type);
    }
}

void vMemPtr::less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v < get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Less", type, v2->type);
    }
}

void vMemPtr::greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v >= get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Greater Equal", type, v2->type);
    }
}

void vMemPtr::lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v <= get_vMemPtr(v2)); break;

    default:
        Error::operationError(op, "Less Equal", type, v2->type);
    }
}

