#pragma once

#include <cstdint>
#include <string>

#include <ostream>

enum class TokenType : uint8_t {
    PARSE_IDENTIFIER,
    TOKEN_IDENTIFIER,

    STRING_LITERAL,
    CHAR_LITERAL,
    NUMBER_LITERAL,

    ARROW,

    NOT,
    CARET,
    PIPE,
    LPAREN,
    RPAREN,
    

    STAR,
    PLUS,
    QUESTION,
    
    LBRACKET,
    RBRACKET,
    DASH,
    DOT,

    SEMICOLON,
    END_OF_FILE
};

inline std::string tokenTypeToString(TokenType type){
    switch(type){
        case TokenType::PARSE_IDENTIFIER: return "PARSE_IDENTIFIER";
        case TokenType::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
        case TokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";

        case TokenType::ARROW: return "->";

        case TokenType::NOT: return "!";
        case TokenType::CARET: return "^";
        case TokenType::PIPE: return "|";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";

        case TokenType::STAR: return "*";
        case TokenType::PLUS: return "+";
        case TokenType::QUESTION: return "?";

        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::DASH: return "-";
        case TokenType::DOT: return ".";
        
        case TokenType::SEMICOLON: return ";";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

inline bool isLiteral(TokenType type) {
    return type == TokenType::CHAR_LITERAL || type == TokenType::NUMBER_LITERAL ||type == TokenType::STRING_LITERAL;
}
inline bool isIdentifier(TokenType type) {
    return type == TokenType::PARSE_IDENTIFIER || type == TokenType::TOKEN_IDENTIFIER;
}
inline bool isSymbol(TokenType type) {
    return type == TokenType::ARROW || type == TokenType::NOT || type == TokenType::CARET || type == TokenType::PIPE || type == TokenType::LPAREN || type == TokenType::RPAREN || type == TokenType::STAR || type == TokenType::PLUS || type == TokenType::QUESTION || type == TokenType::LBRACKET || type == TokenType::RBRACKET || type == TokenType::DASH || type == TokenType::SEMICOLON;
}

struct ErrorInfo{
    int line;
    int col;
    std::string_view file_name;
};

struct Token {
    TokenType type;
    std::string_view lexeme;

    ErrorInfo error_info;


    Token(TokenType type, std::string_view lexeme, int line, int col, std::string_view file_name)
        : type(type), lexeme(lexeme), error_info({line, col, file_name}) {}
};  

inline std::ostream& operator<<(std::ostream& os, const Token& token) {
    if(isLiteral(token.type) || isIdentifier(token.type)){
        os << "[" << tokenTypeToString(token.type) 
        << " | " << token.lexeme << " ] " ;     
    }else{
        os << "[ " << tokenTypeToString(token.type) << " ]";
    }

	   //<< token.error_info.file_name << "@" << token.error_info.line << ":" << token.error_info.col << "]";
	return os;
}


