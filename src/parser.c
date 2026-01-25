#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Parser* parser_new(const char* source) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    
    // Tokenize the source
    Lexer* lexer = lexer_new(source);
    parser->token_count = 0;
    parser->tokens = (Token**)malloc(sizeof(Token*) * 1000); // Initial capacity
    
    Token* token;
    while ((token = lexer_next_token(lexer))->type != TOKEN_EOF) {
        if (token->type != TOKEN_NEWLINE) {  // Skip newlines for now
            parser->tokens[parser->token_count++] = token;
        } else {
            token_free(token);
        }
    }
    parser->tokens[parser->token_count++] = token;  // Add EOF token
    
    lexer_free(lexer);
    parser->current = 0;
    
    return parser;
}

void parser_free(Parser* parser) {
    if (parser) {
        for (int i = 0; i < parser->token_count; i++) {
            token_free(parser->tokens[i]);
        }
        free(parser->tokens);
        free(parser);
    }
}

static Token* peek(Parser* parser) {
    if (parser->current >= parser->token_count) {
        return parser->tokens[parser->token_count - 1];  // Return EOF
    }
    return parser->tokens[parser->current];
}

static Token* peek_ahead(Parser* parser, int n) {
    if (parser->current + n >= parser->token_count) {
        return parser->tokens[parser->token_count - 1];  // Return EOF
    }
    return parser->tokens[parser->current + n];
}

static Token* advance(Parser* parser) {
    Token* token = peek(parser);
    if (token->type != TOKEN_EOF) {
        parser->current++;
    }
    return token;
}

static Token* consume(Parser* parser, TokenType type, const char* error) {
    if (peek(parser)->type != type) {
        fprintf(stderr, "Parse error: %s\n", error);
        return NULL;
    }
    return advance(parser);
}

static int check(Parser* parser, TokenType type) {
    return peek(parser)->type == type;
}

static int match(Parser* parser, TokenType* types, int count) {
    for (int i = 0; i < count; i++) {
        if (check(parser, types[i])) {
            advance(parser);
            return 1;
        }
    }
    return 0;
}

static Expr* parse_expression(Parser* parser);
static Expr* parse_assignment(Parser* parser);
static Expr* parse_logical_or(Parser* parser);
static Expr* parse_logical_and(Parser* parser);
static Expr* parse_equality(Parser* parser);
static Expr* parse_comparison(Parser* parser);
static Expr* parse_bitwise(Parser* parser);
static Expr* parse_shift(Parser* parser);
static Expr* parse_addition(Parser* parser);
static Expr* parse_multiplication(Parser* parser);
static Expr* parse_unary(Parser* parser);
static Expr* parse_call(Parser* parser);
static Expr* parse_primary(Parser* parser);

static Expr* parse_primary(Parser* parser) {
    Expr* expr;
    
    if (check(parser, TOKEN_TRUE) || check(parser, TOKEN_FALSE) || 
        check(parser, TOKEN_NULL) || check(parser, TOKEN_NUMBER) || 
        check(parser, TOKEN_STRING)) {
        Token* token = advance(parser);
        expr = expr_new(EXPR_LITERAL, token->line);
        expr->data.literal = token_new(token->type, token->lexeme, token->line, token->column);
        if (token->type == TOKEN_NUMBER) {
            expr->data.literal->value.number = token->value.number;
        }
        return expr;
    }
    
    if (check(parser, TOKEN_IDENTIFIER)) {
        Token* token = advance(parser);
        expr = expr_new(EXPR_VARIABLE, token->line);
        expr->data.variable = token_new(token->type, token->lexeme, token->line, token->column);
        return expr;
    }
    
    if (match(parser, (TokenType[]){TOKEN_LEFT_PAREN}, 1)) {
        expr = parse_expression(parser);
        consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }
    
    if (check(parser, TOKEN_LEFT_BRACKET)) {
        advance(parser);
        expr = expr_new(EXPR_ARRAY, peek(parser)->line);
        expr->data.array.elements = (Expr**)malloc(sizeof(Expr*) * 100);
        expr->data.array.element_count = 0;
        
        while (!check(parser, TOKEN_RIGHT_BRACKET) && !check(parser, TOKEN_EOF)) {
            expr->data.array.elements[expr->data.array.element_count++] = parse_expression(parser);
            if (!check(parser, TOKEN_RIGHT_BRACKET)) {
                consume(parser, TOKEN_COMMA, "Expected ',' between array elements");
            }
        }
        consume(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after array");
        return expr;
    }
    
    fprintf(stderr, "Parse error: unexpected token\n");
    return NULL;
}

static Expr* parse_call(Parser* parser) {
    Expr* expr = parse_primary(parser);
    
    while (1) {
        if (match(parser, (TokenType[]){TOKEN_LEFT_PAREN}, 1)) {
            Expr* call = expr_new(EXPR_CALL, peek(parser)->line);
            call->data.call.callee = expr;
            call->data.call.arguments = (Expr**)malloc(sizeof(Expr*) * 100);
            call->data.call.arg_count = 0;
            
            while (!check(parser, TOKEN_RIGHT_PAREN) && !check(parser, TOKEN_EOF)) {
                call->data.call.arguments[call->data.call.arg_count++] = parse_expression(parser);
                if (!check(parser, TOKEN_RIGHT_PAREN)) {
                    consume(parser, TOKEN_COMMA, "Expected ',' between arguments");
                }
            }
            consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after arguments");
            expr = call;
        } else if (match(parser, (TokenType[]){TOKEN_LEFT_BRACKET}, 1)) {
            Expr* index = expr_new(EXPR_INDEX, peek(parser)->line);
            index->data.index.object = expr;
            index->data.index.index = parse_expression(parser);
            consume(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after index");
            expr = index;
        } else if (match(parser, (TokenType[]){TOKEN_DOT}, 1)) {
            Expr* member = expr_new(EXPR_MEMBER, peek(parser)->line);
            member->data.member.object = expr;
            Token* name = advance(parser);
            member->data.member.index = expr_new(EXPR_VARIABLE, name->line);
            member->data.member.index->data.variable = token_new(name->type, name->lexeme, name->line, name->column);
            expr = member;
        } else {
            break;
        }
    }
    
    return expr;
}

static Expr* parse_unary(Parser* parser) {
    if (match(parser, (TokenType[]){TOKEN_BANG, TOKEN_MINUS, TOKEN_PLUS, TOKEN_TILDE, TOKEN_INCREMENT, TOKEN_DECREMENT}, 6)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* expr = expr_new(EXPR_UNARY, op->line);
        expr->data.unary.operator = op;
        expr->data.unary.right = parse_unary(parser);
        return expr;
    }
    
    return parse_call(parser);
}

static Expr* parse_multiplication(Parser* parser) {
    Expr* expr = parse_unary(parser);
    
    while (match(parser, (TokenType[]){TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT}, 3)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_unary(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_addition(Parser* parser) {
    Expr* expr = parse_multiplication(parser);
    
    while (match(parser, (TokenType[]){TOKEN_PLUS, TOKEN_MINUS}, 2)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_multiplication(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_shift(Parser* parser) {
    Expr* expr = parse_addition(parser);
    
    while (match(parser, (TokenType[]){TOKEN_LSHIFT, TOKEN_RSHIFT}, 2)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_addition(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_bitwise(Parser* parser) {
    Expr* expr = parse_shift(parser);
    
    while (match(parser, (TokenType[]){TOKEN_AMPERSAND, TOKEN_PIPE, TOKEN_CARET}, 3)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_shift(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_comparison(Parser* parser) {
    Expr* expr = parse_bitwise(parser);
    
    while (match(parser, (TokenType[]){TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL}, 4)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_bitwise(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_equality(Parser* parser) {
    Expr* expr = parse_comparison(parser);
    
    while (match(parser, (TokenType[]){TOKEN_EQUAL_EQUAL, TOKEN_BANG_EQUAL}, 2)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* binary = expr_new(EXPR_BINARY, op->line);
        binary->data.binary.operator = op;
        binary->data.binary.left = expr;
        binary->data.binary.right = parse_comparison(parser);
        expr = binary;
    }
    
    return expr;
}

static Expr* parse_logical_and(Parser* parser) {
    Expr* expr = parse_equality(parser);
    
    while (match(parser, (TokenType[]){TOKEN_AND}, 1)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* logical = expr_new(EXPR_LOGICAL, op->line);
        logical->data.logical.operator = op;
        logical->data.logical.left = expr;
        logical->data.logical.right = parse_equality(parser);
        expr = logical;
    }
    
    return expr;
}

static Expr* parse_logical_or(Parser* parser) {
    Expr* expr = parse_logical_and(parser);
    
    while (match(parser, (TokenType[]){TOKEN_OR}, 1)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* logical = expr_new(EXPR_LOGICAL, op->line);
        logical->data.logical.operator = op;
        logical->data.logical.left = expr;
        logical->data.logical.right = parse_logical_and(parser);
        expr = logical;
    }
    
    return expr;
}

static Expr* parse_assignment(Parser* parser) {
    Expr* expr = parse_logical_or(parser);
    
    if (match(parser, (TokenType[]){TOKEN_EQUAL, TOKEN_PLUS_EQUAL, TOKEN_MINUS_EQUAL, TOKEN_STAR_EQUAL, TOKEN_SLASH_EQUAL}, 5)) {
        Token* op = parser->tokens[parser->current - 1];
        Expr* assignment = expr_new(EXPR_ASSIGNMENT, op->line);
        assignment->data.assignment.object = expr;
        assignment->data.assignment.value = parse_assignment(parser);
        return assignment;
    }
    
    return expr;
}

static Expr* parse_expression(Parser* parser) {
    return parse_assignment(parser);
}

static Stmt* parse_statement(Parser* parser);
static Stmt* parse_declaration(Parser* parser);

static Stmt* parse_variable_declaration(Parser* parser) {
    Token* name = consume(parser, TOKEN_IDENTIFIER, "Expected variable name");
    
    Stmt* stmt = stmt_new(STMT_VARIABLE, name->line);
    stmt->data.var.name = name;
    stmt->data.var.initializer = NULL;
    
    if (match(parser, (TokenType[]){TOKEN_EQUAL}, 1)) {
        stmt->data.var.initializer = parse_expression(parser);
    }
    
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after variable declaration");
    
    return stmt;
}

static Stmt* parse_function_declaration(Parser* parser) {
    Token* name = consume(parser, TOKEN_IDENTIFIER, "Expected function name");
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after function name");
    
    Stmt* stmt = stmt_new(STMT_FUNCTION, name->line);
    stmt->data.function.name = name;
    stmt->data.function.parameters = (Token**)malloc(sizeof(Token*) * 100);
    stmt->data.function.param_count = 0;
    
    while (!check(parser, TOKEN_RIGHT_PAREN) && !check(parser, TOKEN_EOF)) {
        stmt->data.function.parameters[stmt->data.function.param_count++] = 
            consume(parser, TOKEN_IDENTIFIER, "Expected parameter name");
        
        if (!check(parser, TOKEN_RIGHT_PAREN)) {
            consume(parser, TOKEN_COMMA, "Expected ',' between parameters");
        }
    }
    
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after parameters");
    consume(parser, TOKEN_LEFT_BRACE, "Expected '{' before function body");
    
    stmt->data.function.body = (Stmt**)malloc(sizeof(Stmt*) * 1000);
    stmt->data.function.body_count = 0;
    
    while (!check(parser, TOKEN_RIGHT_BRACE) && !check(parser, TOKEN_EOF)) {
        stmt->data.function.body[stmt->data.function.body_count++] = parse_declaration(parser);
    }
    
    consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after function body");
    
    return stmt;
}

static Stmt* parse_block(Parser* parser) {
    Stmt* stmt = stmt_new(STMT_BLOCK, peek(parser)->line);
    stmt->data.block.statements = (Stmt**)malloc(sizeof(Stmt*) * 1000);
    stmt->data.block.statement_count = 0;
    
    while (!check(parser, TOKEN_RIGHT_BRACE) && !check(parser, TOKEN_EOF)) {
        stmt->data.block.statements[stmt->data.block.statement_count++] = parse_declaration(parser);
    }
    
    return stmt;
}

static Stmt* parse_if_statement(Parser* parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'if'");
    Expr* condition = parse_expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after if condition");
    
    consume(parser, TOKEN_LEFT_BRACE, "Expected '{' after if condition");
    Stmt* then_branch = parse_block(parser);
    consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after if body");
    
    Stmt* stmt = stmt_new(STMT_IF, peek(parser)->line);
    stmt->data.if_stmt.condition = condition;
    stmt->data.if_stmt.then_branch = then_branch;
    stmt->data.if_stmt.else_branch = NULL;
    
    if (match(parser, (TokenType[]){TOKEN_ELSE}, 1)) {
        consume(parser, TOKEN_LEFT_BRACE, "Expected '{' after 'else'");
        stmt->data.if_stmt.else_branch = parse_block(parser);
        consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after else body");
    }
    
    return stmt;
}

static Stmt* parse_while_statement(Parser* parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'while'");
    Expr* condition = parse_expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after while condition");
    
    consume(parser, TOKEN_LEFT_BRACE, "Expected '{' after while condition");
    Stmt* body = parse_block(parser);
    consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after while body");
    
    Stmt* stmt = stmt_new(STMT_WHILE, peek(parser)->line);
    stmt->data.while_stmt.condition = condition;
    stmt->data.while_stmt.body = body;
    
    return stmt;
}

static Stmt* parse_for_statement(Parser* parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'for'");
    
    Stmt* initializer = NULL;
    if (match(parser, (TokenType[]){TOKEN_VAR}, 1)) {
        initializer = parse_variable_declaration(parser);
    } else if (!check(parser, TOKEN_SEMICOLON)) {
        Stmt* expr_stmt = stmt_new(STMT_EXPRESSION, peek(parser)->line);
        expr_stmt->data.expression = parse_expression(parser);
        consume(parser, TOKEN_SEMICOLON, "Expected ';'");
        initializer = expr_stmt;
    } else {
        consume(parser, TOKEN_SEMICOLON, "Expected ';'");
    }
    
    Expr* condition = NULL;
    if (!check(parser, TOKEN_SEMICOLON)) {
        condition = parse_expression(parser);
    }
    consume(parser, TOKEN_SEMICOLON, "Expected ';'");
    
    Expr* increment = NULL;
    if (!check(parser, TOKEN_RIGHT_PAREN)) {
        increment = parse_expression(parser);
    }
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after for clauses");
    
    consume(parser, TOKEN_LEFT_BRACE, "Expected '{' after for header");
    Stmt* body = parse_block(parser);
    consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after for body");
    
    Stmt* stmt = stmt_new(STMT_FOR, peek(parser)->line);
    stmt->data.for_stmt.initializer = initializer;
    stmt->data.for_stmt.condition = condition;
    stmt->data.for_stmt.increment = increment;
    stmt->data.for_stmt.body = body;
    
    return stmt;
}

static Stmt* parse_return_statement(Parser* parser) {
    Stmt* stmt = stmt_new(STMT_RETURN, peek(parser)->line);
    stmt->data.return_stmt.value = NULL;
    
    if (!check(parser, TOKEN_SEMICOLON)) {
        stmt->data.return_stmt.value = parse_expression(parser);
    }
    
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after return value");
    
    return stmt;
}

static Stmt* parse_statement(Parser* parser) {
    if (match(parser, (TokenType[]){TOKEN_RETURN}, 1)) {
        return parse_return_statement(parser);
    }
    
    if (match(parser, (TokenType[]){TOKEN_IF}, 1)) {
        return parse_if_statement(parser);
    }
    
    if (match(parser, (TokenType[]){TOKEN_WHILE}, 1)) {
        return parse_while_statement(parser);
    }
    
    if (match(parser, (TokenType[]){TOKEN_FOR}, 1)) {
        return parse_for_statement(parser);
    }
    
    if (match(parser, (TokenType[]){TOKEN_BREAK}, 1)) {
        Stmt* stmt = stmt_new(STMT_BREAK, peek(parser)->line);
        consume(parser, TOKEN_SEMICOLON, "Expected ';' after break");
        return stmt;
    }
    
    if (match(parser, (TokenType[]){TOKEN_CONTINUE}, 1)) {
        Stmt* stmt = stmt_new(STMT_CONTINUE, peek(parser)->line);
        consume(parser, TOKEN_SEMICOLON, "Expected ';' after continue");
        return stmt;
    }
    
    Stmt* stmt = stmt_new(STMT_EXPRESSION, peek(parser)->line);
    stmt->data.expression = parse_expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression");
    return stmt;
}

static Stmt* parse_declaration(Parser* parser) {
    if (match(parser, (TokenType[]){TOKEN_VAR}, 1)) {
        return parse_variable_declaration(parser);
    }
    
    if (match(parser, (TokenType[]){TOKEN_FUNC}, 1)) {
        return parse_function_declaration(parser);
    }
    
    return parse_statement(parser);
}

Stmt** parser_parse(Parser* parser, int* stmt_count) {
    Stmt** statements = (Stmt**)malloc(sizeof(Stmt*) * 1000);
    *stmt_count = 0;
    
    while (!check(parser, TOKEN_EOF)) {
        statements[(*stmt_count)++] = parse_declaration(parser);
    }
    
    return statements;
}
