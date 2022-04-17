#ifndef BUILDER_HPP
#define BUILDER_H__

#include "OpCodes.hpp"
#include <fstream>
#include <vector>

#include "SmartPointer.hpp"

/*
 * File Format:
 * <FilenameLength> <Filename> <MemoryBufferSize> <OpCodesLength> <OpCodes...>
 */

#define enumType uint16_t
#define enumSize sizeof(enumType)
#define size_tSize sizeof(uint32_t)

class Builder
{
public:
private:
public:
    static void buildCompiled(const char* filename, std::vector<OpCode>* opcodes);
    static void loadCompiled(const char* sourcePath);

    template<typename T>
    static void addElement(char* output, size_t& index, T value, size_t size)
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
        value = 0;
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
        value = 0;
        for (size_t i = 0; i < size; i++)
        {
            value <<= 8;

            uint8_t element = output[i] & 0xFF;
            value |= element;
        }
    }

    static void readElement(char* output, bool& value, size_t size)
    {
        value = output[0] & 0xFF;
    }
private:
    Builder() = default;
    ~Builder() = default;

    static void writeOpCode(char* buffer, const OpCode& op, size_t& index);
    static void readOpCode(char* buffer, OpCode& op, std::ifstream& file);

    static size_t getValueSize(const SmartPointer& value);
    // static size_t getValueSize(const ValueType value);

    static void addValue(char* buffer, const SmartPointer& value, size_t& index);
    static void readValue(std::ifstream& file, ValueType type, OpCode& op);
};

#endif
