#ifndef BUILDER_HPP
#define BUILDER_H__

#include "OpCodes.hpp"
#include <fstream>
#include <vector>

//TODO Remove requirement for Compiler based sizes for elements.
//Currently Compilation wont work between compilers due to requirement
//on sizeof

class Builder
{
private:
    static size_t s_HeaderSize;
    static size_t s_BodySize;
public:
    static void buildCompiled(const char* filename, std::vector<OpCode>* opcodes);
    static void loadCompiled(const char* sourcePath);
private:
    Builder() = default;
    ~Builder() = default;

    static void writeOpCode(char* buffer, const OpCode& op, size_t& index);
    static void readOpCode(char* buffer, OpCode& op);

    template<typename T>
    static void addElement(char * output, size_t& index, T value)
    {
        for (size_t i = 1; i <= sizeof(value); i++) 
        {
            output[index + (sizeof(value) - i)] = value & 0xFF;
            value >>= 8;
        }
        index += sizeof(value);
    }

    template<typename T>
    static void readElement(char* output, size_t& index, T& value)
    {
        for (size_t i = 0; i < sizeof(value); i++)
        {
            value <<= 8;
            uint8_t element = output[index + i] & 0xFF;
            value |= element;
        }
        index += sizeof(value);
    }
    
    template<typename T>
    static void readElement(char* output, T& value)
    {
        for (size_t i = 0; i < sizeof(value); i++)
        {
            value <<= 8;
            uint8_t element = output[i] & 0xFF;
            value |= element;
        }
    }
};

#endif
