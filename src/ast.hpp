#pragma once

#include <vector>
#include <string_view>
#include <string>


struct ASTNode {
    virtual ~ASTNode() = default; 
};

struct LexicalNode : public ASTNode {};
struct ParserNode : public ASTNode {};


struct RuleReferenceNode : public LexicalNode {
    std::string_view rule_name;
    explicit RuleReferenceNode(std::string_view name) : rule_name(name) {}
};

struct StringLiteralNode : public LexicalNode {
    std::string literal;
    explicit StringLiteralNode(const std::string& lit) : literal(lit) {}
};

struct CharLiteralNode : public LexicalNode {
    char literal;
    explicit CharLiteralNode(char lit) : literal(lit) {}
};

// Represents the '.' character wildcard matching anything
struct WildcardNode : public LexicalNode {};

struct RangeNode : public LexicalNode {
    char low;
    char high;
    RangeNode(char l, char h) : low(l), high(h) {}
};

enum class BinaryLexicalOp: uint8_t {
    OR,   // |
    XOR,  // ^^
    NEXT  // Sequence / Concatenation
};

enum class UnaryLexicalOp : uint_fast16_t{
    NOT,      // !
    PLUS,     // +
    STAR,     // *
    QUESTION  // ?
};

struct BinaryLexicalOperation : public LexicalNode {
    LexicalNode* left = nullptr;
    LexicalNode* right = nullptr;
    BinaryLexicalOp op;

    BinaryLexicalOperation(LexicalNode* l, LexicalNode* r, BinaryLexicalOp o)
        : left(l), right(r), op(o) {}

    ~BinaryLexicalOperation() override {
        delete left;
        delete right;
    }
};

struct UnaryLexicalOperation : public LexicalNode {
    LexicalNode* operand = nullptr;
    UnaryLexicalOp op;

    UnaryLexicalOperation(LexicalNode* opnd, UnaryLexicalOp o)
        : operand(opnd), op(o) {}

    ~UnaryLexicalOperation() override {
        delete operand;
    }
};




