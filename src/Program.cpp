#include "Program.hpp"

#include "Keywords.hpp"

#include <filesystem>

#include "Builder.hpp"

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
    if (path.extension() == ".SBIMCL")
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
        VM::simulate();
}

void Program::runProgramFromFile(const char* filename)
{
    std::filesystem::path path(filename);

    if (path.extension() == ".SBIMCL")
    {
        createOpcodes(filename);
    }
    else if (path.extension() == ".SBIMCL_BIN")
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

    assert(TokenString.size()  == TOKEN_COUNT);
    assert(Keywords.size()     == KeywordCount);
    assert(OpCodeString.size() == OP_COUNT);

    Lexer::lexFile(filename);

    if (printDebugTokens)
        Lexer::printTokens();

    Compiler::startCompiler();

    Lexer::deallocate();

    if (printDebugOpcodes)
        VM::printOpCodes();
}
