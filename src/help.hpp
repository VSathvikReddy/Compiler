#pragma once

#include "ast.hpp"

#include <iostream>
#include <string>

// Helper to convert BinaryLexicalOp to string
inline std::string to_string(BinaryLexicalOp op) {
    switch (op) {
        case BinaryLexicalOp::OR:   return "OR (|)";
        case BinaryLexicalOp::XOR:  return "XOR (^^)";
        case BinaryLexicalOp::NEXT: return "NEXT (Concat)";
        default: return "UNKNOWN";
    }
}

// Helper to convert UnaryLexicalOp to string
inline std::string to_string(UnaryLexicalOp op) {
    switch (op) {
        case UnaryLexicalOp::NOT:      return "NOT (!)";
        case UnaryLexicalOp::PLUS:     return "PLUS (+)";
        case UnaryLexicalOp::STAR:     return "STAR (*)";
        case UnaryLexicalOp::QUESTION: return "QUESTION (?)";
        default: return "UNKNOWN";
    }
}

// The single recursive print function
inline void print_ast(const LexicalNode* node, int depth = 0) {
    if (!node) return;

    // Create a visual indentation string (e.g., "|   |   |-- ")
    std::string indent = "";
    for (int i = 0; i < depth; ++i) {
        indent += (i == depth - 1) ? "|-- " : "|   ";
    }

    // Attempt to cast and print the specific node type
    if (auto ref = dynamic_cast<const RuleReferenceNode*>(node)) {
        std::cout << indent << "RuleReference: " << ref->rule_name << '\n';
    } 
    else if (auto str = dynamic_cast<const StringLiteralNode*>(node)) {
        std::cout << indent << "StringLiteral: \"" << str->literal << "\"\n";
    } 
    else if (auto chr = dynamic_cast<const CharLiteralNode*>(node)) {
        std::cout << indent << "CharLiteral: '" << chr->literal << "'\n";
    } 
    else if (dynamic_cast<const WildcardNode*>(node)) { // Don't need 'auto w' if we don't read fields
        std::cout << indent << "Wildcard (.)\n";
    } 
    else if (auto range = dynamic_cast<const RangeNode*>(node)) {
        std::cout << indent << "Range: [" << range->low << " - " << range->high << "]\n";
    } 
    else if (auto bin = dynamic_cast<const BinaryLexicalOperation*>(node)) {
        std::cout << indent << "BinaryOperation: " << to_string(bin->op) << '\n';
        print_ast(bin->left, depth + 1);
        print_ast(bin->right, depth + 1);
    } 
    else if (auto un = dynamic_cast<const UnaryLexicalOperation*>(node)) {
        std::cout << indent << "UnaryOperation: " << to_string(un->op) << '\n';
        print_ast(un->operand, depth + 1);
    } 
    else {
        std::cout << indent << "[Unknown LexicalNode type]\n";
    }
}