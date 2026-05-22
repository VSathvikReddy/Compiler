#pragma once

#include <vector>
#include <string>

#include "char_map.hpp"
#include "ast_derived.hpp"
#include "parse_token_gen.hpp"
#include "lex_token_gen.hpp"

#include <fstream>



class TokenGenerator{
public:
    template <typename Container>
    TokenGenerator(const Container& lexical_nodes, const Container& parser_nodes);

    
    void output(const char* target);

private:
    std::unordered_map<std::string,std::string> symbol_tokens;
    std::unordered_map<std::string,std::string> keywords_tokens;
    std::unordered_map<std::string,std::string> identifier_tokens;
    friend class ParseTokenGen;
    friend class LexTokenGen;

    std::ofstream& outputPreprocessorDirectives(std::ofstream& out);
    std::ofstream& outputTokenType(std::ofstream& out);
    std::ofstream& outputKeywordMap(std::ofstream& out);
    std::ofstream& outputToStringFunction(std::ofstream& out);
   

};

template <typename Container>
TokenGenerator::TokenGenerator(const Container& lexical_nodes, const Container& parser_nodes){
    ParseTokenGen pgen(*this);
    for(auto itr: parser_nodes){
        itr.second->accept(pgen);
    }

    LexTokenGen lgen(*this);
    for(auto itr: lexical_nodes){
        lgen.root = itr.first;
        itr.second->accept(lgen);
    }

}






