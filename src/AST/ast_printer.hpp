#pragma once

#include "ast_derived.hpp"
#include <string>

// ANSI Color Codes for terminal output
namespace Color {
    constexpr const char* Reset   = "\033[0m";
    constexpr const char* Bold    = "\033[1m";
    constexpr const char* Red     = "\033[31m";
    constexpr const char* Green   = "\033[32m";
    constexpr const char* Yellow  = "\033[33m";
    constexpr const char* Blue    = "\033[34m";
    constexpr const char* Magenta = "\033[35m";
    constexpr const char* Cyan    = "\033[36m";
}

class ASTPrinter : public ASTVisitor {
private:
    int indent_level = 0;
    void print_indent() const;
    std::string escape_char(char c) const;


public:
    AST_VISIT_OVVERIDE_FUNCTIONS
};