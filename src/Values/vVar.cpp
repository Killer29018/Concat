#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

size_t vVar::getSize() const
{
    return enumSize + sizeof(uint32_t);
}

void vVar::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, varIndex, sizeof(varIndex));
}

void vVar::readBuffer(std::ifstream& file, OpCode& code)
{
    uint32_t value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vVar>(value);

    delete buffer;
}
