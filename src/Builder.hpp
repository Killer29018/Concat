#ifndef BUILDER_HPP
#define BUILDER_H__

#include "OpCodes.hpp"
#include <fstream>
#include <vector>

class Builder
{
public:
    static void BuildCompiled(const char* filename, std::vector<OpCode>* opcodes);
private:
    Builder() = default;
    ~Builder() = default;

    static void addSizeT(char* output, int& index, size_t value);
    static void addUint32(char* output, int& index, uint32_t value);
    static void addValue(char* output, int& index, VALUE_TYPE value);
};

#endif
