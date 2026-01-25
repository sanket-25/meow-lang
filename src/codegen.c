#include "codegen.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>

CodeGen* codegen_new(FILE* output) {
    CodeGen* gen = (CodeGen*)malloc(sizeof(CodeGen));
    gen->output = output;
    gen->indent_level = 0;
    gen->globals = (char**)malloc(sizeof(char*) * 1000);
    gen->global_count = 0;
    gen->locals = (char**)malloc(sizeof(char*) * 1000);
    gen->local_count = 0;
    return gen;
}

void codegen_free(CodeGen* gen) {
    if (gen) {
        for (int i = 0; i < gen->global_count; i++) {
            if (gen->globals[i]) free(gen->globals[i]);
        }
        free(gen->globals);
        for (int i = 0; i < gen->local_count; i++) {
            if (gen->locals[i]) free(gen->locals[i]);
        }
        free(gen->locals);
        free(gen);
    }
}

static void indent(CodeGen* gen) {
    for (int i = 0; i < gen->indent_level; i++) {
        fprintf(gen->output, "    ");
    }
}

static void add_variable(CodeGen* gen, const char* name, int is_global) {
    if (is_global) {
        gen->globals[gen->global_count] = (char*)malloc(strlen(name) + 1);
        strcpy(gen->globals[gen->global_count], name);
        gen->global_count++;
    } else {
        gen->locals[gen->local_count] = (char*)malloc(strlen(name) + 1);
        strcpy(gen->locals[gen->local_count], name);
        gen->local_count++;
    }
}

static int is_variable_defined(CodeGen* gen, const char* name) {
    for (int i = 0; i < gen->global_count; i++) {
        if (strcmp(gen->globals[i], name) == 0) return 1;
    }
    for (int i = 0; i < gen->local_count; i++) {
        if (strcmp(gen->locals[i], name) == 0) return 1;
    }
    return 0;
}

static void codegen_expr(CodeGen* gen, Expr* expr);
static void codegen_stmt(CodeGen* gen, Stmt* stmt);

static void codegen_expr(CodeGen* gen, Expr* expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case EXPR_LITERAL:
            if (expr->data.literal->type == TOKEN_NUMBER) {
                fprintf(gen->output, "%g", expr->data.literal->value.number);
            } else if (expr->data.literal->type == TOKEN_STRING) {
                fprintf(gen->output, "\"%s\"", expr->data.literal->lexeme);
            } else if (expr->data.literal->type == TOKEN_TRUE) {
                fprintf(gen->output, "1");
            } else if (expr->data.literal->type == TOKEN_FALSE) {
                fprintf(gen->output, "0");
            } else if (expr->data.literal->type == TOKEN_NULL) {
                fprintf(gen->output, "NULL");
            }
            break;
        
        case EXPR_VARIABLE:
            fprintf(gen->output, "%s", expr->data.variable->lexeme);
            break;
        
        case EXPR_BINARY: {
            fprintf(gen->output, "(");
            codegen_expr(gen, expr->data.binary.left);
            
            switch (expr->data.binary.operator->type) {
                case TOKEN_PLUS: fprintf(gen->output, " + "); break;
                case TOKEN_MINUS: fprintf(gen->output, " - "); break;
                case TOKEN_STAR: fprintf(gen->output, " * "); break;
                case TOKEN_SLASH: fprintf(gen->output, " / "); break;
                case TOKEN_PERCENT: fprintf(gen->output, " %% "); break;
                case TOKEN_LESS: fprintf(gen->output, " < "); break;
                case TOKEN_LESS_EQUAL: fprintf(gen->output, " <= "); break;
                case TOKEN_GREATER: fprintf(gen->output, " > "); break;
                case TOKEN_GREATER_EQUAL: fprintf(gen->output, " >= "); break;
                case TOKEN_EQUAL_EQUAL: fprintf(gen->output, " == "); break;
                case TOKEN_BANG_EQUAL: fprintf(gen->output, " != "); break;
                case TOKEN_AMPERSAND: fprintf(gen->output, " & "); break;
                case TOKEN_PIPE: fprintf(gen->output, " | "); break;
                case TOKEN_CARET: fprintf(gen->output, " ^ "); break;
                case TOKEN_LSHIFT: fprintf(gen->output, " << "); break;
                case TOKEN_RSHIFT: fprintf(gen->output, " >> "); break;
                default: break;
            }
            
            codegen_expr(gen, expr->data.binary.right);
            fprintf(gen->output, ")");
            break;
        }
        
        case EXPR_UNARY: {
            switch (expr->data.unary.operator->type) {
                case TOKEN_MINUS: fprintf(gen->output, "-"); break;
                case TOKEN_PLUS: fprintf(gen->output, "+"); break;
                case TOKEN_BANG: fprintf(gen->output, "!"); break;
                case TOKEN_TILDE: fprintf(gen->output, "~"); break;
                default: break;
            }
            codegen_expr(gen, expr->data.unary.right);
            break;
        }
        
        case EXPR_LOGICAL: {
            fprintf(gen->output, "(");
            codegen_expr(gen, expr->data.logical.left);
            if (expr->data.logical.operator->type == TOKEN_AND) {
                fprintf(gen->output, " && ");
            } else {
                fprintf(gen->output, " || ");
            }
            codegen_expr(gen, expr->data.logical.right);
            fprintf(gen->output, ")");
            break;
        }
        
        case EXPR_ASSIGNMENT: {
            codegen_expr(gen, expr->data.assignment.object);
            fprintf(gen->output, " = ");
            codegen_expr(gen, expr->data.assignment.value);
            break;
        }
        
        case EXPR_ARRAY: {
            fprintf(gen->output, "{");
            for (int i = 0; i < expr->data.array.element_count; i++) {
                if (i > 0) fprintf(gen->output, ", ");
                codegen_expr(gen, expr->data.array.elements[i]);
            }
            fprintf(gen->output, "}");
            break;
        }
        
        case EXPR_INDEX: {
            codegen_expr(gen, expr->data.index.object);
            fprintf(gen->output, "[");
            codegen_expr(gen, expr->data.index.index);
            fprintf(gen->output, "]");
            break;
        }
        
        case EXPR_CALL: {
            // Check if it's a print call
            if (expr->data.call.callee->type == EXPR_VARIABLE && 
                strcmp(expr->data.call.callee->data.variable->lexeme, "print") == 0) {
                fprintf(gen->output, "meow_print(");
                for (int i = 0; i < expr->data.call.arg_count; i++) {
                    if (i > 0) fprintf(gen->output, ", ");
                    codegen_expr(gen, expr->data.call.arguments[i]);
                }
                fprintf(gen->output, ")");
            } else {
                // Regular function call
                codegen_expr(gen, expr->data.call.callee);
                fprintf(gen->output, "(");
                for (int i = 0; i < expr->data.call.arg_count; i++) {
                    if (i > 0) fprintf(gen->output, ", ");
                    codegen_expr(gen, expr->data.call.arguments[i]);
                }
                fprintf(gen->output, ")");
            }
            break;
        }
        
        default:
            break;
    }
}

static void codegen_stmt(CodeGen* gen, Stmt* stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_EXPRESSION:
            indent(gen);
            codegen_expr(gen, stmt->data.expression);
            fprintf(gen->output, ";\n");
            break;
        
        case STMT_VARIABLE:
            indent(gen);
            fprintf(gen->output, "long long %s", stmt->data.var.name->lexeme);
            add_variable(gen, stmt->data.var.name->lexeme, 0);
            
            if (stmt->data.var.initializer) {
                fprintf(gen->output, " = ");
                codegen_expr(gen, stmt->data.var.initializer);
            }
            fprintf(gen->output, ";\n");
            break;
        
        case STMT_BLOCK:
            indent(gen);
            fprintf(gen->output, "{\n");
            gen->indent_level++;
            for (int i = 0; i < stmt->data.block.statement_count; i++) {
                codegen_stmt(gen, stmt->data.block.statements[i]);
            }
            gen->indent_level--;
            indent(gen);
            fprintf(gen->output, "}\n");
            break;
        
        case STMT_IF:
            indent(gen);
            fprintf(gen->output, "if (");
            codegen_expr(gen, stmt->data.if_stmt.condition);
            fprintf(gen->output, ")\n");
            codegen_stmt(gen, stmt->data.if_stmt.then_branch);
            if (stmt->data.if_stmt.else_branch) {
                indent(gen);
                fprintf(gen->output, "else\n");
                codegen_stmt(gen, stmt->data.if_stmt.else_branch);
            }
            break;
        
        case STMT_WHILE:
            indent(gen);
            fprintf(gen->output, "while (");
            codegen_expr(gen, stmt->data.while_stmt.condition);
            fprintf(gen->output, ")\n");
            codegen_stmt(gen, stmt->data.while_stmt.body);
            break;
        
        case STMT_FOR:
            indent(gen);
            fprintf(gen->output, "for (");
            if (stmt->data.for_stmt.initializer) {
                if (stmt->data.for_stmt.initializer->type == STMT_VARIABLE) {
                    fprintf(gen->output, "long long %s", stmt->data.for_stmt.initializer->data.var.name->lexeme);
                    add_variable(gen, stmt->data.for_stmt.initializer->data.var.name->lexeme, 0);
                    if (stmt->data.for_stmt.initializer->data.var.initializer) {
                        fprintf(gen->output, " = ");
                        codegen_expr(gen, stmt->data.for_stmt.initializer->data.var.initializer);
                    }
                } else {
                    codegen_expr(gen, stmt->data.for_stmt.initializer->data.expression);
                }
            }
            fprintf(gen->output, "; ");
            if (stmt->data.for_stmt.condition) {
                codegen_expr(gen, stmt->data.for_stmt.condition);
            }
            fprintf(gen->output, "; ");
            if (stmt->data.for_stmt.increment) {
                codegen_expr(gen, stmt->data.for_stmt.increment);
            }
            fprintf(gen->output, ")\n");
            codegen_stmt(gen, stmt->data.for_stmt.body);
            break;
        
        case STMT_FUNCTION:
            fprintf(gen->output, "long long %s(", stmt->data.function.name->lexeme);
            for (int i = 0; i < stmt->data.function.param_count; i++) {
                if (i > 0) fprintf(gen->output, ", ");
                fprintf(gen->output, "long long %s", stmt->data.function.parameters[i]->lexeme);
                add_variable(gen, stmt->data.function.parameters[i]->lexeme, 0);
            }
            fprintf(gen->output, ") {\n");
            gen->indent_level++;
            for (int i = 0; i < stmt->data.function.body_count; i++) {
                codegen_stmt(gen, stmt->data.function.body[i]);
            }
            gen->indent_level--;
            indent(gen);
            fprintf(gen->output, "}\n\n");
            break;
        
        case STMT_RETURN:
            indent(gen);
            fprintf(gen->output, "return");
            if (stmt->data.return_stmt.value) {
                fprintf(gen->output, " ");
                codegen_expr(gen, stmt->data.return_stmt.value);
            }
            fprintf(gen->output, ";\n");
            break;
        
        case STMT_BREAK:
            indent(gen);
            fprintf(gen->output, "break;\n");
            break;
        
        case STMT_CONTINUE:
            indent(gen);
            fprintf(gen->output, "continue;\n");
            break;
        
        default:
            break;
    }
}

void codegen_generate(CodeGen* gen, Stmt** statements, int stmt_count) {
    // Write C header
    fprintf(gen->output, "#include <stdio.h>\n");
    fprintf(gen->output, "#include <stdlib.h>\n");
    fprintf(gen->output, "#include <string.h>\n");
    fprintf(gen->output, "#include <math.h>\n\n");
    
    // Write meow standard library functions
    fprintf(gen->output, "void meow_print(long long value) {\n");
    fprintf(gen->output, "    printf(\"%%lld\\n\", value);\n");
    fprintf(gen->output, "}\n\n");
    
    fprintf(gen->output, "void meow_print_str(const char* str) {\n");
    fprintf(gen->output, "    printf(\"%%s\\n\", str);\n");
    fprintf(gen->output, "}\n\n");
    
    // Generate main function
    fprintf(gen->output, "int main() {\n");
    gen->indent_level = 1;
    
    for (int i = 0; i < stmt_count; i++) {
        codegen_stmt(gen, statements[i]);
    }
    
    indent(gen);
    fprintf(gen->output, "return 0;\n");
    fprintf(gen->output, "}\n");
}
