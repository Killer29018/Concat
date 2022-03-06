#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <cstdint>

#include <vector>

struct OpCode;

enum ValueType
{
    TYPE_NULL,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_MEM_PTR,

    TYPE_IP_OFFSET,

    TYPE_COUNT
};

struct Value
{
public:
    ValueType type;

    // Value* value;

    Value() = default;

    Value(ValueType type)
        : type(type) {}
};

struct vNull : Value
{
    vNull() : Value(TYPE_NULL) {}
};

struct vInt : Value
{
    int32_t v;

    vInt(int32_t value) : Value(TYPE_INT), v(value) {}
};

struct vBool : Value
{
    bool v;

    vBool(bool value) : Value(TYPE_BOOL), v(value) {}
};

struct vChar : Value
{
    char v;

    vChar(char value) : Value(TYPE_CHAR), v(value) {}
};

struct vMemPtr : Value
{
    uint32_t v;

    vMemPtr(uint32_t value) : Value(TYPE_MEM_PTR), v(value) {}
};

struct vIpOffset : Value
{
    int32_t v;

    vIpOffset(int32_t value) : Value(TYPE_IP_OFFSET), v(value) {}
};

#define as_vInt(val)        (((vInt*)(val))->v)
#define as_vBool(val)       (((vBool*)(val))->v)
#define as_vChar(val)       (((vChar*)(val))->v)
#define as_vMemPtr(val)     (((vMemPtr*)(val))->v)
#define as_vIpOffset(val)   (((vIpOffset*)(val))->v)

        // rV = vInt(((vInt*)a.value)->v + ((vInt*)b.value)->v);

void value_add(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_subtract(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_multiply(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_divide(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_mod(const Value* a, const Value* b, Value** rV, const OpCode& op);

void value_equal(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_not_equal(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_greater(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_less(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_greater_equal(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_less_equal(const Value* a, const Value* b, Value** rV, const OpCode& op);

void value_invert(const Value* a, Value** rV, const OpCode& op);
void value_lnot(const Value* a, Value** rV, const OpCode& op);
void value_land(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_lor(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_rshift(const Value* a, const Value* b, Value** rV, const OpCode& op);
void value_lshift(const Value* a, const Value* b, Value** rV, const OpCode& op);

const std::vector<const char*> ValueTypeString
{
    "NULL",
    "INT",
    "BOOL",
    "CHAR",
    "MEM_PTR",
    "IP_OFFSET",
};


#endif
