#include "advance_lexer.hpp"

#include <fstream>
#include <iostream>



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


#define CHECK_BRANCH(function) current_pos = start_pos; function()
void Lexer::tokenize(){

    while(!eof()) {
        skip_comment_and_whitespace(); //Need to advance current and start
        if (eof()) break;

        best_token = Token(TokenType::ERROR,source.substr(start_pos.idx,1),start_pos.line,start_pos.col,file_name);
        best_match_pos = start_pos;

        switch (peek()){
            case '"':
                CHECK_BRANCH(read_string_literal);
            case '1' ... '9' : 
                CHECK_BRANCH(read_number_literal);
            default:
                CHECK_BRANCH(read_symbols);
        }


        if(best_token.type == TokenType::ERROR){
            tokens.push_back(best_token);
            advance();
            start_pos = best_match_pos;
            //current_pos = start_pos; not needed as comment will sync anyways
        }
    }
}
#undef CHECK_BRANCH

void Lexer::read_IDENTIFIER(){

}


void Lexer::read_SYMBOLS(){

}



void Lexer::skip_COMMENT_and_WHITESPACE() {

}


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