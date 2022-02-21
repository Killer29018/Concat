#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include <cxxopts.hpp>

#include "Program.hpp"

int main(int argc, char** argv)
{
    std::string inputfile = "";
    cxxopts::Options options("SBIMCL", "Stack Based Interpreted Maybe Compiled Language");
    options.add_options()
        ("print-tokens", "Print Debug Tokens", cxxopts::value<bool>()->default_value("false"))
        ("print-opcodes", "Print Debug OpCodes", cxxopts::value<bool>()->default_value("false"))
        ("f,file", "Input File", cxxopts::value<std::string>(inputfile))
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << "\n";
        exit(0);
    }

    bool debugTokens = result["print-tokens"].as<bool>();
    Program::printDebugTokens = debugTokens;
    bool debugOpcodes = result["print-opcodes"].as<bool>();
    Program::printDebugOpcodes = debugOpcodes;

    if (argc <= 1)
    {
        printf("Invalid Usage\n");
        exit(0);
    }

    if (inputfile.size() != 0)
        Program::startProgramFromFile(inputfile.c_str());
    else
    {
        printf("File is a required option");
    }

    return 0;
}
