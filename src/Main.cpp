#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

#include <cxxopts.hpp>
#include <filesystem>

#include "Program.hpp"

int main(int argc, char** argv)
{
    std::string inputfile = "";
    cxxopts::Options options("SBIMCL", "Stack Based Interpreted Maybe Compiled Language");
    options.add_options()
        ("print-tokens", "Print Debug Tokens", cxxopts::value<bool>()->default_value("false"))
        ("print-opcodes", "Print Debug OpCodes", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (argc <= 1 || result.count("help"))
    {
        std::cout << "Stack Based Interpreted Maybe Compiled Language\n";
        std::cout << "Usage: \n";
        std::cout << "  SBIMCL [<OPTIONS>] <filepath>\n";
        std::cout << "  OPTIONS\n";
        std::cout << "        --print-tokens    Print Debug Tokens\n";
        std::cout << "        --print-opcodes   Print Debug Opcodes\n";
        std::cout << "    -h, --help            Print Help\n";

        exit(0);
    }

    bool debugTokens = result["print-tokens"].as<bool>();
    Program::printDebugTokens = debugTokens;
    bool debugOpcodes = result["print-opcodes"].as<bool>();
    Program::printDebugOpcodes = debugOpcodes;

    if (std::filesystem::is_regular_file(argv[argc - 1]))
    {
        inputfile = argv[argc - 1];
    }

    if (inputfile.size() != 0)
    {
        Program::startProgramFromFile(inputfile.c_str());
    }
    else
    {
        printf("File is a required option");
    }

    return 0;
}
