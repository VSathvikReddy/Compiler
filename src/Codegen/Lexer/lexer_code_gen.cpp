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
    cpp<<"#include \""<<filename<<".hpp\"\n\n";
    outputPreprocessorDirectives(cpp);
    cpp<<"\n\n";

    outputBasicStart(cpp);


    outputIdentifier(cpp);




    outputBasicEnd(cpp);
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
    void tokenize();
    
    
    void skip_COMMENT_and_WHITESPACE();
    void read_IDENTIFIER();
    void read_SYMBOLS();

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
void LexerGenerator::outputBasicStart(std::ofstream& out){
out<<R"(
Lexer::Lexer(const char* file_path): file_name(file_path) ,best_token(TokenType::ERROR,"",1,1,file_path){
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << file_path << std::endl;
        return;
    }
    this->file_data = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    this->source = this->file_data;
    tokenize();
}

void Lexer::print_tokens() const {
    for (const auto& token : this->tokens) {
        std::cout << token << '\n';
    }
}

void Lexer::add_token(TokenType type){
    if(best_match_pos.idx < current_pos.idx ) {
        best_token.type = type;
        best_token.lexeme = this->source.substr(start_pos.idx,this->current_pos.idx - this->start_pos.idx+1);

        best_match_pos = current_pos;
    }
    else if(best_match_pos.idx == current_pos.idx) {
        report_error("Ambugity in grammer");
    }
}

)";
}
void LexerGenerator::outputBasicEnd(std::ofstream& out){
out<<R"(

bool Lexer::eof() const {
    return this->current_pos.idx >= this->source.size();
}
char Lexer::peek(size_t offset) const {
    if (this->current_pos.idx + offset >= this->source.size()) return '\0';
    return this->source[this->current_pos.idx + offset];
}
char Lexer::advance() {
    if (eof()) return '\0';

    char current = this->source[current_pos.idx];
    
    if (current == '\n') {
        this->current_pos.line++;
        this->current_pos.col = 1;
    } else {
        this->current_pos.col++;
    }

    this->current_pos.idx++;
    return current;
}


void Lexer::report_error(const char* message){
    std::cerr<<"Error: "<<message<<
    "\nAt Line:"<<start_pos.line<<" Col:"<<start_pos.col<<'\n';
}
std::vector<Token>& Lexer::getTokens(){
    return this->tokens;
}
)";
}
void LexerGenerator::outputPreprocessorDirectives(std::ofstream& out){
    auto printlib = [&out](const std::string& lib) {
        out << "#include <" << lib << ">\n";
    };
    printlib("fstream");
    printlib("iostream");
}


void LexerGenerator::outputIdentifier(std::ofstream& out){
    out<<"void Lexer::read_IDENTIFIER(){\n";
}