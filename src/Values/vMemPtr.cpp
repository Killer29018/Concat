#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

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


size_t vMemPtr::getSize() const
{
    return enumSize + sizeof(uint32_t);
}

void vMemPtr::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, v, sizeof(v));
}

void vMemPtr::readBuffer(std::ifstream& file, OpCode& code)
{
    uint32_t value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vMemPtr>(value);

    delete buffer;
}
