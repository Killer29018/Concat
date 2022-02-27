#include "Builder.hpp"

#include <filesystem>
#include <cstring>

#include "VirtualMachine.hpp"

size_t Builder::s_HeaderSize = (Size_tSize * 2); // Doesnt include filename
size_t Builder::s_BodySize = EnumSize + EnumSize + sizeof(VALUE_TYPE) + (2 * Size_tSize);

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
    addElement(buffer, index, length, Size_tSize);
    strncpy(buffer + index, filenameOnly.c_str(), length);
    index += length;
    addElement(buffer, index, OpCodes->size(), Size_tSize);

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

    char* buffer = (char*)malloc(sizeof(char) * Size_tSize);
    file.read(buffer, Size_tSize);
    size_t filenameLength;
    readElement(buffer, filenameLength, Size_tSize);

    char* filename = (char*)malloc(sizeof(char) * filenameLength);
    buffer = (char*)realloc(buffer, sizeof(char) * filenameLength);
    file.read(buffer, filenameLength);
    strncpy(filename, buffer, filenameLength);

    buffer = (char*)realloc(buffer, sizeof(char) * Size_tSize);
    file.read(buffer, Size_tSize);
    size_t opcodesLength;

    readElement(buffer, opcodesLength, Size_tSize);

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
    EnumType opType = static_cast<EnumType>(op.code);
    EnumType valueType = static_cast<EnumType>(op.value.type);
    VALUE_TYPE value = op.value.vInt;
    size_t line = op.line;
    size_t column = op.column;

    addElement(buffer, index, opType, EnumSize); 
    addElement(buffer, index, valueType, EnumSize); 
    addElement(buffer, index, value, sizeof(value));
    addElement(buffer, index, line, Size_tSize);
    addElement(buffer, index, column, Size_tSize);
}

void Builder::readOpCode(char* buffer, OpCode& op)
{
    uint32_t opType = 0;
    uint32_t valueType = 0;
    VALUE_TYPE value = 0;
    size_t line = 0;
    size_t column = 0;

    size_t index = 0;

    readElement(buffer, index, opType, EnumSize);
    readElement(buffer, index, valueType, EnumSize);
    readElement(buffer, index, value, sizeof(value));
    readElement(buffer, index, line, Size_tSize);
    readElement(buffer, index, column, Size_tSize);

    op.code = static_cast<OpCodeEnum>(opType);
    op.value.type = static_cast<ValueType>(valueType);
    op.value.vInt = value;
    op.line = line;
    op.column = column;
}
