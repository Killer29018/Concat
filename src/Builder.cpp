#include "Builder.hpp"

#include <filesystem>
#include <cstring>

#include "VirtualMachine.hpp"

size_t Builder::s_HeaderSize = (size_tSize * 2); // Doesnt include filename

void Builder::buildCompiled(const char* filename, std::vector<OpCode>* OpCodes)
{
    std::filesystem::path path(filename);

    path.replace_extension(".CONCAT_BIN");

    std::string filenameOnly = path.stem().generic_string();

    std::ofstream file;
    file.open(path.generic_string().c_str(), std::ios::out | std::ios::binary);

    size_t headerSize = s_HeaderSize + filenameOnly.size();

    size_t index = 0;

    size_t length = filenameOnly.size();
    char* buffer = (char*)malloc((sizeof(char) * headerSize) + (sizeof(char) * length));

    addElement(buffer, index, length, size_tSize);
    strncpy(buffer + index, filenameOnly.c_str(), length);
    index += length;
    addElement(buffer, index, OpCodes->size(), size_tSize);

    file.write(buffer, headerSize);


    for (size_t i = 0; i < OpCodes->size(); i++)
    {
        index = 0;
        OpCode& op = OpCodes->at(i);

        size_t vSize = getValueSize(op.value);
        size_t opSize = enumSize + size_tSize + size_tSize;
        size_t totalSize = sizeof(char) * (vSize + opSize);
        buffer = (char*)realloc(buffer, totalSize);

        writeOpCode(buffer, op, index);

        file.write(buffer, totalSize);
    }

    delete[] buffer;

    file.close();

    printf("Created %s\n", path.generic_string().c_str());

}

void Builder::loadCompiled(const char* sourcePath)
{
    std::filesystem::path path(sourcePath);

    std::string sourceNameOnly = path.stem().generic_string();

    std::ifstream file;
    file.open(path.generic_string().c_str(), std::ios::in | std::ios::binary);

    char* buffer = (char*)malloc(sizeof(char) * size_tSize);
    file.read(buffer, size_tSize);
    size_t filenameLength;
    readElement(buffer, filenameLength, size_tSize);

    char* filename = (char*)malloc(sizeof(char) * filenameLength);
    buffer = (char*)realloc(buffer, sizeof(char) * filenameLength);
    file.read(buffer, filenameLength);
    strncpy(filename, buffer, filenameLength);

    file.read(buffer, size_tSize);
    size_t opcodesLength;
    readElement(buffer, opcodesLength, size_tSize);

    // buffer = (char*)realloc(buffer, sizeof(char) * s_BodySize);
    for (size_t i = 0; i < opcodesLength; i++)
    {
        size_t size = sizeof(char) * (enumSize + enumSize + size_tSize + size_tSize);
        buffer = (char*)realloc(buffer, size);
        file.read(buffer, size);
        OpCode op;

        readOpCode(buffer, op, file);

        VM::addOpCode(op);
    }

    // file.close();

    // delete[] buffer;
    // delete[] filename;
}


void Builder::writeOpCode(char* buffer, const OpCode& op, size_t& index)
{
    enumType opType = static_cast<enumType>(op.code);
    enumType valueType;

    if (!op.value)
        valueType = static_cast<enumType>(TYPE_NULL);
    else
        valueType = static_cast<enumType>(op.value->type);
    size_t line = op.line;
    size_t column = op.column;

    addElement(buffer, index, opType, enumSize); 
    addElement(buffer, index, valueType, enumSize); 
    addElement(buffer, index, line, size_tSize);
    addElement(buffer, index, column, size_tSize);

    addValue(buffer, op.value, index);
}

void Builder::readOpCode(char* buffer, OpCode& op, std::ifstream& file)
{
    enumType opType, valueType;
    size_t line, column;

    size_t index = 0;

    readElement(buffer, index, opType, enumSize);
    readElement(buffer, index, valueType, enumSize);
    readElement(buffer, index, line, size_tSize);
    readElement(buffer, index, column, size_tSize);

    op.code = static_cast<OpCodeEnum>(opType);
    ValueType type = static_cast<ValueType>(valueType);
    op.line = line;
    op.column = column;

    size_t size;
    char* value = (char*)malloc(0);
    if (type == TYPE_STRING)
    {
        value = (char*)realloc(value, size_tSize * sizeof(char));
        file.read(value, size_tSize);
        size_t length;
        readElement(value, length, size_tSize);

        size = length;
    }
    else
    {
        size = sizeof(char) * getValueSize(type);
    }

    value = (char*)realloc(value, size);
    file.read(value, size);
    readValue(value, type, op, size);
    delete value;
}

size_t Builder::getValueSize(const SmartPointer& value)
{
    size_t size = enumSize;
    if (!value)
        size += sizeof(vNull);
    else
    {
        switch (value->type)
        {
        case TYPE_NULL:
            break;
        case TYPE_INT:
            size += sizeof(vInt); break;
        case TYPE_BOOL:
            size += sizeof(vBool); break;
        case TYPE_CHAR:
            size += sizeof(vChar); break;
        case TYPE_STRING:
            size += sizeof(vNull) + size_tSize + get_vStringSize(value); break;
        case TYPE_MEM_PTR:
            size += sizeof(vMemPtr); break;
        case TYPE_IP_OFFSET:
            size += sizeof(vIpOffset); break;

        default:
            assert(false && "Unreachable");
        }
    }

    size -= sizeof(ValueType);

    return size;
}

size_t Builder::getValueSize(const ValueType value)
{
    size_t size = 0;

    switch (value)
    {
    case TYPE_NULL:
        size += sizeof(vNull); break;
    case TYPE_INT:
        size += sizeof(vInt); break;
    case TYPE_BOOL:
        size += sizeof(vBool); break;
    case TYPE_CHAR:
        size += sizeof(vChar); break;
    case TYPE_MEM_PTR:
        size += sizeof(vMemPtr); break;
    case TYPE_IP_OFFSET:
        size += sizeof(vIpOffset); break;

    default:
        assert(false && "Unreachable");
    }

    size -= sizeof(ValueType);

    return size;
}

void Builder::addValue(char* buffer, const SmartPointer& value, size_t& index)
{
    if (!value)
        return;

    switch (value->type)
    {
    case TYPE_NULL:
        break;
    case TYPE_INT:
        addElement(buffer, index, get_vInt(value), sizeof(get_vInt(value)));
        break;
    case TYPE_BOOL:
        addElement(buffer, index, get_vBool(value), sizeof(get_vBool(value)));
        break;
    case TYPE_CHAR:
        addElement(buffer, index, get_vChar(value), sizeof(get_vChar(value)));
        break;
    case TYPE_STRING:
        addString(buffer, index, value);
        break;
    case TYPE_MEM_PTR:
        addElement(buffer, index, get_vMemPtr(value), sizeof(get_vMemPtr(value)));
        break;
    case TYPE_IP_OFFSET:
        addElement(buffer, index, get_vIpOffset(value), sizeof(get_vIpOffset(value)));
        break;

    default:
        assert(false && "Unreachable");
    }
}

void Builder::readValue(char* buffer, ValueType type, OpCode& op, size_t bufferSize)
{
    switch (type)
    {
    case TYPE_NULL:
        op.value = makeSmartPointer<vNull>(); break;
    case TYPE_INT:
        {
            int32_t value;
            readElement(buffer, value, sizeof(value));
            op.value = makeSmartPointer<vInt>(value);
            break;
        }
    case TYPE_BOOL:
        {
            bool value;
            readElement(buffer, value, sizeof(value));
            op.value = makeSmartPointer<vBool>(value);
            break;
        }
    case TYPE_CHAR:
        {
            char value;
            readElement(buffer, value, sizeof(value));
            op.value = makeSmartPointer<vChar>(value);
            break;
        }
    case TYPE_STRING:
        {
            char* value;
            readString(buffer, &value, bufferSize);
            op.value = makeSmartPointer<vString>(value);
            break;
        }
    case TYPE_MEM_PTR:
        {
            uint32_t value;
            readElement(buffer, value, sizeof(value));
            op.value = makeSmartPointer<vMemPtr>(value);
            break;
        }
    case TYPE_IP_OFFSET:
        {
            int32_t value;
            readElement(buffer, value, sizeof(value));
            op.value = makeSmartPointer<vIpOffset>(value);
            break;
        }

    default:
        assert(false && "Unreachable");
    }
}

void Builder::addString(char* buffer, size_t& index, const SmartPointer& value)
{
    vString* str = as_vString(value);
    size_t size = strlen(get_vString(value));

    addElement(buffer, index, size, size_tSize);
    strncpy(buffer + index, str->v, size);

    index += size;
}

void Builder::readString(char* buffer, char** value, size_t bufferSize)
{
    (*value) = (char*)malloc(bufferSize + 1);

    strncpy(*value, buffer, bufferSize);
    (*(value))[bufferSize] = 0x00;

}

