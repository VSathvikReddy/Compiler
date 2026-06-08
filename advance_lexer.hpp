#pragma once

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

    void read_string_literal();
    void read_char_literal();
    void read_number_literal();
    

    bool eof() const;
    char peek(size_t offset = 0) const;
    char advance();

    void report_error(const char* message);
};


