#include "lexer.hpp"

#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>


Lexer::Lexer(const char* file_path): file_name(file_path) {
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

void Lexer::add_token(TokenType type, std::string_view lexeme) {
    this->tokens.emplace_back(type, lexeme, line, col, file_name);
}
void Lexer::tokenize(){
    while(!eof()) {
        skip_comment_and_whitespace();
        

        if (eof()) break;

        if (isalpha(peek()) || peek() == '_') {
            read_identifiers();
        }else if (peek() == '\'') {
            read_char_literal();
        } else if (peek() == '"') {
            read_string_literal();
        } else if(isdigit(peek())){
            read_number_literal();
        }
        else {
            read_symbols();
        }
    }
}

static bool allCaps(std::string_view text){
    return std::all_of(text.begin(), text.end(), [](char c){ 
        return std::isupper(static_cast<unsigned char>(c)) || c == '_'; 
    });
}
void Lexer::read_identifiers(){
    size_t start = pos;

    // Consume the full word
    while (isalnum(peek()) || peek() == '_') advance();
    
    std::string_view text = this->source.substr(start, pos - start);


    if(allCaps(text)) add_token(TokenType::TOKEN_IDENTIFIER, text);
    else add_token(TokenType::PARSE_IDENTIFIER, text);

}


void Lexer::read_char_literal(){
    size_t start = pos;
    advance(); // Skip opening quote


    if (peek() == '\\') { // Handle escape sequences
        advance(); // Skip backslash
    }
    advance(); // Skip character

    if (peek() != '\'') {
        std::cerr << "Unterminated char literal at " << this->line << ":" << this->col << std::endl;
        return;
    }
    advance(); // Skip closing quote

    add_token(TokenType::CHAR_LITERAL, this->source.substr(start, this->pos - start));
}
void Lexer::read_string_literal(){
    size_t start = pos;
    advance(); // Skip opening quote

    while (!eof() && peek() != '"') {
        if(peek() == '\\') { // Handle escape sequences
            advance(); // Skip backslash
        }
        advance();
    }

    if(eof()) {
        std::cerr << "Unterminated string literal at " << this->line << ":" << this->col << std::endl;
        return;
    }
    advance(); // Skip closing quote

    add_token(TokenType::STRING_LITERAL, this->source.substr(start, this->pos - start));
}


void Lexer::read_number_literal() {
    size_t start = pos;

    // 1. Consume the initial sequence of digits (Integer part)
    while (isdigit(peek())) {
        advance();
    }

    // 2. Check if there's a fractional part (e.g., .123)
    // We look ahead to make sure a digit follows the dot, 
    // so we don't accidentally consume a standalone method call dot (e.g., 42.to_string())
    if (peek() == '.' && isdigit(peek(1))) {
        advance(); // Consume the '.'
        
        // Consume the remaining digits after the decimal point
        while (isdigit(peek())) {
            advance();
        }
    }

    // 3. Extract the full lexeme and add the token
    std::string_view text = this->source.substr(start, pos - start);
    add_token(TokenType::NUMBER_LITERAL, text);
}

void Lexer::read_symbols(){
    auto read_symbol = [&](const char* expected, TokenType type) {
        size_t len = strlen(expected);
        for (size_t i = 0; i < len; i++) {
            if (peek(i) != expected[i]) return false;
        }
        add_token(type, this->source.substr(this->pos, len));
        for (size_t i = 0; i < len; i++) advance();
        return true;
    };

    if (read_symbol("->", TokenType::ARROW)) return;

    switch (peek()) {
        case '!': add_token(TokenType::NOT,       this->source.substr(this->pos, 1)); advance(); return;
        case '^': add_token(TokenType::CARET,     this->source.substr(this->pos, 1)); advance(); return;
        case '|': add_token(TokenType::PIPE,      this->source.substr(this->pos, 1)); advance(); return;
        case '(': add_token(TokenType::LPAREN,    this->source.substr(this->pos, 1)); advance(); return;
        case ')': add_token(TokenType::RPAREN,    this->source.substr(this->pos, 1)); advance(); return;
        case '*': add_token(TokenType::STAR,      this->source.substr(this->pos, 1)); advance(); return;
        case '+': add_token(TokenType::PLUS,      this->source.substr(this->pos, 1)); advance(); return;
        case '?': add_token(TokenType::QUESTION,  this->source.substr(this->pos, 1)); advance(); return;

        case '-': add_token(TokenType::DASH,      this->source.substr(this->pos, 1)); advance(); return;
        case '.': add_token(TokenType::DOT,       this->source.substr(this->pos, 1)); advance(); return;
        case ';': add_token(TokenType::SEMICOLON, this->source.substr(this->pos, 1)); advance(); return;

        case '[': add_token(TokenType::LBRACKET,  this->source.substr(this->pos, 1)); advance(); return;
        case ']': add_token(TokenType::RBRACKET,  this->source.substr(this->pos, 1)); advance(); return;

        // 3. Fallback for unexpected characters
        default:
            std::cerr << "Unexpected character: '" << peek() << "' at " << this->line << ":" << this->col << std::endl;
            advance();
            return;
    }
}



void Lexer::skip_comment_and_whitespace() {
    auto skip_whitespace = [&]() {
        while (!eof() && (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r')) {
            advance();
        }
    };

    auto skip_comment = [&]() {
        if (peek() == '/' && peek(1) == '/') {
            while (!eof() && peek() != '\n') advance();
            advance(); // Skip newline
        } else if(peek() == '/' && peek(1) == '*') {
            advance(); // Skip '/'
            advance(); // Skip '*'
            while (!eof() && !(peek() == '*' && peek(1) == '/')) {
                advance();
            }
            if (!eof()) {
                advance(); // Skip '*'
                advance(); // Skip '/'
            } else {
                std::cerr << "Unterminated comment at " << line << ":" << col << std::endl;
            }
        }
    };
    while (!eof()) {
        if (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r') {
            skip_whitespace();
        } else if (peek() == '/' && (peek(1) == '/' || peek(1) == '*')) {
            skip_comment();
        } else {
            break;
        }
    }
}



bool Lexer::eof() const {
    return this->pos >= this->source.size();
}
char Lexer::peek(size_t offset) const {
    if (this->pos + offset >= this->source.size()) return '\0';
    return this->source[this->pos + offset];
}
char Lexer::advance() {
    if (eof()) return '\0';

    char current = this->source[pos];
    
    if (current == '\n') {
        this->line++;
        this->col = 1;
    } else {
        this->col++;
    }

    this->pos++;
    return current;
}



std::vector<Token>& Lexer::getTokens(){
    return this->tokens;
}