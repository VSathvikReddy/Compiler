#include "lex_token_gen.hpp"
#include "token_code_gen.hpp"
#include <iostream>

LexTokenGen::LexTokenGen(TokenGenerator& base)
    :symbol_tokens(base.symbol_tokens), keywords_tokens(base.keywords_tokens), identifier_tokens(base.identifier_tokens){}


void LexTokenGen::visit(const BinaryOperation& /*node*/){
    auto itr = identifier_tokens.find(std::string(root));
    if(itr==identifier_tokens.end()){
        if(root != "COMMENT" && root!= "SKIP"){
            std::cerr<<"Warning: You defined a Litral Token but but not using it in you prasing trees\n"<<root<<'\n';
            identifier_tokens.emplace(std::string(root), "LITEREAL_" + std::string(root));
        }

    }
    
    return;
}
void LexTokenGen::visit(const UnaryOperation& /*node*/){
    auto itr = identifier_tokens.find(std::string(root));
    if(itr==identifier_tokens.end()){
        if(root != "COMMENT" && root!= "SKIP"){
            std::cerr<<"Warning: You defined a Litral Token but but not using it in you prasing trees\n"<<root<<'\n';
            identifier_tokens.emplace(std::string(root), "LITEREAL_" + std::string(root));
        }
    }
    return;
}  
void LexTokenGen::visit(const WildcardNode& /*node*/){
    auto itr = identifier_tokens.find(std::string(root));
    if(itr==identifier_tokens.end()){
        if(root != "COMMENT" && root!= "SKIP"){
            std::cerr<<"Warning: You defined a Litral Token but but not using it in you prasing trees\n"<<root<<'\n';
            identifier_tokens.emplace(std::string(root), "LITEREAL_" + std::string(root));
        }
    }
    return;
} 
void LexTokenGen::visit(const RangeNode& /*node*/){
    auto itr = identifier_tokens.find(std::string(root));
    if(itr==identifier_tokens.end()){
        if(root != "COMMENT" && root!= "SKIP"){
            std::cerr<<"Warning: You defined a Litral Token but but not using it in you prasing trees\n"<<root<<'\n';
            identifier_tokens.emplace(std::string(root), "LITEREAL_" + std::string(root));
        }
    }
    return;
}  

//407 in parser.cpp
inline bool is_valid_keyword_format(std::string_view sv) {
    if (sv.empty()) return false;

    size_t i = 0;    // 1. Skip leading underscores
    while (i < sv.size() && sv[i] == '_')i++;

    if (i == sv.size()) return false;

    if (!std::isalpha(sv[i])) {
        return false;
    }

    for (; i < sv.size(); ++i) {
        if (!std::isalnum(sv[i]) && sv[i] != '_') {
            return false;
        }
    }

    return true;
}
void LexTokenGen::visit(const StringNode&  node){
    if(is_valid_keyword_format(node.text)){
        keywords_tokens[std::string(node.text)] = "KW_" + std::string(root);
    }else{
        symbol_tokens[std::string(node.text)] = "SYMBOL_" + std::string(root);
    }
    return;
}
void LexTokenGen::visit(const StringViewNode& /*node*/){
    return;
}  
void LexTokenGen::visit(const CharNode& node){
    symbol_tokens[std::string(1,node.text)] = "SYMBOL_" + std::string(root);
}