#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"
#include "../VM.hpp"

size_t vFunc::getSize() const
{
    return enumSize + sizeof(uint32_t) + (size_tSize * 3) + (inputs.size() * enumSize) + (outputs.size() * enumSize);
}

void vFunc::writeBuffer(char* buffer, size_t& index) const
{
    size_t inputsSize = inputs.size();
    size_t outputsSize = outputs.size();
    size_t totalSize = inputsSize + outputsSize;

    Builder::addElement(buffer, index, totalSize, size_tSize);

    Builder::addElement(buffer, index, funcIndex, size_tSize);

    Builder::addElement(buffer, index, inputsSize, size_tSize);
    for (size_t i = 0; i < inputsSize; i++) { Builder::addElement(buffer, index, (int)inputs[i], enumSize); }

    Builder::addElement(buffer, index, outputsSize, size_tSize);
    for (size_t i = 0; i < outputsSize; i++) { Builder::addElement(buffer, index, (int)outputs[i], enumSize); }
}

void vFunc::readBuffer(std::ifstream& file, OpCode& code)
{
    size_t size = sizeof(char) * size_tSize;
    char* buffer = (char*)malloc(size);
    file.read(buffer, size);

    size_t totalLength;
    Builder::readElement(buffer, totalLength, size_tSize);
    totalLength *= enumSize;
    totalLength += (size_tSize * 2) + sizeof(uint32_t);

    buffer = (char*)realloc(buffer, totalLength);
    file.read(buffer, totalLength);
    
    uint32_t funcIndex;
    size_t inputsSize, outputsSize;
    enumType type;
    size_t index = 0;

    Builder::readElement(buffer, index, funcIndex, sizeof(uint32_t));

    code.value = makeSmartPointer<vFunc>(funcIndex);

    vFunc* func = as_vFunc(code.value);

    if (code.code == OP_FUNC && funcIndex != VM::addFunction())
        assert(false && "Something has gone wrong");

    if (code.code == OP_FUNC)
        VM::addFunctionDefinition(code);

    Builder::readElement(buffer, index, inputsSize, size_tSize);
    for (size_t i = 0; i < inputsSize; i++)
    {
        Builder::readElement(buffer, index, type, enumSize);
        func->inputs.push_back((ValueType)type);
    }

    Builder::readElement(buffer, index, outputsSize, size_tSize);
    for (size_t i = 0; i < outputsSize; i++)
    {
        Builder::readElement(buffer, index, type, enumSize);
        func->outputs.push_back((ValueType)type);
    }

    delete buffer;
}
