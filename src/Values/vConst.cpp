#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"
#include "../VM.hpp"

size_t vConst::getSize() const
{
    return enumSize + sizeof(uint32_t);
}

void vConst::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, constIndex, sizeof(constIndex));
}

void vConst::readBuffer(std::ifstream& file, OpCode& code)
{
    uint32_t value;
    constexpr size_t size = sizeof(char) * sizeof(value);
    char* buffer = (char*)malloc(size);
    file.read(buffer, size);

    Builder::readElement(buffer, value, sizeof(value));

    if (code.code == OP_CREATE_CONST && value != VM::addGlobalConstant())
    {
        assert(false && "Something has gone wrong");
    }

    code.value = makeSmartPointer<vConst>(value);
    
    delete buffer;
}
