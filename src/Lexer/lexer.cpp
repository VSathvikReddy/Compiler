#include "lexer.hpp"
#include "token_factory.hpp"
#include <iostream>
#include <cctype>

Lexer::Lexer(const char* file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open grammar specification file: " << file_path << std::endl;
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

void Lexer::add_token(TokenType type, const std::string& lexeme) {
    this->tokens.push_back(TokenFactory::create(type, lexeme, static_cast<uint8_t>(line)));
}

void Lexer::add_token(TokenType type, uint16_t value) {
    this->tokens.emplace_back(type, value, static_cast<uint8_t>(line));
}

void Lexer::add_token(TokenType type) {
    this->tokens.emplace_back(type, 0, static_cast<uint8_t>(line));
}





void Lexer::tokenize() {
    while (!eof()) {
        skip_comment_and_whitespace();
        if (eof()) break;

        char c = peek();

        if (isalpha(c) || c == '_') {
            read_identifiers();
        }else if (c == '"') {
            read_string_literal();
        }else {
            read_symbols();
        }
    }
}

void Lexer::read_identifiers() {
    size_t start = pos;
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }
    
    std::string lexeme(this->source.substr(start, pos - start));

    // TRIAGE RULE: Capital letters are Lexer Token Identifiers (e.g., NUMBER, IDENT)
    // Lowercase / CamelCase are Parser Rules Identifiers (e.g., expression, statement)
    bool is_all_upper = true;
    for (char c : lexeme) {
        if (isalpha(c) && !isupper(c)) {
            is_all_upper = false;
            break;
        }
    }

    TokenType deduced_type = is_all_upper ? TokenType::TOKEN_IDENTIFIER : TokenType::PARSE_IDENTIFIER;
    add_token(deduced_type, lexeme);
}

void Lexer::read_string_literal() {
    advance(); // Skip opening '"'
    size_t start = pos;

    while (!eof() && peek() != '"') {
        if (peek() == '\\') { 
            advance(); // Consume escape character sequence backslash
        }
        advance();
    }

    if (eof()) {
        std::cerr << "Lexical Error: Unterminated match string definition at line " << line << std::endl;
        return;
    }

    std::string lexeme(this->source.substr(start, pos - start));
    advance(); // Skip closing '"'

    add_token(TokenType::STRING_LITERAL, lexeme);
}

void Lexer::read_symbols() {
    char current = peek();

    // Lookahead check for multi-character operators ("->")
    if (current == '-' && peek(1) == '>') {
        add_token(TokenType::ARROW, static_cast<uint16_t>('>'));
        advance();
        advance();
        return;
    }

    // Direct structural mappings against the custom char enum values
    switch (current) {
        case '|': add_token(TokenType::PIPE, static_cast<uint16_t>('|')); advance(); return;
        case '(': add_token(TokenType::LPAREN, static_cast<uint16_t>('(')); advance(); return;
        case ')': add_token(TokenType::RPAREN, static_cast<uint16_t>(')')); advance(); return;
        case ';': add_token(TokenType::SEMICOLON, static_cast<uint16_t>(';')); advance(); return;
        case '*': add_token(TokenType::STAR, static_cast<uint16_t>('*')); advance(); return;
        case '+': add_token(TokenType::PLUS, static_cast<uint16_t>('+')); advance(); return;
        case '?': add_token(TokenType::QUESTION, static_cast<uint16_t>('?')); advance(); return;
        case '[': add_token(TokenType::LBRACKET, static_cast<uint16_t>('[')); advance(); return;
        case ']': add_token(TokenType::RBRACKET, static_cast<uint16_t>(']')); advance(); return;
        case '-': add_token(TokenType::DASH, static_cast<uint16_t>('-')); advance(); return;
        default:
            std::cerr << "Lexical Error: Unrecognized structural character '" << current << "' at line " << line << std::endl;
            advance();
            break;
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
                std::cerr << "Unterminated comment at " << line << std::endl;
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
    }

    this->pos++;
    return current;
}
std::vector<Token>& Lexer::getTokens() {
    return this->tokens;
}