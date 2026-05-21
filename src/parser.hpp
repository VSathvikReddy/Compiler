#pragma once

#include "ast.hpp"
#include "lexer.hpp"

#include <unordered_map>

class Parser{
public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    void debug_print();
private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    std::unordered_map<std::string_view, LexicalNode*> lexical_nodes;

    void parse();

    void parseLexerTokens();
    LexicalNode* parseLexicalSequence();
    LexicalNode* parseLexicalOr();
    LexicalNode* parseLexicalConcatnate();
    LexicalNode* parseLexicalRemoval();
    LexicalNode* parseLexicalUnary();
    LexicalNode* parseLexicalPrimary();



    void parseParserTokens(){
        while (peek().type!=TokenType::SEMICOLON){
            advance();
        }
        advance();
    }




    bool eof() const;
    const Token& peek(size_t offset = 0) const;
    const Token& advance();
    bool match(TokenType type);
    const Token& consume(TokenType type, const std::string& error_message);

};
