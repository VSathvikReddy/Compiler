#include "parser.hpp"
#include "help.hpp"
#include <iostream>


namespace {
    char unescape_char(std::string_view lexeme) {
        if (lexeme.size() <= 2) return '\0';
        if (lexeme[1] != '\\') return lexeme[1];
        
        if (lexeme.size() > 2) {
            switch (lexeme[2]) {
                case 'n': return '\n'; case 't': return '\t';
                case 'r': return '\r'; case 'b': return '\b';
                case '0': return '\0'; case '\\': return '\\';
                case '\'': return '\''; case '"': return '"';
                default: return lexeme[2]; 
            }
        }
        return '\0';
    }

    std::string unescape_string(std::string_view lexeme) {
        if (lexeme.size() <= 2) return "";
        std::string_view content = lexeme.substr(1, lexeme.size() - 2);
        std::string result;
        result.reserve(content.size());

        for (size_t i = 0; i < content.size(); ++i) {
            if (content[i] == '\\' && i + 1 < content.size()) {
                ++i;
                switch (content[i]) {
                    case 'n': result += '\n'; break; case 't': result += '\t'; break;
                    case 'r': result += '\r'; break; case '\\': result += '\\'; break;
                    case '"': result += '"'; break;  default: result += content[i]; break;
                }
            } else {
                result += content[i];
            }
        }
        return result;
    }
}


Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    parse();
}

Parser::~Parser() {
    for (auto& [name, node] : lexical_nodes) {
        delete node;
    }
}

void Parser::parse(){
    while (!eof()){
        if(peek().type==TokenType::PARSE_IDENTIFIER){
            parseParserTokens();
        }else if(peek().type == TokenType::TOKEN_IDENTIFIER){
            parseLexerTokens();
        }else{
            std::cerr<<"Unexpected token: "<<advance()<<'\n';
        }
    }
    
}

void Parser::debug_print(){
    for(const auto& itr:lexical_nodes){
        std::cout<<itr.first<<":\n";
        print_ast(itr.second);
        std::cout<<"\n\n";
    }
}

void Parser::parseLexerTokens(){
    std::string_view name = advance().lexeme;
    consume(TokenType::ARROW,"Expecting -> after litral");

    LexicalNode* root = parseLexicalSequence();
    consume(TokenType::SEMICOLON,"Expected ; to end statment");
    
    auto it = lexical_nodes.find(name);
    if(it == lexical_nodes.end()){
        lexical_nodes[name] =  root;
    }else{
        it->second = new BinaryLexicalOperation(it->second,root,BinaryLexicalOp::OR);
    }
}

// Expression -> Oring ;
LexicalNode* Parser::parseLexicalSequence() {
    return parseLexicalOr();
}

// Oring -> Concatnate ('|' Concatnate)* ;
LexicalNode* Parser::parseLexicalOr() {
    LexicalNode* left = parseLexicalConcatnate();

    while (peek().type == TokenType::PIPE) {
        advance();
        LexicalNode* right = parseLexicalConcatnate();
        left = new BinaryLexicalOperation(left, right, BinaryLexicalOp::OR);
    }
    
    return left;
}

// Concatnate -> Removal (Removal)* ;
LexicalNode* Parser::parseLexicalConcatnate() {
    LexicalNode* left = parseLexicalRemoval();

    // Since concatenation has no explicit symbol (like A B), we loop as long 
    // as we don't hit a structural boundary that belongs to lower precedence
    // or closes an expression.
    while (!eof() && 
           peek().type != TokenType::PIPE && 
           peek().type != TokenType::SEMICOLON && 
           peek().type != TokenType::RPAREN) {
        
        LexicalNode* right = parseLexicalRemoval();
        left = new BinaryLexicalOperation(left, right, BinaryLexicalOp::NEXT);
    }
    
    return left;
}

// Removal -> Unary ('^' Unary)* ;
LexicalNode* Parser::parseLexicalRemoval() {
    LexicalNode* left = parseLexicalUnary();

    while (peek().type == TokenType::CARET) {
        advance(); // Consume '^'
        LexicalNode* right = parseLexicalUnary();
        left = new BinaryLexicalOperation(left, right, BinaryLexicalOp::XOR); 
    }
    
    return left;
}

// Unary -> '!' Unary | Unary ( '+' | '*' | '?' ) | Primary ;

UnaryLexicalOp token_to_u_op(const Token& to){
    switch (to.type){
    case TokenType::PLUS: return UnaryLexicalOp::PLUS;
    case TokenType::STAR: return UnaryLexicalOp::STAR;
    case TokenType::QUESTION: return UnaryLexicalOp::QUESTION;
    default:
        std::cerr<<"Token: "<<tokenTypeToString(to.type)<<" not an unaryoperation at "<<to.error_info.line<<"\n";
        return UnaryLexicalOp::QUESTION;
    }
}
LexicalNode* Parser::parseLexicalUnary() {
    //recursively to allow !!A
    LexicalNode* node = nullptr;
    if (peek().type == TokenType::NOT) {
        advance(); // Consume '!'
        node =  new UnaryLexicalOperation(parseLexicalUnary(), UnaryLexicalOp::NOT);
    }else{
        node = parseLexicalPrimary();
    }

   

    // Postfix loop (Allows stacking modifiers like A+?)
    //Should it be ?
    while (peek().type == TokenType::PLUS || 
           peek().type == TokenType::STAR || 
           peek().type == TokenType::QUESTION) {
        

        node = new UnaryLexicalOperation(node, token_to_u_op(advance()));
    }
    
    return node;
}

// Primary -> CHAR | STRING | '(' Expression ')' | '[' CHAR '-' CHAR ']' ;
LexicalNode* Parser::parseLexicalPrimary() {
    Token tok = peek();

    switch (tok.type) {
        case TokenType::CHAR_LITERAL:
            advance();
            return new CharLiteralNode(unescape_char(tok.lexeme));

        case TokenType::STRING_LITERAL:
            advance();
            return new StringLiteralNode(unescape_string(tok.lexeme));

        // Grouping loops back to the very top: '(' Expression ')'
        case TokenType::LPAREN: {
            advance(); // consume '('
            LexicalNode* inner = parseLexicalSequence(); 
            consume(TokenType::RPAREN, "Expected ')' to close grouping");
            return inner;
        }

        // Bracket Ranges: '[' CHAR '-' CHAR ']'
        case TokenType::LBRACKET: {
            advance(); // consume '['
            char low = unescape_char(consume(TokenType::CHAR_LITERAL, "Expected starting char in range").lexeme);
            consume(TokenType::DASH, "Expected '-' in character range");
            char high = unescape_char(consume(TokenType::CHAR_LITERAL, "Expected ending char in range").lexeme);
            consume(TokenType::RBRACKET, "Expected ']' to close range");
            return new RangeNode(low, high);
        }

        /* NOTE: I kept identifiers and the '.' wildcard in the fallback so 
           your compiler can still reference other rules (like A -> INTEGER)
           even though they were omitted from the strict text prompt above.
        */
        case TokenType::DOT:
            advance();
            return new WildcardNode();

        case TokenType::TOKEN_IDENTIFIER:
            advance();
            return new RuleReferenceNode(tok.lexeme);


        case TokenType::PARSE_IDENTIFIER:
            std::cerr<<"Parser Variables should not be in lexer variables:\n";
            [[fallthrough]];
        default:
            std::cerr << "Parser Error: Unexpected token '" << tok<< "\n";
            advance();
            return nullptr;
    }
}




bool Parser::eof() const {
    return pos >= tokens.size() || tokens[pos].type == TokenType::END_OF_FILE; 
}

static Token end_token(TokenType::END_OF_FILE,"",0,0,"");
const Token& Parser::peek(size_t offset) const {
    if (pos + offset >= tokens.size()) {
        return end_token;
    }
    return tokens[pos + offset];
}

const Token& Parser::advance() {
    if (eof()) {
        return end_token;
    }
    return tokens[pos++];
}
bool Parser::match(TokenType type) {
    if (!eof() && peek().type == type) {
        advance();
        return true;
    }
    return false;
}

const Token& Parser::consume(TokenType type, const std::string& error_message) {
    if (!eof() && peek().type == type) {
        return advance();
    }
    std::cerr<<"Parser Error: " + error_message + " at line " << peek().error_info.line<<std::endl;
    return peek();
}