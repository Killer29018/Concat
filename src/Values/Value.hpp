#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include <vector>

#include <unordered_map>
#include <utility>
#include <functional>


class SmartPointer;

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

    virtual ~Value() {}

    virtual void print(const OpCode& op) const;
    virtual void dot(const OpCode& op) const;

    virtual void cast(SmartPointer& rV, const OpCode& op) const;

    virtual void add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void subtract(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void multiply(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void divide(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void mod(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void land(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void lor(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void rShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;
    virtual void lShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const;

    virtual void invert(SmartPointer& rV, const OpCode& op) const;
    virtual void lnot(SmartPointer& rV, const OpCode& op) const;
};

struct vNull : Value
{
    vNull() : Value(TYPE_NULL) {}
};

struct vInt : Value
{
    int32_t v;

    vInt(int32_t value) : Value(TYPE_INT), v(value) {}

    void print(const OpCode& op) const override;
    void dot(const OpCode& op) const override;

    void cast(SmartPointer& rV, const OpCode& op) const override;

    void add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void subtract(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void multiply(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void divide(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void mod(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void land(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lor(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void rShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lShift(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;

    void lnot(SmartPointer& rV, const OpCode& op) const override;
};
#define as_vInt(val)        ((vInt*)(val.get()))
#define get_vInt(val)       (((vInt*)(val.get()))->v)

struct vBool : Value
{
    bool v;

    vBool(bool value) : Value(TYPE_BOOL), v(value) {}

    void print(const OpCode& op) const override;
    void dot(const OpCode& op) const override;

    void cast(SmartPointer& rV, const OpCode& op) const override;

    void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void land(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lor(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;

    void invert(SmartPointer& rV, const OpCode& op) const override;
};
#define as_vBool(val)       ((vBool*)(val.get()))
#define get_vBool(val)      (((vBool*)(val.get()))->v)

struct vChar : Value
{
    char v;

    vChar(char value) : Value(TYPE_CHAR), v(value) {}

    void print(const OpCode& op) const override;
    void dot(const OpCode& op) const override;

    void cast(SmartPointer& rV, const OpCode& op) const override;

    void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
};
#define as_vChar(val)       ((vChar*)(val.get()))
#define get_vChar(val)      (((vChar*)(val.get()))->v)

struct vString : Value
{
    char* v;
    vString(char* value) : Value(TYPE_STRING), v(value) {}
    ~vString() { }

    void print(const OpCode& op) const override;
    void dot(const OpCode& op) const override;

    void cast(SmartPointer& rV, const OpCode& op) const override;

    void add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
};
#define as_vString(val)    ((vString*)(val.get()))
#define get_vString(val)   (((vString*)(val.get()))->v)
#define get_vStringSize(val) (strlen(((vString*)(val.get()))->v))

struct vMemPtr : Value
{
    uint32_t v;

    vMemPtr(uint32_t value) : Value(TYPE_MEM_PTR), v(value) {}

    void print(const OpCode& op) const override;

    void add(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void subtract(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void equal(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void notEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greater(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void less(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void greaterEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
    void lessEqual(const SmartPointer& v2, SmartPointer& rV, const OpCode& op) const override;
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


void runOperation(const SmartPointer& a, SmartPointer& rV, const OpCode& op);
void runOperation(const SmartPointer& a, const SmartPointer& b, SmartPointer& rV, const OpCode& op);

#endif
