#pragma once

#include <vector>
#include <string>

#include "char_map.hpp"
#include <fstream>



class TokenGenerator{
public:
    template <typename Container>
    TokenGenerator(const Container& symbol, const Container& keywords, const Container& identifiers);
    
    void output(const char* target);

private:
    std::unordered_map<std::string,std::string> symbol_tokens;
    std::unordered_map<std::string,std::string> keywords_tokens;
    std::unordered_map<std::string,std::string> identifier_tokens;

    std::ofstream& outputPreprocessorDirectives(std::ofstream& out);
    std::ofstream& outputTokenType(std::ofstream& out);
    std::ofstream& outputKeywordMap(std::ofstream& out);
    std::ofstream& outputToStringFunction(std::ofstream& out);

    template <typename Container> void createSymbolTokens(const Container& container);
    template <typename Container> void createKeywordsTokens(const Container& container);
    template <typename Container> void createIdentifierTokens(const Container& container);
};

template <typename Container>
TokenGenerator::TokenGenerator(const Container& symbol, const Container& keywords, const Container& identifiers){
    createSymbolTokens(symbol);
    createKeywordsTokens(keywords);
    createIdentifierTokens(identifiers);
}

template <typename Container>
void TokenGenerator::createSymbolTokens(const Container& container){
    symbol_tokens.reserve(container.size());
    
    for (const auto& lexeme : container){
        std::string token = "SYMBOL";
        for(const char c: lexeme){
            token+= "_";
            token+= KEYBOARD_CHAR_MAP.at(c);
        }

        symbol_tokens.insert({lexeme,std::move(token)});
    }
}

template <typename Container>
void TokenGenerator::createKeywordsTokens(const Container& container){
    keywords_tokens.reserve(container.size());

    for (const auto& lexeme : container){
        keywords_tokens.insert({lexeme,"KEYWORD_" + lexeme});
    }
}

template <typename Container>
void TokenGenerator::createIdentifierTokens(const Container& container){
    identifier_tokens.reserve(container.size());

    for (const auto& lexeme : container){
        identifier_tokens.insert({lexeme,"LITERAL_" + lexeme});
    }
}