#include "Value.hpp"

#include "OpCodes.hpp"
#include "VirtualMachine.hpp"

#include "Error.hpp"

#include "SmartPointer.hpp"

#include <cstdlib>
#include <cstring>

void runValueOperationSingle(const SmartPointer& a, SmartPointer& rV, const OpCode& op)
{
    ValueType target = a->type;

    const auto& operationMap = ValueOperationSingle.find(op.code);

    if (operationMap != ValueOperationSingle.end())
    {
        const auto& operation = (operationMap->second).find(target);

        if (operation != (operationMap->second).end())
        {
            (*operation->second)(a, rV);
        }
        else
        {
            Error::runtimeError(op, "Type %s not supported for %s", ValueTypeString[a->type], OpCodeString[op.code]);
        }
    }
    else
    {
        assert(false && "Operation not supported");
    }
}

void runValueOperationDouble(const SmartPointer& a, const SmartPointer& b, SmartPointer& rV, const OpCode& op)
{
    Operands targetPair(a->type, b->type);

    const auto& operationMap = ValueOperationDouble.find(op.code);

    if (operationMap != ValueOperationDouble.end())
    {
        const auto& operation = (operationMap->second).find(targetPair);

        if (operation != (operationMap->second).end())
        {
            (*operation->second)(a, b, rV);
        }
        else
        {
            Error::runtimeError(op, "Types %s and %s not supported for %s", ValueTypeString[a->type], ValueTypeString[b->type], OpCodeString[op.code]);
        }
    }
    else
    {
        assert(false && "Operation not supported");
    }
}

const std::unordered_map<OpCodeEnum, std::unordered_map<ValueType, operationFuncSingle>> ValueOperationSingle
{
    {
        OP_INVERT,
        {
            {
                TYPE_BOOL,
                [](operationInputsSingle) { rV = SmartPointer(new vBool(!get_vBool(a))); }
            }
        }
    },
    {
        OP_LNOT,
        {
            {
                TYPE_INT,
                [](operationInputsSingle) { rV = SmartPointer(new vInt(~get_vInt(a))); }
            }
        }
    }
};

const std::unordered_map<OpCodeEnum, std::unordered_map<Operands, operationFuncDouble, hashPair>> ValueOperationDouble
{
    { 
        OP_ADD, 
        {
            { 
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) + get_vInt(b)); } 
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vMemPtr>(get_vMemPtr(a) + get_vInt(b)); } 
            },
            { 
                std::make_pair(TYPE_STRING, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vString>(get_vString(a) + get_vInt(b)); } 
            },
            { 
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputsDouble) { 
                    size_t newLength = get_vStringSize(a) + get_vStringSize(b);
                    char* newString = new char[newLength + 1];
                    strcpy(newString, get_vString(a));
                    strcpy(newString + get_vStringSize(a), get_vString(b));
                    newString[newLength] = 0;
                    rV = makeSmartPointer<vString>(newString); 
                }
            }
        } 
    },
    {
        OP_SUBTRACT, 
        {
            { 
                std::make_pair(TYPE_INT, TYPE_INT), 
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) - get_vInt(b)); }
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vMemPtr>(get_vMemPtr(a) - get_vInt(b)); }
            },
            { 
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputsDouble) { rV = makeSmartPointer<vMemPtr>(get_vMemPtr(a) - get_vMemPtr(b)); }
            }
        }
    },
    {
        OP_MULTIPLY,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) * get_vInt(b)); }
            }
        }
    },
    {
        OP_DIVIDE,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) / get_vInt(b)); }
            }
        }
    },
    {
        OP_MOD,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) % get_vInt(b)); }
            }
        }
    },


    {
        OP_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) == get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) == get_vBool(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) == get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_INT, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) == get_vBool(b)); }
            },
            {
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(strcmp(get_vString(a), get_vString(b)) == 0); }
            },
            {
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vMemPtr(a) == get_vMemPtr(b)); }
            },
        }
    },

    {
        OP_NOT_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) != get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) != get_vBool(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) != get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_INT, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) != get_vBool(b)); }
            },
            {
                std::make_pair(TYPE_STRING, TYPE_STRING),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(strcmp(get_vString(a), get_vString(b)) != 0); }
            },
            {
                std::make_pair(TYPE_MEM_PTR, TYPE_MEM_PTR),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vMemPtr(a) != get_vMemPtr(b)); }
            },
        }
    },

    {
        OP_GREATER,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) > get_vInt(b)); }
            }
        }
    },
    {
        OP_LESS,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) < get_vInt(b)); }
            }
        }
    },
    {
        OP_GREATER_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) >= get_vInt(b)); }
            }
        }
    },
    {
        OP_LESS_EQUAL,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vInt(a) <= get_vInt(b)); }
            }
        }
    },

    {
        OP_LAND,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) & get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) && get_vBool(b)); }
            }
        }
    },
    {
        OP_LOR,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) | get_vInt(b)); }
            },
            {
                std::make_pair(TYPE_BOOL, TYPE_BOOL),
                [](operationInputsDouble) { rV = makeSmartPointer<vBool>(get_vBool(a) || get_vBool(b)); }
            }
        }
    },
    {
        OP_RSHIFT,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) >> get_vInt(b)); }
            }
        }
    },
    {
        OP_LSHIFT,
        {
            {
                std::make_pair(TYPE_INT, TYPE_INT),
                [](operationInputsDouble) { rV = makeSmartPointer<vInt>(get_vInt(a) << get_vInt(b)); }
            }
        }
    }
};

// void value_subtract(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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
//         rV = SmartPointer(new vInt(get_vInt(a) - get_vInt(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vMemPtr(get_vMemPtr(a) - get_vInt(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR && b->type == TYPE_MEM_PTR)
//     {
//         rV = SmartPointer(new vMemPtr(get_vMemPtr(a) - get_vMemPtr(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }

// void value_multiply(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vInt(get_vInt(a) * get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not Checked properly");
//     }
// }

// void value_divide(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vInt(get_vInt(a) / get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }

// void value_mod(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type. %s was expected but found %s instead", ValueTypeString[TYPE_INT], ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vInt(get_vInt(a) % get_vInt(b)));
//     }
//     else
//     {
//         assert(false && "Not checked properly");
//     }
// }


// void value_equal(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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
//         rV = SmartPointer(new vBool(get_vInt(a) == get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) == get_vBool(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) == get_vInt(b)));
//     }
//     else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) == get_vBool(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR) // Both are equal
//     {
//         rV = SmartPointer(new vBool(get_vMemPtr(a) == get_vMemPtr(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_not_equal(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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
//         rV = SmartPointer(new vBool(get_vInt(a) != get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) != get_vBool(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) != get_vInt(b)));
//     }
//     else if (a->type == TYPE_INT && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) != get_vBool(b)));
//     }
//     else if (a->type == TYPE_MEM_PTR) // Both are equal
//     {
//         rV = SmartPointer(new vBool(get_vMemPtr(a) != get_vMemPtr(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_greater(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) > get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_less(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) < get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_greater_equal(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) >= get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_less_equal(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);
//     if (b->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[b->type]);

//     if (a->type == TYPE_INT && b->type == TYPE_INT)
//     {
//         rV = SmartPointer(new vBool(get_vInt(a) <= get_vInt(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_invert(SmartPointer a, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_BOOL)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);

//     rV = SmartPointer(new vBool(!get_vBool(a)));
// }

// void value_lnot(SmartPointer a, SmartPointer& rV, const OpCode& op)
// {
//     if (a->type != TYPE_INT)
//         Error::runtimeError(op, "Invalid Type %s", ValueTypeString[a->type]);

//     rV = SmartPointer(new vInt(~get_vInt(a)));
// }

// void value_land(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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
//         rV = SmartPointer(new vInt(get_vInt(a) & get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) && get_vBool(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_lor(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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
//         rV = SmartPointer(new vInt(get_vInt(a) | get_vInt(b)));
//     }
//     else if (a->type == TYPE_BOOL && b->type == TYPE_BOOL)
//     {
//         rV = SmartPointer(new vBool(get_vBool(a) || get_vBool(b)));
//     }
//     else
//     {
//         Error::runtimeError(op, "Invalid Types");
//     }
// }

// void value_rshift(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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

//     rV = SmartPointer(new vInt(get_vInt(a) >> get_vInt(b)));
// }

// void value_lshift(SmartPointer a, SmartPointer b, SmartPointer& rV, const OpCode& op)
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

//     rV = SmartPointer(new vInt(get_vInt(a) << get_vInt(b)));
// }
