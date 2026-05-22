#pragma once

#include "ast_derived.hpp"
#include <string>
#include <unordered_map>

class TokenGenerator;

class LexTokenGen:public ASTVisitor{
public:
    LexTokenGen(TokenGenerator& base);
    std::string_view root;


    AST_VISIT_OVVERIDE_FUNCTIONS
private:
    std::unordered_map<std::string,std::string> &symbol_tokens;
    std::unordered_map<std::string,std::string> &keywords_tokens;
    std::unordered_map<std::string,std::string> &identifier_tokens;
};