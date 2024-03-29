#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

#include <cstring>

void vString::print(const OpCode& op) const
{
    printf("%s", v);
}

void vString::dot(const OpCode& op) const
{
    printf("%c", v[0]);
}


void vString::cast(SmartPointer& rV, const OpCode& op) const
{
    switch (op.value->type)
    {
    case TYPE_INT:
        {
            char* end;
            strtol(v, &end, 10);

            if (end != v + strlen(v))
                Error::runtimeError(op, "Failed to cast %s to int", v);
            
            rV = makeSmartPointer<vInt>(atoi(v));
            break;
        }
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v[0]); break;

    default:
        Error::castError(op, type);
    }
}


void vString::add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vString>(v + get_vInt(v2)); break;
    case TYPE_CHAR:
        {
            size_t newLength = strlen(v) + 1;
            char* newString = new char[newLength + 1];
            strcpy(newString, v);
            newString[newLength - 1] = get_vChar(v2);
            newString[newLength] = 0;
            rV = makeSmartPointer<vString>(newString);

            break;
        }
    case TYPE_STRING:
        {
            size_t newLength = strlen(v) + get_vStringSize(v2);
            char* newString = new char[newLength + 1];
            strcpy(newString, v);
            strcpy(newString + strlen(v), get_vString(v2));
            newString[newLength] = 0;
            rV = makeSmartPointer<vString>(newString); 

            break;
        }

    default:
        Error::operationError(op, "Add", type, v2->type);
    }
}

void vString::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_STRING:
        rV = makeSmartPointer<vBool>(strcmp(v, get_vString(v2)) == 0); break;

    default:
        Error::operationError(op, "Equal", type, v2->type);
    }
}

void vString::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_STRING:
        rV = makeSmartPointer<vBool>(strcmp(v, get_vString(v2)) != 0); break;

    default:
        Error::operationError(op, "Not Equal", type, v2->type);
    }
}

size_t get_vStringSize(const SmartPointer& val)
{
    vString* value = as_vString(val);
    if (value->v)
        return strlen(value->v);
    return 0;
}

size_t get_vStringSize(const vString* val)
{
    if (val->v)
        return strlen(val->v);
    return 0;
}


size_t vString::getSize() const
{
    return enumSize + size_tSize + get_vStringSize(this);
}

void vString::writeBuffer(char* buffer, size_t& index) const
{
    size_t size = get_vStringSize(this);

    Builder::addElement(buffer, index, size, size_tSize);
    strncpy(buffer + index, v, size);

    index += size;
}

void vString::readBuffer(std::ifstream& file, OpCode& code)
{
    size_t size = sizeof(char) * size_tSize;
    char* buffer = (char*)malloc(size);
    file.read(buffer, size);

    size_t length;
    Builder::readElement(buffer, length, size_tSize);

    char* value = (char*)malloc(length + 1);
    file.read(value, length);
    value[length] = 0;

    code.value = makeSmartPointer<vString>(value);

    delete buffer;
}
