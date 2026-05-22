#pragma once

#include <vector>
#include <string>

#include "char_map.hpp"
#include "ast_derived.hpp"

#include <fstream>



class TokenGenerator: public ASTVisitor {
public:
    template <typename Container>
    TokenGenerator(const Container& lexical_nodes, const Container& parser_nodes);

    
    void output(const char* target);

    AST_VISIT_OVVERIDE_FUNCTIONS

private:
    std::unordered_map<std::string,std::string> symbol_tokens;
    std::unordered_map<std::string,std::string> keywords_tokens;
    std::unordered_map<std::string,std::string> identifier_tokens;

    std::ofstream& outputPreprocessorDirectives(std::ofstream& out);
    std::ofstream& outputTokenType(std::ofstream& out);
    std::ofstream& outputKeywordMap(std::ofstream& out);
    std::ofstream& outputToStringFunction(std::ofstream& out);
   
    void extractTokensFromParserNode(ASTNode* container);
    void extractTokensFromLexicalNode(ASTNode* container);
};

template <typename Container>
TokenGenerator::TokenGenerator(const Container& lexical_nodes, const Container& parser_nodes){
    for(auto itr: parser_nodes){
        extractTokensFromParserNode(itr.second);
    }
    for(auto itr: lexical_nodes){
        extractTokensFromLexicalNode(itr.second);
    }

}






