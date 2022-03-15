#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

#include <cstdlib>
#include <cstring>

void runValueOperation(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
{
    Operands targetPair(a->type, b->type);

    const auto& operationMap = ValueOperations.find(op.code);

    if (operationMap != ValueOperations.end())
    {
        const auto& operation = (operationMap->second).find(targetPair);

        if (operation != (operationMap->second).end())
        {
            (*operation->second)(a, b, rV);
        }
    }
}

const std::unordered_map<OpCodeEnum, std::unordered_map<Operands, operationFunc, hashPair>> ValueOperations
{
    { 
        OP_ADD, 
        {
            { 
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) + get_vInt(b))); } 
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) + get_vMemPtr(b))); } 
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) + get_vInt(b))); } 
            },
            { 
                std::make_pair(TYPE_STRING, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vString(get_vString(a) + get_vInt(b))); } 
            },
            { 
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputs) { 
                    size_t newLength = get_vStringSize(a) + get_vStringSize(b);
                    char* newString = new char[newLength + 1];
                    strcpy(newString, get_vString(a));
                    strcpy(newString + get_vStringSize(a), get_vString(b));
                    newString[newLength] = 0;
                    rV = std::shared_ptr<Value>(new vString(newString)); 
                }
            }
        } 
    },
    {
        OP_SUBTRACT, 
        {
            { 
                std::make_pair(TYPE_INT, TYPE_INT), 
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) - get_vInt(b))); }
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vInt(b))); }
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vMemPtr(b))); }
            }
        }
    },
    {
        OP_MULTIPLY,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) * get_vInt(b))); }
            }
        }
    },
    {
        OP_DIVIDE,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) / get_vInt(b))); }
            }
        }
    },
    {
        OP_MOD,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) % get_vInt(b))); }
            }
        }
    },


    {
        OP_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vBool(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_INT, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vBool(b))); }
            },
            {
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(strcmp(get_vString(a), get_vString(b)) == 0)); }
            },
            {
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) == get_vMemPtr(b))); }
            },
        }
    },

    {
        OP_NOT_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vBool(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_INT, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vBool(b))); }
            },
            {
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(strcmp(get_vString(a), get_vString(b)) != 0)); }
            },
            {
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) != get_vMemPtr(b))); }
            },
        }
    },

    {
        OP_GREATER,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) > get_vInt(b))); }
            }
        }
    },
    {
        OP_LESS,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) < get_vInt(b))); }
            }
        }
    },
    {
        OP_GREATER_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) >= get_vInt(b))); }
            }
        }
    },
    {
        OP_LESS_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vInt(a) <= get_vInt(b))); }
            }
        }
    },

    {
        OP_LAND,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) & get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) && get_vBool(b))); }
            }
        }
    },
    {
        OP_LOR,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) | get_vInt(b))); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vBool(get_vBool(a) || get_vBool(b))); }
            }
        }
    },
    {
        OP_RSHIFT,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) >> get_vInt(b))); }
            }
        }
    },
    {
        OP_LSHIFT,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputs) { rV = std::shared_ptr<Value>(new vInt(get_vInt(a) << get_vInt(b))); }
            }
        }
    }
};

// void value_subtract(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//         Error::runtimeError(op, "Invalid Type. %s or %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[TYPE_MEM_PTR], ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);
//     }

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) - get_vInt(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vInt(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR && b->type == TYPE_MEM_PTR)
//     {
//         rV = std::shared_ptr<Value>(new vMemPtr(get_vMemPtr(a) - get_vMemPtr(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }

// void value_multiply(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) * get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not Checked properly");
//     }
// }

// void value_divide(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) / get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }

// void value_mod(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) % get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }


// void value_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//         case TYPE_STRING:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//         case TYPE_STRING:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
//     }

//     if (a->type != b->type)
//     {
//         if (a->type == TYPE_MEM_PTR || b->type == TYPE_MEM_PTR)
//             Error::runtimeError(op, "Memptr can only be compared to itself");
//     }

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vBool(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) == get_vInt(b)));
//     }
//     else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) == get_vBool(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR) // Both are equal
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) == get_vMemPtr(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_not_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//         case TYPE_MEM_PTR:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
//     }

//     if (a->type != b->type)
//     {
//         if (a->type == TYPE_MEM_PTR || b->type == TYPE_MEM_PTR)
//             Error::runtimeError(op, "Memptr can only be compared to itself");
//     }

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vBool(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) != get_vInt(b)));
//     }
//     else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) != get_vBool(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR) // Both are equal
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vMemPtr(a) != get_vMemPtr(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_greater(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) > get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_less(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) < get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_greater_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) >= get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_less_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vInt(a) <= get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

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

// void value_land(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
//     }

//     if (a->type != b->type)
//         Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a->type], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) & get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) && get_vBool(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_lor(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//         case TYPE_BOOL:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);
//     }

//     if (a->type != b->type)
//         Error::runtimeError(op, "Types were expected to be equal but found %s and %s instead", ValueTypeString[a->type], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = std::shared_ptr<Value>(new vInt(get_vInt(a) | get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = std::shared_ptr<Value>(new vBool(get_vBool(a) || get_vBool(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_rshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     }

//     rV = std::shared_ptr<Value>(new vInt(get_vInt(a) >> get_vInt(b)));
// }

// void value_lshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op)
// {
//     switch (a->type)
//     {
//         case TYPE_INT:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     }

//     switch (b->type)
//     {
//         case TYPE_INT:
//             break;
//         default:
//             Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     }

//     rV = std::shared_ptr<Value>(new vInt(get_vInt(a) << get_vInt(b)));
// }
