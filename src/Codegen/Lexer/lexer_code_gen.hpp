#pragma once

#include <unordered_map>
#include <string>

#define STRING_MAP std::unordered_map<std::string,std::string>

class TokenGenerator;

class LexerGenerator{
public:
    LexerGenerator(TokenGenerator &ref);
    

    void output(const std::string& filename);

    void outputHPP(const std::string& hpp_name);
    void outputPreprocessorDirectives(std::ofstream& out);
private:
    STRING_MAP &symbol_tokens;
    STRING_MAP &keywords_tokens;
    STRING_MAP &identifier_tokens;
};

#undef STRING_MAP