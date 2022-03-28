#include "Error.hpp"

#include <cstdio>
#include <cstdarg>

void Error::runtimeError(const OpCode& code, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printError("RUNTIME ERROR", "'N/A'", code.line, code.column, fmt, args);
    va_end(args);
    exit(-1);
}

void Error::compilerError(const Token& token, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printError("COMPILER ERROR", "'N/A'", token.line, token.column, fmt, args);
    va_end(args);
}

void Error::operationError(const OpCode& op, const char* operation, ValueType t1, ValueType t2)
{
    runtimeError(op, "Operation %s does not support %s and %s", operation, ValueTypeString[t1], ValueTypeString[t2]);
}

void Error::operationError(const OpCode& op, const char* operation, ValueType t1)
{
    runtimeError(op, "Operation %s not supported for %s", operation, ValueTypeString[t1]);
}

void Error::castError(const OpCode& op, ValueType type)
{
    runtimeError(op, "Unable to cast from %s to %s", ValueTypeString[type], ValueTypeString[op.value->type]);
}

void Error::stackTooSmallError(const OpCode& code, int expectedSize)
{
    printError("RUNTIME ERROR", "'N/A'", code.line, code.column, "Not enough items on the stack. Atleast %d item/s expected", expectedSize);
    exit(-1);
}

void Error::printError(const char* errorname, const char* filename, size_t line, size_t column, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printError(errorname, filename, line, column, fmt, args);
    va_end(args);
}

void Error::printError(const char* errorname, const char* filename, size_t line, size_t column, const char* fmt, va_list args)
{
    const char* fmtTemplate = "[%s] %s:%ld:%ld %s\n";

    size_t length = snprintf(NULL, 0, fmtTemplate, errorname, filename, line, column, fmt);
    char* newFmt = (char*)malloc(length + 1);
    snprintf(newFmt, length + 1, fmtTemplate, errorname, filename, line, column, fmt);

    vfprintf(stderr, newFmt, args);

    delete[] newFmt;
}
