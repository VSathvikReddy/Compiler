#pragma once

#include "token.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

struct FilePos{
    uint32_t idx;
    uint32_t line;
    uint16_t col;
};

class Lexer {
public:
    Lexer(std::string_view source, std::string_view file_name);
    
    std::vector<Token>& getTokens();
    void print_tokens() const;

private:
    std::string_view source;
    std::string_view file_name;

    FilePos start, current;

    std::vector<Token> tokens;

    void add_token(TokenType type, const std::string& lexeme);
    void add_token(TokenType type, uint16_t value);
    void add_token(TokenType type);

    void tokenize();


    bool eof() const;
    char peek(size_t offset = 0) const;
    char advance();
};