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

    std::unordered_map<std::string_view, ASTNode*> lexical_nodes;
    std::unordered_map<std::string_view, ASTNode*> parser_nodes;

    void parse();

    void parseLexerTokens();
    ASTNode* parseLexicalSequence();
    ASTNode* parseLexicalOr();
    ASTNode* parseLexicalConcatnate();
    ASTNode* parseLexicalRemoval();
    ASTNode* parseLexicalPrefix();
    ASTNode* parseLexicalUnary();
    ASTNode* parseLexicalPrimary();



    void parseParserTokens();
    ASTNode* parseParsingSequence();
    ASTNode* parseParsingOr();
    ASTNode* parseParsingConcatnate();
    ASTNode* parseParsingUnary();
    ASTNode* parseParsingPrimary();



    bool eof() const;
    const Token& peek(size_t offset = 0) const;
    const Token& advance();
    bool match(TokenType type);
    const Token& consume(TokenType type, const std::string& error_message);

};
