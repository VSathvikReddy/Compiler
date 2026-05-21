#pragma once

#include "token.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

class Lexer {
public:
    Lexer(const char* file_path);
    
    std::vector<Token>& getTokens();
    void print_tokens() const;

private:
    std::string_view source;
    std::string file_data;

    size_t pos = 0;
    int line = 1;

    std::vector<Token> tokens;

    // Token creation dispatch helpers
    void add_token(TokenType type, const std::string& lexeme);
    void add_token(TokenType type, uint16_t value);
    void add_token(TokenType type);

    void tokenize();
    void read_identifiers();
    void read_string_literal();
    void read_symbols();
    void skip_comment_and_whitespace();

    bool eof() const;
    char peek(size_t offset = 0) const;
    char advance();
};