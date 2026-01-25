#ifndef MEOW_PARSER_H
#define MEOW_PARSER_H

#include "token.h"
#include "ast.h"
#include "lexer.h"

typedef struct {
    Token** tokens;
    int token_count;
    int current;
} Parser;

Parser* parser_new(const char* source);
void parser_free(Parser* parser);
Stmt** parser_parse(Parser* parser, int* stmt_count);

#endif
