#include "Value.hpp"
#include "../SmartPointer.hpp"

void vInt::add(const SmartPointer& v2, SmartPointer& rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v + get_vInt(v2)); break;
    }
}

void vInt::subtract(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v - get_vInt(v2)); break;
    }
}

void vInt::multiply(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v * get_vInt(v2)); break;
    }
}

void vInt::divide(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v / get_vInt(v2)); break;
    }
}

void vInt::mod(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v % get_vInt(v2)); break;
    }
}

void vInt::equal(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v == get_vInt(v2)); break;
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v == get_vBool(v2)); break;
    }
}

void vInt::notEqual(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v != get_vInt(v2)); break;
    case TYPE_BOOL:
        rV = makeSmartPointer<vBool>(v != get_vBool(v2)); break;
    }
}

void vInt::greater(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v > get_vInt(v2)); break;
    }
}

void vInt::less(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v < get_vInt(v2)); break;
    }
}

void vInt::greaterEqual(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v >= get_vInt(v2)); break;
    }
}

void vInt::lessEqual(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vBool>(v <= get_vInt(v2)); break;
    }
}

void vInt::land(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v & get_vInt(v2)); break;
    }
}

void vInt::lor(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v | get_vInt(v2)); break;
    }
}

void vInt::rShift(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v >> get_vInt(v2)); break;
    }
}
void vInt::lShift(const SmartPointer &v2, SmartPointer &rV) const
{
    switch (v2->type)
    {
    case TYPE_INT:
        rV = makeSmartPointer<vInt>(v << get_vInt(v2)); break;
    }
}

void vInt::lnot(SmartPointer& rV) const
{
    rV = makeSmartPointer<vInt>(~v);
}
