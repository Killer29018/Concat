#include "Value.hpp"
#include "../SmartPointer.hpp"
#include "../Error.hpp"
#include "../Builder.hpp"

size_t vNull::getSize() const { return enumSize; }
void vNull::writeBuffer(char* buffer, size_t& index) const {}
void vNull::readBuffer(std::ifstream& file, OpCode& code) {}
