#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

#include <cstring>

void vInt::print(const OpCode& op) const
{
    printf("%d", v);
}

void vInt::dot(const OpCode& op) const
{
    printf("%c", v);
}


void vInt::cast(SmartPointer& rV, const OpCode& op) const
{
    switch (op.value->type)
    {
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>((bool)v); break;
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>((char)v); break;
    case TYPE_STRING:
        {
            std::string tmp = std::to_string(v);
            char* newString = new char[tmp.length() + 1];

            strcpy(newString, tmp.c_str());
            newString[tmp.length()] = 0x00;
            rV = makeSmartPointer<vString>(newString);

            break;
        }

    default:
        Error::castError(op, type);
    }
}


void vInt::add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v + get_vInt(v2)); break;

    default:
        Error::operationError(op, "Add", type, v2->type);
    }
}

void vInt::subtract(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v - get_vInt(v2)); break;

    default:
        Error::operationError(op, "Subtract", type, v2->type);
    }
}

void vInt::multiply(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v * get_vInt(v2)); break;

    default:
        Error::operationError(op, "Multiply", type, v2->type);
    }
}

void vInt::divide(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v / get_vInt(v2)); break;

    default:
        Error::operationError(op, "Divide", type, v2->type);
    }
}

void vInt::mod(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v % get_vInt(v2)); break;

    default:
        Error::operationError(op, "Mod", type, v2->type);
    }
}

void vInt::equal(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
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

void vInt::notEqual(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
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

void vInt::greater(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v > get_vInt(v2)); break;

    default:
        Error::operationError(op, "Greater", type, v2->type);
    }
}

void vInt::less(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v < get_vInt(v2)); break;

    default:
        Error::operationError(op, "Less", type, v2->type);
    }
}

void vInt::greaterEqual(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v >= get_vInt(v2)); break;

    default:
        Error::operationError(op, "Greater Equal", type, v2->type);
    }
}

void vInt::lessEqual(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v <= get_vInt(v2)); break;

    default:
        Error::operationError(op, "Less Equal", type, v2->type);
    }
}

void vInt::land(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v & get_vInt(v2)); break;

    default:
        Error::operationError(op, "Land", type, v2->type);
    }
}

void vInt::lor(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v | get_vInt(v2)); break;

    default:
        Error::operationError(op, "Lor", type, v2->type);
    }
}

void vInt::rShift(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v >> get_vInt(v2)); break;

    default:
        Error::operationError(op, "Right Shift", type, v2->type);
    }
}
void vInt::lShift(const SmartPointer &v2, SmartPointer &rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v << get_vInt(v2)); break;

    default:
        Error::operationError(op, "Left Shift", type, v2->type);
    }
}

void vInt::lnot(SmartPointer& rV, const OpCode& op) const
{
    rV = makeSmartPointer<vInt>(~v);
}

size_t vInt::getSize() const
{
    return enumSize + sizeof(int32_t);
}

void vInt::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, v, sizeof(v));
}

void vInt::readBuffer(std::ifstream& file, OpCode& code)
{
    int32_t value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vInt>(value);

    delete buffer;
}
