#include "Value.hpp"
#include "../SmartPointer.hpp"

#include <cstring>

void vString::add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vString>(v + get_vInt(v2)); break;
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
    }
}

void vString::equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_STRING:
        rV = makeSmartPointer<vBool>(strcmp(v, get_vString(v2)) == 0); break;
    }
}

void vString::notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const
{
    switch (v2->type)
    {
    case TYPE_STRING:
        rV = makeSmartPointer<vBool>(strcmp(v, get_vString(v2)) != 0); break;
    }
}

