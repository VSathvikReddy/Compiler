#ifndef LEXTER_H
#define LEXTER_H

#include "source_file.h"
#include "token.h"

void skip_whitespace(Source *src);
Token scan_number(Source *src);



#endif