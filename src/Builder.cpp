#include "Builder.hpp"

#include <filesystem>
#include <cstring>

#include "VM.hpp"
#include "Lexer.hpp"

#include "Filetypes.hpp"

size_t Builder::s_HeaderSize = size_tSize; // Doesnt include filename

void Builder::buildCompiled(const char* filename, std::vector<OpCode>* OpCodes)
{
    std::filesystem::path path(filename);

    path.replace_extension(binExt);

    std::ofstream file;
    file.open(path.generic_string().c_str(), std::ios::out | std::ios::binary);

    size_t filenamesSize = 0;
    for (size_t i = 0; i < Lexer::filenames.size(); i++)
    {
        filenamesSize += Lexer::filenames[i].size();
    }

    size_t headerSize = s_HeaderSize + size_tSize + (size_tSize * Lexer::filenames.size()) + filenamesSize;
    char* buffer = (char*)malloc((sizeof(char) * headerSize));

    size_t index = 0;

    addElement(buffer, index, Lexer::filenames.size(), size_tSize);

    for (size_t i = 0; i < Lexer::filenames.size(); i++)
    {
        addElement(buffer, index, Lexer::filenames[i].size(), size_tSize);
        strncpy(buffer + index, Lexer::filenames[i].c_str(), Lexer::filenames[i].size());
        index += Lexer::filenames[i].size();
    }

    addElement(buffer, index, OpCodes->size(), size_tSize);

    file.write(buffer, headerSize);

    constexpr size_t opSize = enumSize + size_tSize + size_tSize + size_tSize; // Operation Source Line Column
    for (size_t i = 0; i < OpCodes->size(); i++)
    {
        index = 0;
        OpCode& op = OpCodes->at(i);

        size_t vSize = getValueSize(op.value); // Type Value
        size_t totalSize = sizeof(char) * (vSize + opSize); // Operation Type Line Column Value
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

    size_t filenamesLength;
    readElement(buffer, filenamesLength, size_tSize);

    for (size_t i = 0; i < filenamesLength; i++)
    {
        buffer = (char*)realloc(buffer, sizeof(char) * size_tSize);
        file.read(buffer, size_tSize);
        size_t length;
        readElement(buffer, length, size_tSize);

        char* filename = (char*)malloc(sizeof(char) * length);
        buffer = (char*)realloc(buffer, sizeof(char) * length);
        file.read(buffer, length);
        strncpy(filename, buffer, length);

        Lexer::filenames.emplace_back(filename);

        delete[] filename;
    }

    file.read(buffer, size_tSize);
    size_t opcodesLength;
    readElement(buffer, opcodesLength, size_tSize);

    // buffer = (char*)realloc(buffer, sizeof(char) * s_BodySize);
    for (size_t i = 0; i < opcodesLength; i++)
    {
        size_t size = sizeof(char) * (enumSize + enumSize + size_tSize + size_tSize + size_tSize);
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

    size_t sourceIndex = op.sourceIndex;
    size_t line = op.line;
    size_t column = op.column;

    addElement(buffer, index, opType, enumSize); 
    addElement(buffer, index, valueType, enumSize); 
    addElement(buffer, index, sourceIndex, size_tSize);
    addElement(buffer, index, line, size_tSize);
    addElement(buffer, index, column, size_tSize);

    addValue(buffer, op.value, index);
}

void Builder::readOpCode(char* buffer, OpCode& op, std::ifstream& file)
{
    enumType opType, valueType;
    size_t sourceIndex, line, column;

    size_t index = 0;

    readElement(buffer, index, opType, enumSize);
    readElement(buffer, index, valueType, enumSize);
    readElement(buffer, index, sourceIndex, size_tSize);
    readElement(buffer, index, line, size_tSize);
    readElement(buffer, index, column, size_tSize);

    op.code = static_cast<OpCodeEnum>(opType);
    ValueType type = static_cast<ValueType>(valueType);
    op.sourceIndex = sourceIndex;
    op.line = line;
    op.column = column;

    if (valueType >= static_cast<enumType>(TYPE_COUNT))
    {
        assert(false);
    }

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
    else if (type == TYPE_FUNC)
    {
        value = (char*)realloc(value, size_tSize * sizeof(char));
        file.read(value, size_tSize);

        size_t totalLength;
        readElement(value, totalLength, size_tSize);
        totalLength = (totalLength * enumSize);
        totalLength += (size_tSize * 3);

        size = totalLength;
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
    if (value.get())
    {
        switch (value->type)
        {
        case TYPE_NULL:
            break;
        case TYPE_INT:
            size += sizeof(int32_t); break;
        case TYPE_BOOL:
            size += sizeof(bool); break;
        case TYPE_CHAR:
            size += sizeof(char); break;
        case TYPE_STRING:
            size += size_tSize + get_vStringSize(value); break;
        case TYPE_MEM_PTR:
            size += sizeof(uint32_t); break;
        case TYPE_IP_OFFSET:
            size += sizeof(int32_t); break;
        case TYPE_FUNC:
            {
                vFunc* func = as_vFunc(value);
                size += sizeof(uint32_t) + (size_tSize * 3) + (func->inputs.size() * enumSize)
                        + (func->outputs.size() * enumSize);
                break;
            }

        default:
            assert(false && "Unreachable");
        }
    }

    return size;
}

size_t Builder::getValueSize(const ValueType value)
{
    size_t size;

    switch (value)
    {
    case TYPE_NULL:
        size = 0; break;
    case TYPE_INT:
        size = sizeof(int32_t); break;
    case TYPE_BOOL:
        size = sizeof(bool); break;
    case TYPE_CHAR:
        size = sizeof(char); break;
    case TYPE_MEM_PTR:
        size = sizeof(uint32_t); break;
    case TYPE_IP_OFFSET:
        size = sizeof(int32_t); break;

    default:
        assert(false && "Unreachable");
    }

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
    case TYPE_FUNC:
        addFunc(buffer, index, value);
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
    case TYPE_FUNC:
        {
            readFunc(buffer, op, bufferSize);
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

void Builder::addFunc(char* buffer, size_t& index, const SmartPointer& value)
{
    vFunc* func = as_vFunc(value);

    size_t inputsSize = func->inputs.size();
    size_t outputsSize = func->outputs.size();
    size_t totalSize = inputsSize + outputsSize;

    addElement(buffer, index, totalSize, size_tSize);

    addElement(buffer, index, func->funcIndex, size_tSize);

    addElement(buffer, index, inputsSize, size_tSize);
    for (size_t i = 0; i < inputsSize; i++) { addElement(buffer, index, (int)func->inputs[i], enumSize); }

    addElement(buffer, index, outputsSize, size_tSize);
    for (size_t i = 0; i < outputsSize; i++) { addElement(buffer, index, (int)func->outputs[i], enumSize); }
}

void Builder::readFunc(char* buffer, OpCode& code, size_t bufferSize)
{
    size_t funcIndex, inputsSize, outputsSize;
    enumType type;
    size_t index = 0;

    readElement(buffer, index, funcIndex, size_tSize);

    readElement(buffer, index, inputsSize, size_tSize);

    code.value = makeSmartPointer<vFunc>(funcIndex);

    vFunc* func = as_vFunc(code.value);

    for (size_t i = 0; i < inputsSize; i++)
    {
        readElement(buffer, index, type, enumSize);
        func->inputs.push_back((ValueType)type);
    }

    readElement(buffer, index, outputsSize, size_tSize);
    for (size_t i = 0; i < outputsSize; i++)
    {
        readElement(buffer, index, type, enumSize);
        func->outputs.push_back((ValueType)type);
    }

    if (funcIndex != VM::addFunction())
        assert(false && "Something has gone wrong");
}
