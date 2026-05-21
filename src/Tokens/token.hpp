#pragma once
#include <cstdint>

#include <ostream>

enum class TokenType : uint8_t {
    PARSE_IDENTIFIER,
    TOKEN_IDENTIFIER,

    STRING_LITERAL,
    CHAR_LITERAL,
    NUMBER_LITERAL,

    ARROW = '>',

    NOT = '^',
    PIPE = '|',
    LPAREN = '(',
    RPAREN = ')',
    

    STAR = '*',
    PLUS = '+',
    QUESTION = '?',
    
    LBRACKET = '[',
    RBRACKET = ']',
    DASH = '-',


    SEMICOLON = ';',
};

struct Token {
    TokenType type;
    uint8_t line;
    uint16_t value;

    Token(TokenType type, uint16_t value, uint8_t line)
        : type(type),  line(line), value(value) {}
};
bool isLiteral(TokenType type);
std::ostream& operator<<(std::ostream& os, const Token& token);


