#include "Builder.hpp"

#include <filesystem>
#include <cstring>

#include "VM.hpp"
#include "Lexer.hpp"

#include "Filetypes.hpp"

void Builder::buildCompiled(const char* filename, std::vector<OpCode>* OpCodes)
{
    std::filesystem::path path(filename);

    path.replace_extension(binExt);

    std::ofstream file;
    file.open(path.generic_string().c_str(), std::ios::out | std::ios::binary);

    size_t filenamesSize = 0;
    for (size_t i = 0; i < Lexer::filenames.size(); i++)
    {
        filenamesSize += Lexer::filenames[i].size();
    }

    std::vector<SmartPointer>& variables = VM::getVariables();
    size_t variableCount = size_tSize;
    for (size_t i = 0; i < variables.size(); i++)
    {
        variableCount += getValueSize(variables[i]);
    }

    auto& lVariables = VM::getLocalVariables();
    size_t lVariableCount = size_tSize;
    size_t totalLVariableCount = 0;
    for (size_t i = 0; i < lVariables.size(); i++)
    {
        for (size_t j = 0; j < lVariables[i].size(); j++)
        {
            lVariableCount += size_tSize + getValueSize(lVariables[i][j]);
            totalLVariableCount++;
        }
    }

    // FilenamesCount [FilenameSize, Filename] Variables OpCodes 
    size_t headerSize = size_tSize + (size_tSize * Lexer::filenames.size()) + filenamesSize + variableCount + lVariableCount + size_tSize;
    char* buffer = (char*)malloc((sizeof(char) * headerSize));

    size_t index = 0;

    addElement(buffer, index, Lexer::filenames.size(), size_tSize);
    for (size_t i = 0; i < Lexer::filenames.size(); i++)
    {
        addElement(buffer, index, Lexer::filenames[i].size(), size_tSize);
        strncpy(buffer + index, Lexer::filenames[i].c_str(), Lexer::filenames[i].size());
        index += Lexer::filenames[i].size();
    }

    addElement(buffer, index, variables.size(), size_tSize);
    for (size_t i = 0; i < variables.size(); i++)
    {
        SmartPointer& value = variables[i];
        addElement(buffer, index, (int)value->type, enumSize);
        addValue(buffer, value, index);
    }

    addElement(buffer, index, totalLVariableCount, size_tSize);
    for (size_t i = 0; i < lVariables.size(); i++)
    {
        for (size_t j = 0; j < lVariables[i].size(); j++)
        {
            SmartPointer& value = lVariables[i][j];
            addElement(buffer, index, i, size_tSize);
            addElement(buffer, index, (int)value->type, enumSize);
            addValue(buffer, value, index);
        }
    }

    addElement(buffer, index, OpCodes->size(), size_tSize);

    file.write(buffer, headerSize);

    constexpr size_t opSize = enumSize + size_tSize + size_tSize + size_tSize; // Operation Source Line Column
    for (size_t i = 0; i < OpCodes->size(); i++)
    {
        index = 0;
        OpCode& op = OpCodes->at(i);

        size_t vSize = getValueSize(op.value); // Type Value
        size_t totalSize = sizeof(char) * (vSize + opSize); // Operation Type Line Column Value
        buffer = (char*)realloc(buffer, totalSize);

        writeOpCode(buffer, op, index);

        file.write(buffer, totalSize);
    }

    delete[] buffer;

    file.close();

    printf("Created %s\n", path.generic_string().c_str());

}

void Builder::loadCompiled(const char* sourcePath)
{
    std::filesystem::path path(sourcePath);

    std::string sourceNameOnly = path.stem().generic_string();

    std::ifstream file;
    file.open(path.generic_string().c_str(), std::ios::in | std::ios::binary);

    char* buffer = (char*)malloc(size_tSize);
    file.read(buffer, size_tSize);

    // Filenames
    size_t filenamesLength;
    readElement(buffer, filenamesLength, size_tSize);

    for (size_t i = 0; i < filenamesLength; i++)
    {
        buffer = (char*)realloc(buffer, sizeof(char) * size_tSize);
        file.read(buffer, size_tSize);
        size_t length;
        readElement(buffer, length, size_tSize);

        char* filename = (char*)malloc(sizeof(char) * length);
        buffer = (char*)realloc(buffer, sizeof(char) * length);
        file.read(buffer, length);
        strncpy(filename, buffer, length);

        Lexer::filenames.emplace_back(filename);

        delete[] filename;
    }

    // Variables

    buffer = (char*)realloc(buffer, size_tSize);
    file.read(buffer, size_tSize);
    size_t variablesSize;
    readElement(buffer, variablesSize, size_tSize);

    OpCode op;
    for (size_t i = 0; i < variablesSize; i++)
    {
        buffer = (char*)realloc(buffer, enumSize);
        file.read(buffer, enumSize);

        ValueType type;
        enumType enumType;

        readElement(buffer, enumType, enumSize);
        type = (ValueType)enumType;

        readValue(file, type, op);

        VM::addGlobalVariable(op.value);
    }

    buffer = (char*)realloc(buffer, size_tSize);
    file.read(buffer, size_tSize);
    size_t lVariablesSize;
    readElement(buffer, lVariablesSize, size_tSize);

    for (size_t i = 0; i < lVariablesSize; i++)
    {
        buffer = (char*)realloc(buffer, size_tSize);
        file.read(buffer, size_tSize);

        uint32_t funcIndex;
        readElement(buffer, funcIndex, size_tSize);

        buffer = (char*)realloc(buffer, enumSize);
        file.read(buffer, enumSize);

        ValueType type;
        enumType enumType;

        readElement(buffer, enumType, enumSize);
        type = (ValueType)enumType;

        readValue(file, type, op);

        VM::addLocalVariable(funcIndex, op.value);
    }

    // OpCodes
    buffer = (char*)realloc(buffer, size_tSize);
    file.read(buffer, size_tSize);
    size_t opcodesLength;
    readElement(buffer, opcodesLength, size_tSize);

    // buffer = (char*)realloc(buffer, sizeof(char) * s_BodySize);
    for (size_t i = 0; i < opcodesLength; i++)
    {
        size_t size = sizeof(char) * (enumSize + enumSize + size_tSize + size_tSize + size_tSize);
        buffer = (char*)realloc(buffer, size);
        file.read(buffer, size);
        OpCode op;

        readOpCode(buffer, op, file);

        VM::addOpCode(op);
    }

    file.close();

    delete[] buffer;
}


void Builder::writeOpCode(char* buffer, const OpCode& op, size_t& index)
{
    enumType opType = static_cast<enumType>(op.code);
    enumType valueType;

    if (!op.value)
        valueType = static_cast<enumType>(TYPE_NULL);
    else
        valueType = static_cast<enumType>(op.value->type);

    size_t sourceIndex = op.sourceIndex;
    size_t line = op.line;
    size_t column = op.column;

    addElement(buffer, index, opType, enumSize); 
    addElement(buffer, index, valueType, enumSize); 
    addElement(buffer, index, sourceIndex, size_tSize);
    addElement(buffer, index, line, size_tSize);
    addElement(buffer, index, column, size_tSize);

    addValue(buffer, op.value, index);
}

void Builder::readOpCode(char* buffer, OpCode& op, std::ifstream& file)
{
    enumType opType, valueType;
    size_t sourceIndex, line, column;

    size_t index = 0;

    readElement(buffer, index, opType, enumSize);
    readElement(buffer, index, valueType, enumSize);
    readElement(buffer, index, sourceIndex, size_tSize);
    readElement(buffer, index, line, size_tSize);
    readElement(buffer, index, column, size_tSize);

    op.code = static_cast<OpCodeEnum>(opType);
    ValueType type = static_cast<ValueType>(valueType);
    op.sourceIndex = sourceIndex;
    op.line = line;
    op.column = column;

    if (valueType >= static_cast<enumType>(TYPE_COUNT))
    {
        assert(false);
    }

    readValue(file, type, op);
}

size_t Builder::getValueSize(const SmartPointer& value)
{
    if (!value.get())
        return enumSize;

    return value->getSize();
}

void Builder::addValue(char* buffer, const SmartPointer& value, size_t& index)
{
    if (!value.get())
        return;

    value->writeBuffer(buffer, index);
}

void Builder::readValue(std::ifstream& file, ValueType type, OpCode& code)
{
    switch (type)
    {
    case TYPE_NULL:
        vNull::readBuffer(file, code);
        break;
    case TYPE_INT:
        {
            vInt::readBuffer(file, code);
            break;
        }
    case TYPE_BOOL:
        {
            vBool::readBuffer(file, code);
            break;
        }
    case TYPE_CHAR:
        {
            vChar::readBuffer(file, code);
            break;
        }
    case TYPE_STRING:
        {
            vString::readBuffer(file, code);
            break;
        }
    case TYPE_MEM_PTR:
        {
            vMemPtr::readBuffer(file, code);
            break;
        }

    case TYPE_IP_OFFSET:
        {
            vIpOffset::readBuffer(file, code);
            break;
        }
    case TYPE_FUNC:
        {
            vFunc::readBuffer(file, code);
            break;
        }
    case TYPE_VAR:
        {
            vVar::readBuffer(file, code);
            break;
        }
    case TYPE_CONST:
        {
            vConst::readBuffer(file, code);
            break;
        }

    default:
        assert(false && "Unreachable");
    }
}
