#include "ast.h"
#include <stdlib.h>

Expr* expr_new(ExprType type, int line) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = type;
    expr->line = line;
    return expr;
}

void expr_free(Expr* expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case EXPR_LITERAL:
        case EXPR_VARIABLE:
            if (expr->data.literal) token_free(expr->data.literal);
            break;
        
        case EXPR_BINARY:
            if (expr->data.binary.left) expr_free(expr->data.binary.left);
            if (expr->data.binary.right) expr_free(expr->data.binary.right);
            break;
        
        case EXPR_UNARY:
            if (expr->data.unary.right) expr_free(expr->data.unary.right);
            break;
        
        case EXPR_CALL:
            if (expr->data.call.callee) expr_free(expr->data.call.callee);
            for (int i = 0; i < expr->data.call.arg_count; i++) {
                if (expr->data.call.arguments[i]) 
                    free(expr->data.call.arguments[i]);
            }
            if (expr->data.call.arguments) free(expr->data.call.arguments);
            break;
        
        case EXPR_LOGICAL:
            if (expr->data.logical.left) expr_free(expr->data.logical.left);
            if (expr->data.logical.right) expr_free(expr->data.logical.right);
            break;
        
        case EXPR_ASSIGNMENT:
            if (expr->data.assignment.object) expr_free(expr->data.assignment.object);
            if (expr->data.assignment.value) expr_free(expr->data.assignment.value);
            break;
        
        case EXPR_ARRAY:
            for (int i = 0; i < expr->data.array.element_count; i++) {
                if (expr->data.array.elements[i]) 
                    expr_free(expr->data.array.elements[i]);
            }
            if (expr->data.array.elements) free(expr->data.array.elements);
            break;
        
        case EXPR_INDEX:
            if (expr->data.index.object) expr_free(expr->data.index.object);
            if (expr->data.index.index) expr_free(expr->data.index.index);
            break;
        
        case EXPR_MEMBER:
            if (expr->data.index.object) expr_free(expr->data.index.object);
            if (expr->data.index.index) expr_free(expr->data.index.index);
            break;
        
        default:
            break;
    }
    
    free(expr);
}

Stmt* stmt_new(StmtType type, int line) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = type;
    stmt->line = line;
    return stmt;
}

void stmt_free(Stmt* stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_EXPRESSION:
            if (stmt->data.expression) expr_free(stmt->data.expression);
            break;
        
        case STMT_VARIABLE:
            if (stmt->data.var.name) token_free(stmt->data.var.name);
            if (stmt->data.var.initializer) expr_free(stmt->data.var.initializer);
            break;
        
        case STMT_BLOCK:
            for (int i = 0; i < stmt->data.block.statement_count; i++) {
                if (stmt->data.block.statements[i]) 
                    stmt_free(stmt->data.block.statements[i]);
            }
            if (stmt->data.block.statements) free(stmt->data.block.statements);
            break;
        
        case STMT_IF:
            if (stmt->data.if_stmt.condition) expr_free(stmt->data.if_stmt.condition);
            if (stmt->data.if_stmt.then_branch) stmt_free(stmt->data.if_stmt.then_branch);
            if (stmt->data.if_stmt.else_branch) stmt_free(stmt->data.if_stmt.else_branch);
            break;
        
        case STMT_WHILE:
            if (stmt->data.while_stmt.condition) expr_free(stmt->data.while_stmt.condition);
            if (stmt->data.while_stmt.body) stmt_free(stmt->data.while_stmt.body);
            break;
        
        case STMT_FOR:
            if (stmt->data.for_stmt.initializer) stmt_free(stmt->data.for_stmt.initializer);
            if (stmt->data.for_stmt.condition) expr_free(stmt->data.for_stmt.condition);
            if (stmt->data.for_stmt.increment) expr_free(stmt->data.for_stmt.increment);
            if (stmt->data.for_stmt.body) stmt_free(stmt->data.for_stmt.body);
            break;
        
        case STMT_FUNCTION:
            if (stmt->data.function.name) token_free(stmt->data.function.name);
            for (int i = 0; i < stmt->data.function.param_count; i++) {
                if (stmt->data.function.parameters[i])
                    token_free(stmt->data.function.parameters[i]);
            }
            if (stmt->data.function.parameters) free(stmt->data.function.parameters);
            for (int i = 0; i < stmt->data.function.body_count; i++) {
                if (stmt->data.function.body[i])
                    stmt_free(stmt->data.function.body[i]);
            }
            if (stmt->data.function.body) free(stmt->data.function.body);
            break;
        
        case STMT_RETURN:
            if (stmt->data.return_stmt.value) expr_free(stmt->data.return_stmt.value);
            break;
        
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
        
        default:
            break;
    }
    
    free(stmt);
}
