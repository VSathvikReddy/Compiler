#ifndef SOURCE_H
#define SOURCE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const char *data;   // full file contents
    size_t size;        // total size in bytes

    size_t pos;         // current index
    size_t line;        // current line (1-based)
    size_t column;      // current column (1-based)
} Source;

char source_peek(const Source *src);
char source_peek_next(const Source *src);
char source_advance(Source *src);
bool source_eof(const Source *src);

#endif
