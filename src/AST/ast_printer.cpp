
#include "ast_printer.hpp"
#include <iostream>



void ASTPrinter::print_indent() const {
    std::cout << std::string(indent_level * 2, ' ');
}

// Helper to re-escape control characters for safe terminal viewing
std::string ASTPrinter::escape_char(char c) const {
    switch(c) {
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\0': return "\\0";
        default: return std::string(1, c);
    }
}

void ASTPrinter::visit(const BinaryOperation& node) {
    print_indent();
    std::cout << Color::Cyan << "BinaryOperation: " << Color::Bold << op_to_string(node.op) << Color::Reset << "\n";
    
    indent_level++;
    // Always check for nullptrs in case of parser recovery errors
    if (node.left) node.left->accept(*this);
    if (node.right) node.right->accept(*this);
    indent_level--;
}

void ASTPrinter::visit(const UnaryOperation& node) {
    print_indent();
    std::cout << Color::Magenta << "UnaryOperation: " << Color::Bold << op_to_string(node.op) << Color::Reset << "\n";
    
    indent_level++;
    if (node.operand) node.operand->accept(*this);
    indent_level--;
}

// Suppressed the unused parameter warning by omitting the variable name
void ASTPrinter::visit(const WildcardNode& /*node*/) {
    print_indent();
    std::cout << Color::Yellow << "WildcardNode: " << Color::Bold << "." << Color::Reset << "\n";
}

void ASTPrinter::visit(const RangeNode& node) {
    print_indent();
    std::cout << Color::Green << "RangeNode: " << Color::Reset 
                << "[" << Color::Yellow << escape_char(node.low) << Color::Reset << "-" 
                << Color::Yellow << escape_char(node.high) << Color::Reset << "]\n";
}

void ASTPrinter::visit(const StringNode& node) {
    print_indent();
    std::cout << Color::Green << "StringNode: " << Color::Reset 
                << "\"" << Color::Yellow << node.text << Color::Reset << "\"\n";
}

void ASTPrinter::visit(const StringViewNode& node) {
    print_indent();
    std::cout << Color::Green << "StringViewNode: " << Color::Reset 
                << "'" << Color::Yellow << node.text << Color::Reset << "' "
                << Color::Blue << "[" << type_to_string(node.type) << "]" << Color::Reset << "\n";
}

void ASTPrinter::visit(const CharNode& node) {
    print_indent();
    std::cout << Color::Green << "CharNode: " << Color::Reset 
                << "'" << Color::Yellow << escape_char(node.text) << Color::Reset << "'\n";
}