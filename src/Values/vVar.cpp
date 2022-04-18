#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

size_t vVar::getSize() const
{
    return enumSize + sizeof(bool) + sizeof(uint32_t);
}

void vVar::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, inFunction, sizeof(bool));
    Builder::addElement(buffer, index, varIndex, sizeof(uint32_t));
}

void vVar::readBuffer(std::ifstream& file, OpCode& code)
{
    uint32_t value;
    bool inFunction;

    size_t size = sizeof(char) * sizeof(inFunction);
    char* buffer = (char*)malloc(size);
    file.read(buffer, size);
    Builder::readElement(buffer, inFunction, sizeof(inFunction));

    size = sizeof(char) * sizeof(value);
    buffer = (char*)realloc(buffer, size);
    file.read(buffer, size);
    Builder::readElement(buffer, value, sizeof(value));

    code.value = makeSmartPointer<vVar>(value, inFunction);

    delete buffer;
}
