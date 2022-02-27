#include "Builder.hpp"

#include <filesystem>
#include <cstring>

void Builder::BuildCompiled(const char* filename, std::vector<OpCode>* OpCodes)
{
    std::filesystem::path path(filename);

    path.replace_extension(".SBIMCL_BIN");

    std::string filenameOnly = path.stem().generic_string();

    std::ofstream file;
    file.open(path.generic_string().c_str(), std::ios::out | std::ios::binary);

    size_t header = sizeof(size_t) + filenameOnly.size() + sizeof(size_t);
    size_t body = sizeof(OpCodeEnum) + sizeof(ValueType) + sizeof(VALUE_TYPE) + (2 * sizeof(size_t));
    size_t size = header + body;

    int index = 0;

    char* text = (char*)malloc(sizeof(char) * size);
    addSizeT(text, index, filenameOnly.size());
    file.write(filenameOnly.c_str(), filenameOnly.size());
    index += filenameOnly.size();
    addSizeT(text, index, OpCodes->size());

    for (size_t i = 0; i < OpCodes->size(); i++)
    {
        OpCode& op = OpCodes->at(i);

        uint32_t opType = static_cast<uint32_t>(op.code);
        uint32_t valueType = static_cast<uint32_t>(op.value.type);
        VALUE_TYPE value = op.value.vInt;
        size_t line = op.line;
        size_t column = op.column;

        addUint32(text, index, opType); 
        addUint32(text, index, valueType); 
        addValue(text, index, value);
        addSizeT(text, index, line);
        addSizeT(text, index, column);

        file.write(text, size);
    }

    file.close();

    free(text);
}

void Builder::addSizeT(char* output, int& index, size_t value)
{
    for (size_t i = 1; i <= sizeof(value); i++) 
    {
        output[index + (sizeof(value) - i)] = value & 0xFF;
        value = value >> 8;
    }
    index += sizeof(value);
}

void Builder::addUint32(char* output, int& index, uint32_t value)
{
    for (size_t i = 1; i <= sizeof(value); i++) 
    {
        output[index + (sizeof(value) - i)] = value & 0xFF;
        value = value >> 8;
    }
    index += sizeof(value);
}

void Builder::addValue(char* output, int& index, VALUE_TYPE value)
{
    for (size_t i = 1; i <= sizeof(value); i++) 
    {
        output[index + (sizeof(value) - i)] = value & 0xFF;
        value = value >> 8;
    }
    index += sizeof(value);
}
