#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"
#include "../VM.hpp"

size_t vConst::getSize() const
{
    return enumSize + sizeof(uint32_t) + sizeof(bool) + sizeof(uint32_t);
}

void vConst::writeBuffer(char* buffer, size_t& index) const
{
    Builder::addElement(buffer, index, inFunction, sizeof(bool));
    Builder::addElement(buffer, index, functionIndex, sizeof(uint32_t));
    Builder::addElement(buffer, index, constIndex, sizeof(uint32_t));
}

void vConst::readBuffer(std::ifstream& file, OpCode& code)
{
    uint32_t index;
    bool inFunction;
    uint32_t functionIndex;

    size_t size = sizeof(char) * sizeof(inFunction);
    char* buffer = (char*)malloc(size);
    file.read(buffer, size);
    Builder::readElement(buffer, inFunction, sizeof(inFunction));

    size = sizeof(char) * sizeof(functionIndex);
    buffer = (char*)realloc(buffer, size);
    file.read(buffer, size);
    Builder::readElement(buffer, functionIndex, sizeof(functionIndex));

    size = sizeof(char) * sizeof(index);
    buffer = (char*)realloc(buffer, size);
    file.read(buffer, size);
    Builder::readElement(buffer, index, sizeof(index));

    code.value = makeSmartPointer<vConst>(index, inFunction, functionIndex);

    if (code.code == OP_CREATE_CONST)
    {
        uint32_t pos;
        if (inFunction)
        {
            pos = VM::addLocalConstant(functionIndex);
        }
        else
        {
            pos = VM::addGlobalConstant();
        }

        if (pos != index)
        {
            assert(false && "Something has gone wrong");
        }
    }
    
    delete buffer;
}
