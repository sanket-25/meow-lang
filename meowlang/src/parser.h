#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    int current;

    Token peek();
    Token previous();
    bool match(TokenType type);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();

    // parsing
    StmtPtr declaration();
    StmtPtr statement();
    StmtPtr printStatement();
    StmtPtr ifStatement();
    StmtPtr block();
    StmtPtr varDeclaration();

    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logicalOr();
    ExprPtr logicalAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr primary();

public:
    explicit Parser(const std::vector<Token>& t);
    std::vector<StmtPtr> parse();
};

#endif
