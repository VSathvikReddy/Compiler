#pragma once
#include <cstdint>

#include <ostream>

enum class TokenType : uint8_t {
    PARSE_IDENTIFIER,
    TOKEN_IDENTIFIER,
    STRING_LITERAL,

    ARROW = '>',

    PIPE = '|',
    LPAREN = '(',
    RPAREN = ')',
    SEMICOLON = ';',

    STAR = '*',
    PLUS = '+',
    QUESTION = '?',

    CHAR_LITERAL,
    NUMBER_LITERAL,
    LBRACKET = '[',
    RBRACKET = ']',
    DASH = '-'
};

struct Token{
    TokenType type;
    std::string_view lexeme;
    struct{
        uint32_t line;
        uint16_t col;
        std::string_view file;
    } debug_info;

    Token(TokenType type, std::string_view lexeme, uint32_t line, uint16_t col, std::string_view file):
        type(type), lexeme(lexeme), debug_info{line,col,file}{}
};

std::ostream& operator<<(std::ostream& os, const Token& token);


