#ifndef BUILDER_HPP
#define BUILDER_H__

#include "OpCodes.hpp"
#include <fstream>
#include <vector>

/*
 * File Format:
 * <FilenameLength> <Filename> <MemoryBufferSize> <OpCodesLength> <OpCodes...>
 */

#define EnumType uint32_t
#define EnumSize sizeof(uint32_t)
#define Size_tSize sizeof(uint32_t)

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
    static void addElement(char * output, size_t& index, T value, size_t size)
    {
        for (size_t i = 1; i <= size; i++) 
        {
            output[index + (size - i)] = value & 0xFF;
            value >>= 8;
        }
        index += size;
    }

    template<typename T>
    static void readElement(char* output, size_t& index, T& value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            value <<= 8;
            uint8_t element = output[index + i] & 0xFF;
            value |= element;
        }
        index += size;
    }
    
    template<typename T>
    static void readElement(char* output, T& value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            value <<= 8;
            uint8_t element = output[i] & 0xFF;
            value |= element;
        }
    }
};

#endif
