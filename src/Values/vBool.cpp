#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

void vBool::print(const OpCode& op) const
{
    printf("%d", v);
}

void vBool::dot(const OpCode& op) const
{
    printf("%i", v);
}


void vBool::cast(SmartPointer& rV, const OpCode& op) const
{
    switch (op.value->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>((int)v); break;

    default:
        Error::castError(op, type);
    }
}


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


size_t vBool::getSize() const
{
    return enumSize + sizeof(bool);
}

void vBool::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, v, sizeof(v));
}

void vBool::readBuffer(std::ifstream& file, OpCode& code)
{
    bool value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vBool>(value);

    delete buffer;
}
