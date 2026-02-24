#include <ctype.h>

#include "lexer.h"

void skip_whitespace(Source *src) {
    for (;;){
        char peek = source_peek(src);

        if (peek == ' ' || peek == '\t' || peek == '\r') {
            source_advance(src);   // do nothing, just consume
        }else if (peek == '\n') {
            source_advance(src);   // line++ happens inside advance()
        }else {
            break;
        }
    }
}

Token scan_number(Source *src){
    
    size_t start = src->pos;
    int line = src->line;
    int column = src->column;
    int v =0;
    do{
        v = v*10 + (source_peek(src))-'0';
        source_advance(src);
    }while(isdigit(source_peek(src)));
    
    size_t length = src->pos - start;

    return (Token){
        .type = TOKEN_INT_LITERAL,

        .value.int_val = v,
        .line = line,
        .column = column
    };
        
}