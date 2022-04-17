#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

void vChar::print(const OpCode& op) const
{
    printf("%c", v);
}

void vChar::dot(const OpCode& op) const
{
    printf("%d", v);
}


void vChar::cast(SmartPointer& rV, const OpCode& op) const
{
    switch (op.value->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>((int)v); break;
    case TYPE_STRING:
        rV = makeSmartPointer<vString>(new char(v)); break;

    default:
        Error::castError(op, type);
    }
}


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


size_t vChar::getSize() const
{
    return enumSize + sizeof(char); 
}

void vChar::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, v, sizeof(v));
}

void vChar::readBuffer(std::ifstream& file, OpCode& code)
{
    char value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vChar>(value);

    delete buffer;
}
