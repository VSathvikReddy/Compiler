#ifndef TOKEN_H
#define TOKEN_H

#include "string_view.h"

typedef enum{
    //Special
    TOKEN_EOF,
    TOKEN_ERROR,

    //Identifiers & literals
    TOKEN_IDENTIFIER,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,

    //Keywords
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_VOID,

    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RETURN,
    TOKEN_BREAK,
    TOKEN_CONTINUE,

    //Operators
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_STAR,        // *
    TOKEN_SLASH,       // /
    TOKEN_PERCENT,     // %

    TOKEN_ASSIGN,      // =
    TOKEN_EQ,          // ==
    TOKEN_NEQ,         // !=
    TOKEN_LT,          // <
    TOKEN_GT,          // >
    TOKEN_LE,          // <=
    TOKEN_GE,          // >=

    TOKEN_AND,         // &&
    TOKEN_OR,          // ||
    TOKEN_NOT,         // !

    TOKEN_INC,         // ++
    TOKEN_DEC,         // --

    //Delimiters
    TOKEN_LPAREN,      // (
    TOKEN_RPAREN,      // )
    TOKEN_LBRACE,      // {
    TOKEN_RBRACE,      // }
    TOKEN_LBRACKET,    // [
    TOKEN_RBRACKET,    // ]

    TOKEN_SEMICOLON,   // ;
    TOKEN_COMMA        // ,
}Token_Type;

typedef struct {
    Token_Type type;
    union {
        unsigned int ID;
        int int_val;
        double float_val;
    } value;
    int line;
    int column;
} Token;


#endif