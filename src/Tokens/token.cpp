#include "token.hpp"
#include "token_factory.hpp"

#include <ostream>
// 2. Helper Functions Implementation
bool isLiteral(TokenType type) {
    return type == TokenType::CHAR_LITERAL || type == TokenType::PARSE_IDENTIFIER ||type == TokenType::TOKEN_IDENTIFIER || type == TokenType::STRING_LITERAL;
}

std::ostream& operator<<(std::ostream& os, const Token& token){
    if(isLiteral(token.type)){
        os << "Token(" << static_cast<int>(token.type) << ", \"" << TokenFactory::get(token) << "\", " << token.line << ")";
    }else{
        os << "Token(" << static_cast<char>(token.type) << ", " << token.value << ", " << token.line << ")";
    }
    return os;
}
