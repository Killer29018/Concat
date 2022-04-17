#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

size_t vIpOffset::getSize() const
{
    return enumSize + sizeof(int32_t);
}

void vIpOffset::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, v, sizeof(v));
}

void vIpOffset::readBuffer(std::ifstream& file, OpCode& code)
{
    int32_t value;
    size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);

    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));
    code.value = makeSmartPointer<vIpOffset>(value);

    delete buffer;
}
