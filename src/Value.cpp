#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

#include <cstdlib>
#include <cstring>

bool CallFunction(const std::pair<ValueType, ValueType>& target, std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, std::unordered_map<std::pair<ValueType, ValueType>, OperationFunc> map)
{
    const auto& output = map.find(target);

    if (output != map.end())
    {
        (*output->second)(a, b, rV);
        return true;
    }
    return false;
}

void runValueOperation(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    std::pair<ValueType, ValueType> targetPair(a->type, b->type);

    switch (op.code)
    {
    case OP_ADD:
        {
            CallFunction(targetPair, a, b, rV, ValueAddition);
            break;
        }
    case OP_SUBTRACT:
        {
            CallFunction(targetPair, a, b, rV, ValueSubtraction);
            break;
        }
    case OP_MULTIPLY:
        {
            CallFunction(targetPair, a, b, rV, ValueMultiply);
            break;
        }
    case OP_DIVIDE:
        {
            CallFunction(targetPair, a, b, rV, ValueDivide);
            break;
        }


    default:
        assert(false && "Not implemented");
    }
}

const std::unordered_map<std::pair<ValueType, ValueType>, operationFunc, hashPair> ValueAddition
{
    { std::make_pair(TYPE_INT, TYPE_INT),           [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) + get_vInt(b))); } },
    { std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),   [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) + get_vMemPtr(b))); } },
    { std::make_pair(TYPE_MEM_PTR, TYPE_INT),       [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) + get_vInt(b))); } },
    { std::make_pair(TYPE_STRING, TYPE_INT),        [](operationInputs) { rV = std::shared_ptr<Value>(new vString(get_vString(a) + get_vInt(b))); } },
    { std::make_pair(TYPE_STRING, TYPE_STRING),     [](operationInputs) 
        { 
            size_t newLength = get_vStringSize(a) + get_vStringSize(b);
            char* newString = new char[newLength + 1];
            strcpy(newString, get_vString(a));
            strcpy(newString + get_vStringSize(a), get_vString(b));
            newString[newLength] = 0;
            rV = std::shared_ptr<Value>(new vString(newString)); 
        } },
};

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
        case TYPE_STRING:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_STRING:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
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
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
        case TYPE_MEM_PTR:
            break;
        default:
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
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
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

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
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

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
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

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
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    if (b->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

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
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);

    rV = std::shared_ptr<Value>(new vBool(!get_vBool(a)));
}

void value_lnot(std::shared_ptr<Value> a, std::shared_ptr<Value>& rV, const OpCode& op)
{
    if (a->type != TYPE_INT)
        Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);

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
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
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
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
    }

    switch (b->type)
    {
        case TYPE_INT:
        case TYPE_BOOL:
            break;
        default:
            Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
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
