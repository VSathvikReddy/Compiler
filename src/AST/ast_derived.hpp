#pragma once

#include "ast_base.hpp"

#include <string_view>

#define VISIT_OVERRIDE void accept(ASTVisitor& visitor) override

struct BinaryOperation : public ASTNode {
    ASTNode* left = nullptr;
    ASTNode* right = nullptr;
    BinaryOp op;

    BinaryOperation(ASTNode* l, ASTNode* r, BinaryOp o);
    ~BinaryOperation() override;

    VISIT_OVERRIDE;
};

struct UnaryOperation : public ASTNode {
    ASTNode* operand = nullptr;
    UnaryOp op;

    UnaryOperation(ASTNode* opnd, UnaryOp o);
    ~UnaryOperation() override;

    VISIT_OVERRIDE;
};

struct WildcardNode : public ASTNode {
    explicit WildcardNode() = default;
    VISIT_OVERRIDE;
};

struct RangeNode : public ASTNode {
    char low;
    char high;

    RangeNode(char l, char h);
    VISIT_OVERRIDE;
};

struct StringNode : public ASTNode {
    std::string text;

    explicit StringNode(const std::string& lit);
    VISIT_OVERRIDE;
};

struct StringViewNode: public ASTNode{
    std::string_view text;
    TextNodeType type;

    explicit StringViewNode(const std::string_view lit, TextNodeType type);
    VISIT_OVERRIDE;
};

struct CharNode : public ASTNode {
    char text;

    explicit CharNode(char lit);
    VISIT_OVERRIDE;
};

















#define AST_VISIT_OVVERIDE_FUNCTIONS  virtual void visit(const BinaryOperation& node) override; \
    virtual void visit(const UnaryOperation& node) override;  \
    virtual void visit(const WildcardNode& node) override;    \
    virtual void visit(const RangeNode& node) override;       \
    virtual void visit(const StringNode& node) override;      \
    virtual void visit(const StringViewNode& node) override;  \
    virtual void visit(const CharNode& node) override;


class ASTVisitor{
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(const BinaryOperation& node) = 0;
    virtual void visit(const UnaryOperation& node) = 0;

    virtual void visit(const WildcardNode& node) = 0;
    virtual void visit(const RangeNode& node) = 0;
    virtual void visit(const StringNode& node) = 0;
    virtual void visit(const StringViewNode& node) = 0;
    virtual void visit(const CharNode& node) = 0;
};


#undef VISIT_OVERRIDE