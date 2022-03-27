#include "Value.hpp"
#include "../SmartPointer.hpp"

void vMemPtr::add(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vMemPtr>(v + get_vInt(v2)); break;
    }
}

void vMemPtr::subtract(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vMemPtr>(v - get_vInt(v2)); break;
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vMemPtr>(v - get_vMemPtr(v2)); break;
    }
}

void vMemPtr::equal(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v == get_vMemPtr(v2)); break;
    }
}

void vMemPtr::notEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v != get_vMemPtr(v2)); break;
    }
}

void vMemPtr::greater(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v > get_vMemPtr(v2)); break;
    }
}

void vMemPtr::less(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v < get_vMemPtr(v2)); break;
    }
}

void vMemPtr::greaterEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v >= get_vMemPtr(v2)); break;
    }
}

void vMemPtr::lessEqual(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_MEM_PTR:
        rV = makeSmartPointer<vBool>(v <= get_vMemPtr(v2)); break;
    }
}

