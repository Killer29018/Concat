#ifndef ERROR_HPP
#define ERROR_HPP

#include "Tokens.hpp"
#include "OpCodes.hpp"

#define FMTARGS(FMT) __attribute__((format(gnu_printf, FMT, FMT+1)))

class Error
{
public:
    static void runtimeError(const OpCode& code, const char* fmt, ...) FMTARGS(2);

    static void invalidTypeError(const OpCode& code, const char* fmt, ...) FMTARGS(2);
    static void invalidTypeError(const OpCode& codeA, const OpCode& codeB, const char* fmt, ...) FMTARGS(3);

    static void compilerError();
    static void stackTooSmallError(const OpCode& code, int expectedSize);
private:
    Error() = default;
    ~Error() = default;

    static void printError(const char* errorname, const char* filename, size_t line, size_t column, const char* fmt, ...) FMTARGS(5);
    static void printError(const char* errorname, const char* filename, size_t line, size_t column, const char* fmt, va_list args);
};

#endif
