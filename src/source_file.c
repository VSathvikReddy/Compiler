#include "source_file.h"

//Returns the current char
char source_peek(const Source *src) {
    if (src->pos >= src->size) return '\0';
    return src->data[src->pos];
}

//Returns the next char
char source_peek_next(const Source *src) {
    if (src->pos + 1 >= src->size) return '\0';
    return src->data[src->pos + 1];
}

//Smartly advances the pos, line and col
char source_advance(Source *src) {
    if (src->pos >= src->size) return '\0';
    char c = src->data[src->pos++];
    if(c == '\n') {
        src->line++;
        src->column = 1;
    }else{
        src->column++;
    }
    return c;
}

//Tells if we are at the enf og the file
bool source_eof(const Source *src) {
    return src->pos >= src->size;
}
