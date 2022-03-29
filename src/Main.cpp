#include <cstdio>

/*
 *      Text -> Lexer -> Tokens
 *      Tokens -> Compiler -> Opcode
 *      Opcode -> Virtual Machine -> Output
 */

// #include <cxxopts.hpp>
// #include <filesystem>
#include "Options.hpp"

#include "Program.hpp"

int main(int argc, char** argv)
{
    // cxxopts::Options options("Concat", "Concatenative - Imperitive Language");
    // options.add_options()
    //     ("t,print-tokens", "Print Debug Tokens", cxxopts::value<bool>()->default_value("false"))
    //     ("o,print-opcodes", "Print Debug OpCodes", cxxopts::value<bool>()->default_value("false"))
    //     ("r,run", "Run", cxxopts::value<bool>()->default_value("false"))
    //     ("h,help", "Print usage")
    // ;

    // auto result = options.parse(argc, argv);

    // if (argc <= 1 || result.count("help"))
    // {
    //     std::cout << "Usage: \n";
    //     std::cout << "  Concatenative - Imperitive Language\n";
    //     std::cout << "  Concat <MODE> [<OPTIONS>] <filepath>\n";
    //     std::cout << "  MODE\n";
    //     std::cout << "    run                   Run either a source file or Compiled File\n";
    //     std::cout << "    build                 Build a source file into a Compiled file\n";
    //     std::cout << "  OPTIONS\n";
    //     std::cout << "    -t, --print-tokens    Print Debug Tokens\n";
    //     std::cout << "    -o, --print-opcodes   Print Debug Opcodes\n";
    //     std::cout << "    -r, --run             When building run the program as well\n";
    //     std::cout << "    -h, --help            Print Help\n";

    //     exit(0);
    // }

    // Program::printDebugTokens = result["print-tokens"].as<bool>();
    // Program::printDebugOpcodes = result["print-opcodes"].as<bool>();
    // Program::run = result["run"].as<bool>();

    // std::string inputfile = "";
    // bool runmode = false;

    // if (argc < 3)
    // {
    //     printf("Both mode and filepath are required");
    //     exit(0);
    // }

    // if (!strcmp(argv[1], "run"))
    // {
    //     runmode = true;
    // }
    // else if (!strcmp(argv[1], "build"))
    // {
    //     runmode = false;
    // }
    // else
    // {
    //     printf("Unrecognised mode");
    //     exit(0);
    // }

    // if (std::filesystem::is_regular_file(argv[argc - 1]))
    // {
    //     inputfile = argv[argc - 1];
    // }
    // else
    // {
    //     printf("File is a required option");
    //     exit(0);
    // }
    //
    Options::getValues(argc, argv);

    Program::printDebugTokens = Options::printDebugTokens;
    Program::printDebugOpcodes = Options::printDebugOpcodes;
    Program::runBuild = Options::runBuild;

    Program::createProgram(Options::mode, Options::filePath.c_str());

    return 0;
}
