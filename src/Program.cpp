#include "Program.hpp"

#include "Keywords.hpp"

#include <filesystem>

#include "Builder.hpp"

#include "Filetypes.hpp"

bool Program::printDebugTokens =false;
bool Program::printDebugOpcodes = false;
bool Program::run = false;

void Program::createProgram(bool runmode, const char* filename)
{
    if (runmode)
        runProgramFromFile(filename);
    else
        buildProgramFromFile(filename);
}

void Program::buildProgramFromFile(const char* filename)
{
    std::filesystem::path path(filename);
    if (path.extension() == programExt)
    {
        createOpcodes(filename);
    }
    else
    {
        fprintf(stderr, "Unrecognised filetype for %s\n", filename);
        exit(-1);
    }

    VM::build(filename);

    if (run)
    {
        path.replace_extension(binExt);
        printf("\nRunning %s: \n", path.generic_string().c_str());
        runProgramFromFile(path.generic_string().c_str());
    }
}

void Program::runProgramFromFile(const char* filename)
{
    std::filesystem::path path(filename);

    if (path.extension() == programExt)
    {
        createOpcodes(filename);
    }
    else if (path.extension() == binExt)
    {
        if (printDebugTokens)
        {
            printf("Tokens are unavailable when run fromming Compiled File");
            exit(0);
        }

        Builder::loadCompiled(filename);
    }
    else
    {
        fprintf(stderr, "Unrecognised filetype for %s\n", filename);
        exit(-1);
    }

    VM::simulate();
}

void Program::createOpcodes(const char* filename)
{
    std::filesystem::path path(filename);

    if (!std::filesystem::exists(path))
    {
        fprintf(stderr, "Unknown file %s\n", filename);
        exit(-1);
    }

    assert(TokenString.size() == TOKEN_COUNT && "Tokens strings is not the right size");
    assert(Keywords.size() == KeywordCount && "Keywords size is not correct");
    assert(OpCodeString.size() == OP_COUNT && "OpCode strings is not the right size");
    assert(ValueTypeString.size() == TYPE_COUNT && "vType string is not the right size");

    Lexer::lexFile(filename);

    if (printDebugTokens)
        Lexer::printTokens();

    Compiler::startCompiler();

    if (printDebugOpcodes)
        VM::printOpCodes();

    Lexer::deallocate();
}
