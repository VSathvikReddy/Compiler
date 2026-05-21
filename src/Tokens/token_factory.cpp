#include "token_factory.hpp"
#include "token.hpp"

#include <cstdlib>
#include <cctype>
#include <string_view>


#define STORAGE_MASK 0xC000 // Top 2 bits
#define INDEX_MASK   0x3FFF // Lower 14 bits

#define TAG_SYMBOL            0x0000 // 00
#define TAG_KEYWORD           0x4000 // 01
#define TAG_TOKEN_IDENTIFIER  0x8000 // 10
#define TAG_PARSER_IDENTIFIER 0xC000 // 11

// 1. Static Storage Allocation
std::deque<std::string> TokenFactory::symbol_storage;
std::deque<std::string> TokenFactory::keywords_storage;
std::deque<std::string> TokenFactory::token_identifier_storage;
std::deque<std::string> TokenFactory::parser_identifer_storage;
std::unordered_map<std::string_view, uint16_t> TokenFactory::view_to_id;

// Helper utility to check if a literal is entirely alphabetic keywords (like "if") 
// or punctuation symbols (like "==")
static bool isAlphaString(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isalpha(c)) return false;
    }
    return true;
}

uint16_t TokenFactory::addNewString(TokenType type, const std::string& lexeme) {
    uint16_t index = 0;
    uint16_t storage_tag = 0;
    std::string* permanent_ptr = nullptr;

    // Direct routing split logic based on the Lexer classification tokens
    auto check_push = [&lexeme, &index, &storage_tag, &permanent_ptr](std::deque<std::string> &storage, uint16_t mask){
        storage.push_back(lexeme);
        index = static_cast<uint16_t>(storage.size() - 1);
        storage_tag = mask;
        permanent_ptr = &storage.back();
    };

    if (type == TokenType::TOKEN_IDENTIFIER) {
        check_push(token_identifier_storage,TAG_TOKEN_IDENTIFIER);
    } 
    else if (type == TokenType::PARSE_IDENTIFIER) {
        check_push(parser_identifer_storage,TAG_PARSER_IDENTIFIER);
    } 
    else if (type == TokenType::STRING_LITERAL) {
        if (isAlphaString(lexeme)) {
            check_push(keywords_storage,TAG_KEYWORD);
        } else {
            check_push(symbol_storage,TAG_SYMBOL);
        }
    }

    uint16_t tagged_id = storage_tag | (index & INDEX_MASK);
    
    std::string_view permanent_view(*permanent_ptr);
    view_to_id[permanent_view] = tagged_id;

    return tagged_id;
}

Token TokenFactory::create(TokenType type, const std::string& lexeme, uint8_t line) {
    std::string_view lookup_view(lexeme);

    auto it = view_to_id.find(lookup_view);
    if (it != view_to_id.end()) {
        return Token(type, it->second, line);
    }

    return Token(type, addNewString(type, lexeme), line);
}

const std::string& TokenFactory::get(const Token& token) {
    uint16_t storage_tag = token.value & STORAGE_MASK;
    uint16_t index = token.value & INDEX_MASK;

    switch (storage_tag) {
        case TAG_SYMBOL:
            if (index >= symbol_storage.size()) exit(1);
            return symbol_storage[index];
            
        case TAG_KEYWORD:
            if (index >= keywords_storage.size()) exit(1);
            return keywords_storage[index];
            
        case TAG_TOKEN_IDENTIFIER:
            if (index >= token_identifier_storage.size()) exit(1);
            return token_identifier_storage[index];
            
        case TAG_PARSER_IDENTIFIER:
            if (index >= parser_identifer_storage.size()) exit(1);
            return parser_identifer_storage[index];
            
        default:
            exit(1);
    }
}



#undef STORAGE_MASK
#undef INDEX_MASK

#undef TAG_SYMBOL 
#undef TAG_KEYWORD        
#undef TAG_TOKEN_IDENTIFIER 
#undef TAG_PARSER_IDENTIFIER