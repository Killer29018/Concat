#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include <memory>

#include <vector>

#include <unordered_map>
#include <utility>
#include <functional>

struct OpCode;
enum OpCodeEnum : uint16_t;

enum ValueType
{
    TYPE_NULL,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_MEM_PTR,

    TYPE_IP_OFFSET,

    TYPE_COUNT
};

const std::vector<const char*> ValueTypeString
{
    "NULL",
    "INT",
    "BOOL",
    "CHAR",
    "STRING",
    "MEM_PTR",
    "IP_OFFSET",
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
#define as_vInt(val)        ((vInt*)(val.get()))
#define get_vInt(val)       (((vInt*)(val.get()))->v)

struct vBool : Value
{
    bool v;

    vBool(bool value) : Value(TYPE_BOOL), v(value) {}
};
#define as_vBool(val)       ((vBool*)(val.get()))
#define get_vBool(val)      (((vBool*)(val.get()))->v)

struct vChar : Value
{
    char v;

    vChar(char value) : Value(TYPE_CHAR), v(value) {}
};
#define as_vChar(val)       ((vChar*)(val.get()))
#define get_vChar(val)      (((vChar*)(val.get()))->v)

struct vString : Value
{
    char* v;
    vString(char* value) : Value(TYPE_STRING), v(value) {}
};
#define as_vString(val)    ((vString*)(val.get()))
#define get_vString(val)   (((vString*)(val.get()))->v)
#define get_vStringSize(val) (strlen(((vString*)(val.get()))->v))

struct vMemPtr : Value
{
    uint32_t v;

    vMemPtr(uint32_t value) : Value(TYPE_MEM_PTR), v(value) {}
};
#define as_vMemPtr(val)     ((vMemPtr*)(val.get()))
#define get_vMemPtr(val)    (((vMemPtr*)(val.get()))->v)

struct vIpOffset : Value
{
    int32_t v;

    vIpOffset(int32_t value) : Value(TYPE_IP_OFFSET), v(value) {}
};
#define as_vIpOffset(val)   ((vIpOffset*)(val.get()))
#define get_vIpOffset(val)  (((vIpOffset*)(val.get()))->v)

void runValueOperation(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);

// void value_add(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_subtract(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_multiply(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_divide(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_mod(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);

// void value_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_not_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_greater(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_less(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_greater_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_less_equal(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);

void value_invert(std::shared_ptr<Value> a, std::shared_ptr<Value>& rV, const OpCode& op);
void value_lnot(std::shared_ptr<Value> a, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_land(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_lor(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_rshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);
// void value_lshift(std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV, const OpCode& op);

typedef void (*operationFunc)(std::shared_ptr<Value>, std::shared_ptr<Value>, std::shared_ptr<Value>&);

struct hashPair
{
    template<class T, class U>
    size_t operator()(const std::pair<T, U> p) const
    {
        uintmax_t hash = std::hash<T>{}(p.first);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<U>{}(p.second);
        return std::hash<uintmax_t>{}(hash);
    }
};

#define operationInputs std::shared_ptr<Value> a, std::shared_ptr<Value> b, std::shared_ptr<Value>& rV
typedef std::pair<ValueType, ValueType> Operands;
extern const std::unordered_map<OpCodeEnum, std::unordered_map<Operands, operationFunc, hashPair>> ValueOperations;

#endif
