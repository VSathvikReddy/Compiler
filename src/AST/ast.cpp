#include "ast_derived.hpp"


#define VISITER_FUNC(NodeType) void NodeType::accept(ASTVisitor& visitor){ visitor.visit(*this);}


std::string op_to_string(BinaryOp op) {
    switch (op) {
        case BinaryOp::OR: return "| (OR)";
        case BinaryOp::XOR: return "^ (XOR)";
        case BinaryOp::NEXT: return "-> (NEXT / CONCAT)";
        default: return "UNKNOWN_BIN_OP";
    }
}

// Helper to stringify Unary Operations
std::string op_to_string(UnaryOp op) {
    switch (op) {
        case UnaryOp::NOT: return "!";
        case UnaryOp::PLUS: return "+";
        case UnaryOp::STAR: return "*";
        case UnaryOp::QUESTION: return "?";
        default: return "UNKNOWN_UN_OP";
    }
}

// Helper to stringify Node Types
std::string type_to_string(TextNodeType type) {
    switch(type) {
        case TextNodeType::PARSER: return "PARSER";
        case TextNodeType::LEXICAL: return "LEXICAL";
        case TextNodeType::KEYWORD: return "KEYWORD";
        case TextNodeType::SYMBOL: return "SYMBOL";
        default: return "UNKNOWN";
    }
}



BinaryOperation::BinaryOperation(ASTNode* l, ASTNode* r, BinaryOp o):
    left(l), right(r), op(o) {}
BinaryOperation::~BinaryOperation(){
    delete left;
    delete right;
}
VISITER_FUNC(BinaryOperation)



UnaryOperation::UnaryOperation(ASTNode* opnd, UnaryOp o)
    :operand(opnd), op(o) {}
UnaryOperation::~UnaryOperation() {
    delete operand;
}
VISITER_FUNC(UnaryOperation)


VISITER_FUNC(WildcardNode)



RangeNode::RangeNode(char l, char h) 
    : low(l), high(h) {}
VISITER_FUNC(RangeNode)

StringNode::StringNode(const std::string& lit) 
    : text(lit) {}
VISITER_FUNC(StringNode)



StringViewNode::StringViewNode(const std::string_view lit, TextNodeType type)
    : text(lit), type(type) {}
VISITER_FUNC(StringViewNode)


CharNode::CharNode(char lit) : text(lit) {}
VISITER_FUNC(CharNode)


#undef VISITER_FUNC