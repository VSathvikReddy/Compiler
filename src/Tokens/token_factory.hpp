#pragma once

#include <deque>
#include <unordered_map>
#include <string>
#include <cstdint>

struct Token;
enum class TokenType: uint8_t;

class TokenFactory {
private:
    static std::deque<std::string> symbol_storage;
    static std::deque<std::string> keywords_storage;
    static std::deque<std::string> token_identifier_storage;
    static std::deque<std::string> parser_identifer_storage;
    
    static std::unordered_map<std::string_view, uint16_t> view_to_id;


    static uint16_t addNewString(TokenType type, const std::string& lexeme);

public:
    TokenFactory() = delete; // Static factory layout

    static Token create(TokenType type, const std::string& lexeme, uint8_t line);
    static const std::string& get(const Token& token);

    // Getters to access the categorized arrays directly for your meta-parser generator loop
    static const std::deque<std::string>& getSymbols() { return symbol_storage; }
    static const std::deque<std::string>& getKeywords() { return keywords_storage; }
    static const std::deque<std::string>& getTokenIdentifiers() { return token_identifier_storage; }
    static const std::deque<std::string>& getParserIdentifiers() { return parser_identifer_storage; }
};