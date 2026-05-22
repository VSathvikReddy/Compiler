#pragma once

#include "ast.hpp"
#include <iostream>
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

    // Helper to print spaces based on the current depth
    void print_indent() const {
        std::cout << std::string(indent_level * 2, ' ');
    }

    // Helper to re-escape control characters for safe terminal viewing
    std::string escape_char(char c) const {
        switch(c) {
            case '\n': return "\\n";
            case '\r': return "\\r";
            case '\t': return "\\t";
            case '\0': return "\\0";
            default: return std::string(1, c);
        }
    }

    // Helper to stringify Binary Operations
    std::string op_to_string(BinaryOp op) const {
        switch (op) {
            case BinaryOp::OR: return "| (OR)";
            case BinaryOp::XOR: return "^ (XOR)";
            case BinaryOp::NEXT: return "-> (NEXT / CONCAT)";
            default: return "UNKNOWN_BIN_OP";
        }
    }

    // Helper to stringify Unary Operations
    std::string op_to_string(UnaryOp op) const {
        switch (op) {
            case UnaryOp::NOT: return "!";
            case UnaryOp::PLUS: return "+";
            case UnaryOp::STAR: return "*";
            case UnaryOp::QUESTION: return "?";
            default: return "UNKNOWN_UN_OP";
        }
    }

    // Helper to stringify Node Types
    std::string type_to_string(TextNodeType type) const {
        switch(type) {
            case TextNodeType::PARSER: return "PARSER";
            case TextNodeType::LEXICAL: return "LEXICAL";
            case TextNodeType::KEYWORD: return "KEYWORD";
            case TextNodeType::SYMBOL: return "SYMBOL";
            case TextNodeType::LITERAL: return "LITERAL";
            default: return "UNKNOWN";
        }
    }

public:
    void visit(const BinaryOperation& node) override {
        print_indent();
        std::cout << Color::Cyan << "BinaryOperation: " << Color::Bold << op_to_string(node.op) << Color::Reset << "\n";
        
        indent_level++;
        // Always check for nullptrs in case of parser recovery errors
        if (node.left) node.left->accept(*this);
        if (node.right) node.right->accept(*this);
        indent_level--;
    }

    void visit(const UnaryOperation& node) override {
        print_indent();
        std::cout << Color::Magenta << "UnaryOperation: " << Color::Bold << op_to_string(node.op) << Color::Reset << "\n";
        
        indent_level++;
        if (node.operand) node.operand->accept(*this);
        indent_level--;
    }

    // Suppressed the unused parameter warning by omitting the variable name
    void visit(const WildcardNode& /*node*/) override {
        print_indent();
        std::cout << Color::Yellow << "WildcardNode: " << Color::Bold << "." << Color::Reset << "\n";
    }

    void visit(const RangeNode& node) override {
        print_indent();
        std::cout << Color::Green << "RangeNode: " << Color::Reset 
                  << "[" << Color::Yellow << escape_char(node.low) << Color::Reset << "-" 
                  << Color::Yellow << escape_char(node.high) << Color::Reset << "]\n";
    }

    void visit(const StringNode& node) override {
        print_indent();
        std::cout << Color::Green << "StringNode: " << Color::Reset 
                  << "\"" << Color::Yellow << node.text << Color::Reset << "\"\n";
    }

    void visit(const StringViewNode& node) override {
        print_indent();
        std::cout << Color::Green << "StringViewNode: " << Color::Reset 
                  << "'" << Color::Yellow << node.text << Color::Reset << "' "
                  << Color::Blue << "[" << type_to_string(node.type) << "]" << Color::Reset << "\n";
    }

    void visit(const CharNode& node) override {
        print_indent();
        std::cout << Color::Green << "CharNode: " << Color::Reset 
                  << "'" << Color::Yellow << escape_char(node.text) << Color::Reset << "'\n";
    }
};