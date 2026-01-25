#ifndef MEOW_AST_H
#define MEOW_AST_H

#include "token.h"

typedef struct Expr Expr;
typedef struct Stmt Stmt;

typedef enum {
    EXPR_LITERAL,
    EXPR_VARIABLE,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_CALL,
    EXPR_LOGICAL,
    EXPR_ASSIGNMENT,
    EXPR_ARRAY,
    EXPR_INDEX,
    EXPR_MEMBER
} ExprType;

typedef enum {
    STMT_EXPRESSION,
    STMT_VARIABLE,
    STMT_BLOCK,
    STMT_IF,
    STMT_WHILE,
    STMT_FOR,
    STMT_FUNCTION,
    STMT_RETURN,
    STMT_BREAK,
    STMT_CONTINUE
} StmtType;

typedef struct {
    Token* operator;
    Expr* left;
    Expr* right;
} BinaryExpr;

typedef struct {
    Token* operator;
    Expr* right;
} UnaryExpr;

typedef struct {
    Expr* callee;
    Expr** arguments;
    int arg_count;
} CallExpr;

typedef struct {
    Token* operator;
    Expr* left;
    Expr* right;
} LogicalExpr;

typedef struct {
    Expr* object;
    Expr* value;
} AssignmentExpr;

typedef struct {
    Expr** elements;
    int element_count;
} ArrayExpr;

typedef struct {
    Expr* object;
    Expr* index;
} IndexExpr;

typedef struct Expr {
    ExprType type;
    int line;
    union {
        Token* literal;
        Token* variable;
        BinaryExpr binary;
        UnaryExpr unary;
        CallExpr call;
        LogicalExpr logical;
        AssignmentExpr assignment;
        ArrayExpr array;
        IndexExpr index;
    } data;
} Expr;

typedef struct {
    Token* name;
    Expr* initializer;
} VarStmt;

typedef struct {
    Stmt** statements;
    int statement_count;
} BlockStmt;

typedef struct {
    Expr* condition;
    Stmt* then_branch;
    Stmt* else_branch;
} IfStmt;

typedef struct {
    Expr* condition;
    Stmt* body;
} WhileStmt;

typedef struct {
    Stmt* initializer;
    Expr* condition;
    Expr* increment;
    Stmt* body;
} ForStmt;

typedef struct {
    Token* name;
    Token** parameters;
    int param_count;
    Stmt** body;
    int body_count;
} FunctionStmt;

typedef struct {
    Expr* value;
} ReturnStmt;

typedef struct Stmt {
    StmtType type;
    int line;
    union {
        Expr* expression;
        VarStmt var;
        BlockStmt block;
        IfStmt if_stmt;
        WhileStmt while_stmt;
        ForStmt for_stmt;
        FunctionStmt function;
        ReturnStmt return_stmt;
    } data;
} Stmt;

Expr* expr_new(ExprType type, int line);
void expr_free(Expr* expr);

Stmt* stmt_new(StmtType type, int line);
void stmt_free(Stmt* stmt);

#endif
