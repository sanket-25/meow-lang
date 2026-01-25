#ifndef MEOW_LEXER_H
#define MEOW_LEXER_H

#include "token.h"

typedef struct {
    const char* source;
    int current;
    int line;
    int column;
    int start;
} Lexer;

Lexer* lexer_new(const char* source);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);

#endif
