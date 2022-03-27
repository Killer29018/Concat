#include "Value.hpp"
#include "../SmartPointer.hpp"

void vChar::equal(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v == get_vChar(v2)); break;
    }
}

void vChar::notEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v != get_vChar(v2)); break;
    }
}

void vChar::greater(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v > get_vChar(v2)); break;
    }
}

void vChar::less(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v < get_vChar(v2)); break;
    }
}

void vChar::greaterEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v >= get_vChar(v2)); break;
    }
}

void vChar::lessEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_CHAR:
        rV = makeSmartPointer<vChar>(v <= get_vChar(v2)); break;
    }
}

