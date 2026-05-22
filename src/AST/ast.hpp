#pragma once

#include <cstdint>
#include <string>
#include <string_view>

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
    LITERAL
};

struct BinaryOperation : public ASTNode {
    ASTNode* left = nullptr;
    ASTNode* right = nullptr;
    BinaryOp op;

    BinaryOperation(ASTNode* l, ASTNode* r, BinaryOp o):left(l), right(r), op(o) {}
    ~BinaryOperation() override {delete left;delete right;}

    void accept(ASTVisitor& visitor) override;
};

struct UnaryOperation : public ASTNode {
    ASTNode* operand = nullptr;
    UnaryOp op;

    UnaryOperation(ASTNode* opnd, UnaryOp o):operand(opnd), op(o) {}
    ~UnaryOperation() override {delete operand;}

    void accept(ASTVisitor& visitor) override;
};

struct WildcardNode : public ASTNode {
    explicit WildcardNode() = default;
    void accept(ASTVisitor& visitor) override;
};

struct RangeNode : public ASTNode {
    char low;
    char high;

    RangeNode(char l, char h) : low(l), high(h) {}
    void accept(ASTVisitor& visitor) override;
};

struct StringNode : public ASTNode {
    std::string text;

    explicit StringNode(const std::string& lit) : text(lit) {}
    void accept(ASTVisitor& visitor) override;
};

struct StringViewNode: public ASTNode{
    std::string_view text;
    TextNodeType type;

    explicit StringViewNode(const std::string_view lit, TextNodeType type) : text(lit), type(type) {}
    void accept(ASTVisitor& visitor) override;
};

struct CharNode : public ASTNode {
    char text;

    explicit CharNode(char lit) : text(lit) {}
    void accept(ASTVisitor& visitor) override;
};


class ASTVisitor{
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(const BinaryOperation& node) = 0;
    virtual void visit(const UnaryOperation& node) = 0;

    virtual void visit(const WildcardNode& node) = 0;
    virtual void visit(const RangeNode& node) = 0;
    virtual void visit(const StringNode& node) = 0;
    virtual void visit(const StringViewNode& node) = 0; // Added missing StringViewNode visit
    virtual void visit(const CharNode& node) = 0;
};

// --- INLINE DEFINITIONS ---
// These resolve the circular dependency by being placed after ASTVisitor is fully defined.
inline void BinaryOperation::accept(ASTVisitor& visitor) { visitor.visit(*this); }
inline void UnaryOperation::accept(ASTVisitor& visitor)  { visitor.visit(*this); }
inline void WildcardNode::accept(ASTVisitor& visitor)    { visitor.visit(*this); }
inline void RangeNode::accept(ASTVisitor& visitor)       { visitor.visit(*this); }
inline void StringNode::accept(ASTVisitor& visitor)        { visitor.visit(*this); }
inline void StringViewNode::accept(ASTVisitor& visitor)      { visitor.visit(*this); }
inline void CharNode::accept(ASTVisitor& visitor)        { visitor.visit(*this); }