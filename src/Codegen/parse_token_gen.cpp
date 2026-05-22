#include "parse_token_gen.hpp"
#include "token_code_gen.hpp"
    
#include <iostream>

ParseTokenGen::ParseTokenGen(TokenGenerator& base)
    :symbol_tokens(base.symbol_tokens), keywords_tokens(base.keywords_tokens), identifier_tokens(base.identifier_tokens){}


void ParseTokenGen::visit(const BinaryOperation& node){
    node.left->accept(*this);
    node.right->accept(*this);
}
void ParseTokenGen::visit(const UnaryOperation& node){
    node.operand->accept(*this);
}  
void ParseTokenGen::visit(const WildcardNode& /*node*/){
    std::cerr<<"WTF .\n";
    return;
} 
void ParseTokenGen::visit(const RangeNode& /*node*/){
    std::cerr<<"Wtf []\n";
    return;
}  
void ParseTokenGen::visit(const StringNode& node){
    return;
}  
void ParseTokenGen::visit(const StringViewNode& node){
    switch(node.type){
        case(TextNodeType::SYMBOL):{

            break;
        }
        case(TextNodeType::KEYWORD):{
           keywords_tokens.insert(static_cast<std::string>(node.text),"KW_"+static_cast<std::string>(node.text));
           break;
        }
        case(TextNodeType::LEXICAL):{
            identifier_tokens.insert(static_cast<std::string>(node.text),"LITEREAL_"+static_cast<std::string>(node.text));
        }
        case(TextNodeType::PARSER):
        default:
            break;
    }
}  
void ParseTokenGen::visit(const CharNode& node){

}