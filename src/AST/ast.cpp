#include "ast.hpp"



#define VISITER_FUNC(NodeType) void NodeType::accept(ASTVisitor& visitor){ visitor.visit(*this);}


BinaryOperation::BinaryOperation(ASTNode* l, ASTNode* r, BinaryOp o):
    left(l), right(r), op(o) {}
BinaryOperation::~BinaryOperation(){
    delete left;
    delete right;
}



UnaryOperation::UnaryOperation(ASTNode* opnd, UnaryOp o)
    :operand(opnd), op(o) {}
UnaryOperation::~UnaryOperation() {
    delete operand;
}






RangeNode::RangeNode(char l, char h) 
    : low(l), high(h) {}
VISITER_FUNC(RangeNode)

explicit StringNode::StringNode(const std::string& lit) 
    : text(lit) {}
VISITER_FUNC(StringNode)



explicit StringViewNode::StringViewNode(const std::string_view lit, TextNodeType type)
    : text(lit), type(type) {}
VISITER_FUNC(StringViewNode)


explicit CharNode::CharNode(char lit) : text(lit) {}
VISITER_FUNC(CharNode)


#undef VISITER_FUNC