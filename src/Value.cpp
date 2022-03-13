#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

#include <cstdlib>
#include <cstring>

// TODO: Type checking doesn't work if types are different
// TODO: Return Type should be set within the operations themselves

void value_add(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_STRING:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s %s or %s was expected but found %s instead",
                    ValueTypeString[TYPE_INT],
                    ValueTypeString[TYPE_STRING],
                    ValueTypeString[TYPE_MEM_PTR],
                    ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
        case TYPE_STRING:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s %s or %s was expected but found %s instead",
                    ValueTypeString[TYPE_INT],
                    ValueTypeString[TYPE_STRING],
                    ValueTypeString[TYPE_MEM_PTR],
                    ValueTypeString[b->type]);
    }

    if (a->type == b->type)
    {
        if (a->type == TYPE_MEM_PTR)
            Error::runtimeError(op, "Can not add two %s together", ValueTypeString[TYPE_MEM_PTR]);
    }

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) + get_vInt(b)));
    }
    // else if ((a->type == TYPE_MEM_PTR && b->type == TYPE_INT) ||
    //          (a->type == TYPE_INT && b->type == TYPE_MEM_PTR))
    else if (a->type == TYPE_MEM_PTR && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) + get_vInt(b)));
    }
    else if (a->type == TYPE_INT && b->type == TYPE_MEM_PTR)
    {
        rV = std::shared_ptr<Value>(new vMemPtr(get_vInt(a) + get_vMemPtr(b)));
    }
    else if (a->type == TYPE_STRING && b->type == TYPE_INT)
    {
        size_t offset = get_vInt(b);
        char* value = get_vString(a);

        rV = std::shared_ptr<Value>(new vString((value + offset)));
    }
    else if(a->type == TYPE_STRING && b->type == TYPE_STRING)
    {
        size_t newLength = get_vStringSize(a) + get_vStringSize(b) + 1;
        char* newString = new char[newLength];
        newString[newLength - 1] = 0x00;

        strcpy(newString, get_vString(a));
        strcpy(newString + get_vStringSize(a), get_vString(b));

        rV = std::shared_ptr<Value>(new vString(newString));
    }
    else
    {
        Error::runtimeError(op, "Invalid types");
    }
}

void value_subtract(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_MEM_PTR:
            break;
        default:
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);
    }

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) - get_vInt(b)));
    }
    else if (a->type == TYPE_MEM_PTR && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vInt(b)));
    }
    else if (a->type == TYPE_MEM_PTR && b->type == TYPE_MEM_PTR)
    {
        rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vMemPtr(b)));
    }
    else
    {
        assert(false && "Not checked properly");
    }
}

void value_multiply(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) * get_vInt(b)));
    }
    else
    {
        assert(false && "Not Checked properly");
    }
}

void value_divide(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) / get_vInt(b)));
    }
    else
    {
        assert(false && "Not checked properly");
    }
}

void value_mod(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) % get_vInt(b)));
    }
    else
    {
        assert(false && "Not checked properly");
    }
}


void value_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b->type]);
    }

    if (a->type != b->type)
    {
        if (a->type == TYPE_MEM_PTR || b->type == TYPE_MEM_PTR)
            Error::runtimeError(op, "Memptr can only be compared to itself");
    }

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vInt(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vBool(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vInt(b)));
    }
    else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vBool(b)));
    }
    else if (a->type == TYPE_MEM_PTR) // Both are equal
    {
        rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) == get_vMemPtr(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_not_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b->type]);
    }

    if (a->type != b->type)
    {
        if (a->type == TYPE_MEM_PTR || b->type == TYPE_MEM_PTR)
            Error::runtimeError(op, "Memptr can only be compared to itself");
    }

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vInt(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vBool(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vInt(b)));
    }
    else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vBool(b)));
    }
    else if (a->type == TYPE_MEM_PTR) // Both are equal
    {
        rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) != get_vMemPtr(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_greater(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) > get_vInt(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_less(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) < get_vInt(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_greater_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) >= get_vInt(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_less_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vInt(a) <= get_vInt(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_invert(std::shared_ptr<Value> a, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_BOOL)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);

    rV = std::shared_ptr<Value>(new vBool(!get_vBool(a)));
}

void value_lnot(std::shared_ptr<Value> a, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);

    rV = std::shared_ptr<Value>(new vInt(~get_vInt(a)));
}

void value_land(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b->type]);
    }

    if (a->type != b->type)
        Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a->type], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) & get_vInt(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) && get_vBool(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_lor(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_BOOL], ValueTypeString[b->type]);
    }

    if (a->type != b->type)
        Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a->type], ValueTypeString[b->type]);

    if (a->type == TYPE_INT && b->type == TYPE_INT)
    {
        rV = std::shared_ptr<Value>(new vInt(get_vInt(a) | get_vInt(b)));
    }
    else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
    {
        rV = std::shared_ptr<Value>(new vBool(get_vBool(a) || get_vBool(b)));
    }
    else
    {
        Error::runtimeError(op, "Invalid Types");
    }
}

void value_rshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    }

    rV = std::shared_ptr<Value>(new vInt(get_vInt(a) >> get_vInt(b)));
}

void value_lshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    switch (a->type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
            break;
        default:
            Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
    }

    rV = std::shared_ptr<Value>(new vInt(get_vInt(a) << get_vInt(b)));
}
