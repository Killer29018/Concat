#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"

void vBool::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const 
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v == get_vInt(v2)); break;
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v == get_vBool(v2)); break;

    default:
        Error::operationError(op, "Equal", type, v2->type);
    }
}

void vBool::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const 
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v != get_vInt(v2)); break;
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v != get_vBool(v2)); break;

    default:
        Error::operationError(op, "Not Equal", type, v2->type);
    }
}

void vBool::land(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const 
{
    switch (v2->type)
    {
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v && get_vBool(v2)); break;

    default:
        Error::operationError(op, "Land", type, v2->type);
    }
}

void vBool::lor(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const 
{
    switch (v2->type)
    {
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v || get_vBool(v2)); break;

    default:
        Error::operationError(op, "Lor", type, v2->type);
    }
}


void vBool::invert(SmartPointer& rV, const OpCode& op) const 
{
    rV = makeSmartPointer<vBool>(!v);
}

