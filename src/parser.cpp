#include "parser.hpp"
#include "ast_printer.hpp"
#include "ast.hpp"
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
    for (auto& [name, node] : parser_nodes) {
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

void Parser::debug_print() {
    ASTPrinter printer; // Create the visitor

    std::cout << "=== LEXICAL RULES ===\n";
    for(const auto& itr : lexical_nodes) {
        std::cout << "Rule: " << itr.first << "\n";
        if (itr.second) {
            itr.second->accept(printer); // Dispatch the visitor
        }
        std::cout << "\n";
    }

    std::cout << "=== PARSER RULES ===\n";
    for(const auto& itr : parser_nodes) {
        std::cout << "Rule: " << itr.first << "\n";
        if (itr.second) {
            itr.second->accept(printer); // Dispatch the visitor
        }
        std::cout << "\n";
    }
}






































// The backslashes (\) are required to make a multi-line macro!
#define PARSER_START(container, InitialFunction)                                                \
    std::string_view name = advance().lexeme;                                                   \
    consume(TokenType::ARROW, "Expecting -> after literal");                                    \
                                                                                                \
    ASTNode* root = InitialFunction();                                                          \
    consume(TokenType::SEMICOLON, "Expected ; to end statement");                               \
                                                                                                \
    auto it = container.find(name);                                                             \
    if (it == container.end()) {                                                                \
        container[name] = root;                                                                 \
    } else {                                                                                    \
        it->second = new BinaryOperation(it->second, root, BinaryOp::OR);                       \
    }


void Parser::parseLexerTokens(){
    PARSER_START(lexical_nodes,parseLexicalSequence)    
}


// Expression -> Oring ;
ASTNode* Parser::parseLexicalSequence() {
    return parseLexicalOr();
}

// Oring -> Concatnate ('|' Concatnate)* ;
ASTNode* Parser::parseLexicalOr() {
    ASTNode* left = parseLexicalConcatnate();

    while (peek().type == TokenType::PIPE) {
        advance();
        ASTNode* right = parseLexicalConcatnate();
        left = new BinaryOperation(left, right, BinaryOp::OR);
    }
    
    return left;
}

// Concatnate -> Removal (Removal)* ;
ASTNode* Parser::parseLexicalConcatnate() {
    ASTNode* left = parseLexicalRemoval();

    // Since concatenation has no explicit symbol (like A B), we loop as long 
    // as we don't hit a structural boundary that belongs to lower precedence
    // or closes an expression.
    while (!eof() && 
           peek().type != TokenType::PIPE && 
           peek().type != TokenType::SEMICOLON && 
           peek().type != TokenType::RPAREN) {
        
        ASTNode* right = parseLexicalRemoval();
        left = new BinaryOperation(left, right, BinaryOp::NEXT);
    }
    
    return left;
}

// Removal -> Unary ('^' Unary)* ;
ASTNode* Parser::parseLexicalRemoval() {
    ASTNode* left = parseLexicalUnary();

    while (peek().type == TokenType::CARET) {
        advance(); // Consume '^'
        ASTNode* right = parseLexicalUnary();
        left = new BinaryOperation(left, right, BinaryOp::XOR); 
    }
    
    return left;
}

// Unary -> '!' Unary | Unary ( '+' | '*' | '?' ) | Primary ;

UnaryOp token_to_u_op(const Token& to){
    switch (to.type){
    case TokenType::PLUS: return UnaryOp::PLUS;
    case TokenType::STAR: return UnaryOp::STAR;
    case TokenType::QUESTION: return UnaryOp::QUESTION;
    default:
        std::cerr<<"Token: "<<tokenTypeToString(to.type)<<" not an unaryoperation at "<<to.error_info.line<<"\n";
        return UnaryOp::QUESTION;
    }
}


// 1. Prefix binds tightly to the primary token
ASTNode* Parser::parseLexicalPrefix() {
    if (peek().type == TokenType::NOT) {
        advance(); 
        return new UnaryOperation(parseLexicalPrefix(), UnaryOp::NOT);
    }
    return parseLexicalPrimary();
}

// 2. Postfix applies AFTER prefix is evaluated
ASTNode* Parser::parseLexicalUnary() {
    // Start by getting the prefix/primary node
    ASTNode* node = parseLexicalPrefix();

    // Now apply any trailing modifiers
    while (peek().type == TokenType::PLUS || 
           peek().type == TokenType::STAR || 
           peek().type == TokenType::QUESTION) {
        node = new UnaryOperation(node, token_to_u_op(advance()));
    }
    
    return node;
}
// ASTNode* Parser::parseLexicalUnary() {
//     //recursively to allow !!A
//     ASTNode* node = nullptr;
//     if (peek().type == TokenType::NOT) {
//         advance(); // Consume '!'
//         node =  new UnaryOperation(parseLexicalUnary(), UnaryOp::NOT);
//     }else{
//         node = parseLexicalPrimary();
//     }

   

//     // Postfix loop (Allows stacking modifiers like A+?)
//     //Should it be ?
//     while (peek().type == TokenType::PLUS || 
//            peek().type == TokenType::STAR || 
//            peek().type == TokenType::QUESTION) {
        

//         node = new UnaryOperation(node, token_to_u_op(advance()));
//     }
    
//     return node;
// }

// Primary -> CHAR | STRING | '(' Expression ')' | '[' CHAR '-' CHAR ']' ;
ASTNode* Parser::parseLexicalPrimary() {
    Token tok = peek();

    switch (tok.type) {
        case TokenType::CHAR_LITERAL:
            advance();
            return new CharNode(unescape_char(tok.lexeme));

        case TokenType::STRING_LITERAL:
            advance();
            return new StringNode(unescape_string(tok.lexeme));

        // Grouping loops back to the very top: '(' Expression ')'
        case TokenType::LPAREN: {
            advance(); // consume '('
            ASTNode* inner = parseLexicalSequence(); 
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
            return new StringViewNode(tok.lexeme, TextNodeType::LEXICAL);


        case TokenType::PARSE_IDENTIFIER:
            std::cerr<<"Parser Variables should not be in lexer variables:\n";
            [[fallthrough]];
        default:
            std::cerr << "Parser Error: Unexpected token '" << tok<< "\n";
            advance();
            return nullptr;
    }
}





















void Parser::parseParserTokens(){
    PARSER_START(parser_nodes,parseParsingSequence)
}
#undef PARSER_START



ASTNode* Parser::parseParsingSequence(){
    return parseParsingOr();
}


ASTNode* Parser::parseParsingOr() {
    ASTNode* left = parseParsingConcatnate();

    while (peek().type == TokenType::PIPE) {
        advance();
        ASTNode* right = parseParsingConcatnate();
        left = new BinaryOperation(left, right, BinaryOp::OR);
    }
    
    return left;
}


ASTNode* Parser::parseParsingConcatnate() {
    ASTNode* left = parseParsingUnary();

    // Since concatenation has no explicit symbol (like A B), we loop as long 
    // as we don't hit a structural boundary that belongs to lower precedence
    // or closes an expression.
    while (!eof() && 
           peek().type != TokenType::PIPE && 
           peek().type != TokenType::SEMICOLON && 
           peek().type != TokenType::RPAREN) {
        
        ASTNode* right = parseParsingUnary();
        left = new BinaryOperation(left, right, BinaryOp::NEXT);
    }
    
    return left;
}


ASTNode* Parser::parseParsingUnary(){
    ASTNode* node = parseParsingPrimary();
    // Postfix loop (Allows stacking modifiers like A+?)
    //Should it be ?
    while (peek().type == TokenType::PLUS || 
           peek().type == TokenType::STAR || 
           peek().type == TokenType::QUESTION){

        node = new UnaryOperation(node, token_to_u_op(advance()));
    }
    
    return node;
}


ASTNode* Parser::parseParsingPrimary(){
    Token tok = peek();

    switch (tok.type) {
        case(TokenType::PARSE_IDENTIFIER):
            advance();
            return new StringViewNode(tok.lexeme,TextNodeType::PARSER);
        case(TokenType::TOKEN_IDENTIFIER) :
            advance();
            return new StringViewNode(tok.lexeme,TextNodeType::LEXICAL);
        case TokenType::LPAREN: {
            advance(); // consume '('
            ASTNode* inner = parseParsingSequence(); 
            consume(TokenType::RPAREN, "Expected ')' to close grouping");
            return inner;
        }

        case(TokenType::STRING_LITERAL):
        case(TokenType::CHAR_LITERAL):{
            advance();
            std::string_view content = tok.lexeme.substr(1, tok.lexeme.size() - 2);

            if (content.empty()) {
                std::cerr << "Parser Error: Empty literal not allowed in grammar rule.\n";
                return nullptr;
            }

            //Classify based on the first character, can me issue later
            if (std::isalpha(content[0]) || content[0] == '_') {
                return new StringViewNode(content,TextNodeType::KEYWORD);  // e.g., "if", "while", "_return"
            } else {
                return new StringViewNode(content,TextNodeType::SYMBOL); // e.g., "+", "==", ";", "\n"
            }
        }
        default:
            std::cerr << "Parser Error: Unexpected token in parsing rule '" << tok.lexeme << "'\n";
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
    //std::cout<<tokens[pos+offset]<<'\n';
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



