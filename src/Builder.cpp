#include "Builder.hpp"

#include <filesystem>
#include <cstring>

#include "VirtualMachine.hpp"

size_t Builder::s_HeaderSize = (sizeof(size_t) * 2); // Doesnt include filename
size_t Builder::s_BodySize = sizeof(OpCodeEnum) + sizeof(ValueType) + sizeof(VALUE_TYPE) + (2 * sizeof(size_t));

void Builder::buildCompiled(const char* filename, std::vector<OpCode>* OpCodes)
{
    std::filesystem::path path(filename);

    path.replace_extension(".SBIMCL_BIN");

    std::string filenameOnly = path.stem().generic_string();

    std::ofstream file;
    file.open(path.generic_string().c_str(), std::ios::out | std::ios::binary);

    size_t header = s_HeaderSize + filenameOnly.size();
    size_t size = header + (s_BodySize * OpCodes->size());

    size_t index = 0;

    char* buffer = (char*)malloc(sizeof(char) * size);
    size_t length = filenameOnly.size();
    addElement(buffer, index, length);
    strncpy(buffer + index, filenameOnly.c_str(), length);
    index += length;
    addElement(buffer, index, OpCodes->size());

    for (size_t i = 0; i < OpCodes->size(); i++)
    {
        OpCode& op = OpCodes->at(i);

        writeOpCode(buffer, op, index);
    }
    file.write(buffer, size);

    file.close();

    delete[] buffer;
}

void Builder::loadCompiled(const char* sourcePath)
{
    std::filesystem::path path(sourcePath);

    std::string sourceNameOnly = path.stem().generic_string();

    std::ifstream file;
    file.open(path.generic_string().c_str(), std::ios::in | std::ios::binary);

    char* buffer = (char*)malloc(sizeof(char) * sizeof(size_t));
    file.read(buffer, sizeof(size_t));
    size_t filenameLength;
    readElement(buffer, filenameLength);

    char* filename = (char*)malloc(sizeof(char) * filenameLength);
    buffer = (char*)realloc(buffer, sizeof(char) * filenameLength);
    file.read(buffer, filenameLength);
    strncpy(filename, buffer, filenameLength);

    buffer = (char*)realloc(buffer, sizeof(char) * sizeof(size_t));
    file.read(buffer, sizeof(size_t));
    size_t opcodesLength;

    readElement(buffer, opcodesLength);

    buffer = (char*)realloc(buffer, sizeof(char) * s_BodySize);
    for (size_t i = 0; i < opcodesLength; i++)
    {
        file.read(buffer, sizeof(char) * s_BodySize);
        OpCode op;

        readOpCode(buffer, op);

        VM::addOpCode(op);
    }

    file.close();

    delete[] buffer;
    delete[] filename;
}

void Builder::writeOpCode(char* buffer, const OpCode& op, size_t& index)
{
    uint32_t opType = static_cast<uint32_t>(op.code);
    uint32_t valueType = static_cast<uint32_t>(op.value.type);
    VALUE_TYPE value = op.value.vInt;
    size_t line = op.line;
    size_t column = op.column;

    addElement(buffer, index, opType); 
    addElement(buffer, index, valueType); 
    addElement(buffer, index, value);
    addElement(buffer, index, line);
    addElement(buffer, index, column);
}

void Builder::readOpCode(char* buffer, OpCode& op)
{
    uint32_t opType, valueType;
    VALUE_TYPE value;
    size_t line, column;

    size_t index = 0;

    readElement(buffer, index, opType);
    readElement(buffer, index, valueType);
    readElement(buffer, index, value);
    readElement(buffer, index, line);
    readElement(buffer, index, column);

    op.code = static_cast<OpCodeEnum>(opType);
    op.value.type = static_cast<ValueType>(valueType);
    op.value.vInt = value;
    op.line = line;
    op.column = column;
}
