#include "parse_token_gen.hpp"
#include "token_code_gen.hpp"
#include "char_map.hpp"
    
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
void ParseTokenGen::visit(const StringNode&  /*node*/){
    std::cerr<<"Again wtf\n";
    return;
}  
void ParseTokenGen::visit(const StringViewNode& node){
    switch(node.type){
        case(TextNodeType::SYMBOL):{
            std::string token = "SYMBOL";
            for(const char c: node.text){
                token+= "_";
                token+= KEYBOARD_CHAR_MAP.at(c);
            }
            symbol_tokens.emplace(std::string(node.text),token);
            break;
        }
        case(TextNodeType::KEYWORD):{
            std::string text_str(node.text);
            keywords_tokens.emplace(text_str, "KW_" + text_str);
            break;
        }
        case(TextNodeType::LEXICAL):{
            std::string text_str(node.text);
            identifier_tokens.emplace(text_str, "LITEREAL_" + text_str);
            break;
        }
        case(TextNodeType::PARSER):
        default:
            break;
    }
}  
void ParseTokenGen::visit(const CharNode& node){
    std::string key(1, node.text);
    symbol_tokens.emplace(key,"SYMBOL_"+key);
}