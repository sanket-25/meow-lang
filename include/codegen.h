#ifndef MEOW_CODEGEN_H
#define MEOW_CODEGEN_H

#include "ast.h"
#include <stdio.h>

typedef struct {
    FILE* output;
    int indent_level;
    char** globals;
    int global_count;
    char** locals;
    int local_count;
} CodeGen;

CodeGen* codegen_new(FILE* output);
void codegen_free(CodeGen* codegen);
void codegen_generate(CodeGen* codegen, Stmt** statements, int stmt_count);

#endif
