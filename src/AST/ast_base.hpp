#pragma once

#include <cstdint>
#include <string>


class ASTVisitor;

struct ASTNode {
    virtual ~ASTNode() = default; 
    virtual void accept(ASTVisitor& visitor) = 0;
};

enum class BinaryOp: uint8_t {
    OR,   // |
    XOR,  // ^^
    NEXT  // Sequence / Concatenation
};

enum class UnaryOp : uint8_t{
    NOT,      // !
    PLUS,     // +
    STAR,     // *
    QUESTION  // ?
};

enum class TextNodeType{
    PARSER,
    LEXICAL,
    KEYWORD,
    SYMBOL,
};
std::string op_to_string(BinaryOp op);
std::string op_to_string(UnaryOp op);
std::string type_to_string(TextNodeType type);