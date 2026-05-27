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
    TokenGenerator(Container& lexical_nodes, Container& parser_nodes);

    
    void output(const char* target);
    friend class ParseTokenGen;
    friend class LexTokenGen;
    friend class LexerGenerator;
private:
    std::unordered_map<std::string,std::string> symbol_tokens;
    std::unordered_map<std::string,std::string> keywords_tokens;
    std::unordered_map<std::string,std::string> identifier_tokens;


    std::ofstream& outputPreprocessorDirectives(std::ofstream& out);
    std::ofstream& outputTokenType(std::ofstream& out);
    std::ofstream& outputKeywordMap(std::ofstream& out);
    std::ofstream& outputToStringFunction(std::ofstream& out);
   

};

template <typename Container>
TokenGenerator::TokenGenerator(Container& lexical_nodes, Container& parser_nodes){
    ParseTokenGen pgen(*this);
    for(auto& itr: parser_nodes){
        itr.second->accept(pgen);
    }

    LexTokenGen lgen(*this);

    auto itr = lexical_nodes.begin();
    while (itr != lexical_nodes.end()) {
        lgen.root = itr->first;
        itr->second->accept(lgen);

        if (lgen.isJustRename()) {
            // .erase() returns the iterator to the NEXT element
            itr = lexical_nodes.erase(itr);
        } else {
            // Only increment if we didn't erase
            ++itr;
        }
    }

}






