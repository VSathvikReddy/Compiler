#include "lexer_code_gen.hpp"
#include "token_code_gen.hpp"

#include <iostream>
#include <fstream>

#define STRING_MAP std::unordered_map<std::string,std::string>



LexerGenerator::LexerGenerator(TokenGenerator &ref)
    :symbol_tokens(ref.symbol_tokens), keywords_tokens(ref.keywords_tokens), identifier_tokens(ref.identifier_tokens){}
    

void LexerGenerator::output(const std::string& filename){
    outputHPP(filename);

    std::ofstream cpp(filename + ".cpp");
    cpp<<"#include \""<<filename<<"\"\n\n";
    outputPreprocessorDirectives(cpp);
    cpp<<"\n\n";
}

void LexerGenerator::outputHPP(const std::string& filename){
    std::ofstream out(filename+".hpp");
out<<R"(#pragma once

#include "token.hpp"

#include <vector>
#include <string>
#include <string_view>


struct FilePos{
    size_t idx = 0;
    int line = 1;
    int col = 1;
};

class Lexer {
public:
    Lexer(const char* file_path);
    
    std::vector<Token>& getTokens();
    void print_tokens() const;

private:
    std::string_view source;

    std::string file_data;
    std::string file_name;

    FilePos start_pos;
    FilePos current_pos;
    FilePos best_match_pos;
    
    std::vector<Token> tokens;
    Token best_token;
    void add_token(TokenType type);
    void finish_itr();
    void tokenize();
    
    
    void skip_comment_and_whitespace();
    void read_identifiers();
    void read_symbols();

)";



    for(const auto& itr:identifier_tokens){
        out<<"\tvoid read_"<<itr.first<<"_literal();\n";
    }
    

out<<R"(

    bool eof() const;
    char peek(size_t offset = 0) const;
    char advance();

    void report_error(const char* message);
};
)";



}
void LexerGenerator::outputPreprocessorDirectives(std::ofstream& out){
    auto printlib = [&out](const std::string& lib) {
        out << "#include <" << lib << ">\n";
    };
    printlib("fstream");
    printlib("iostream");
}