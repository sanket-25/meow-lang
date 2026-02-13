#include "parser.h"
#include <stdexcept>
#include <string>

Parser::Parser(const std::vector<Token>& t)
    : tokens(t), current(0) {}

Token Parser::peek() { return tokens[current]; }
Token Parser::previous() { return tokens[current - 1]; }
bool Parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}


// ================= PARSE ENTRY =================

std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}


// ================= DECLARATIONS =================

StmtPtr Parser::declaration() {
    if (match(TokenType::INT) ||
        match(TokenType::DECI) ||
        match(TokenType::BOOL) ||
        match(TokenType::CHAR) ||
        match(TokenType::STRING)) {

        return varDeclaration();
    }

    return statement();
}

StmtPtr Parser::varDeclaration() {
    std::string type = previous().value;

    Token name = advance(); // identifier

    ExprPtr initializer = nullptr;

    if (match(TokenType::ASSIGN)) {
        initializer = expression();
    }

    match(TokenType::SEMICOLON);

    return std::make_shared<VarDeclStmt>(type, name.value, initializer);
}


// ================= STATEMENTS =================

StmtPtr Parser::statement() {
    if (match(TokenType::MEOW))
        return printStatement();

    if (match(TokenType::IF))
        return ifStatement();

    if (match(TokenType::LBRACE))
        return block();

    return std::make_shared<ExprStmt>(expression());
}

StmtPtr Parser::printStatement() {
    match(TokenType::SHIFT_LEFT);
    ExprPtr value = expression();
    match(TokenType::SEMICOLON);
    return std::make_shared<PrintStmt>(value);
}

StmtPtr Parser::ifStatement() {
    match(TokenType::LPAREN);
    ExprPtr condition = expression();
    match(TokenType::RPAREN);

    StmtPtr thenBranch = statement();
    return std::make_shared<IfStmt>(condition, thenBranch);
}

StmtPtr Parser::block() {
    auto blockStmt = std::make_shared<BlockStmt>();

    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        blockStmt->statements.push_back(declaration());
    }

    match(TokenType::RBRACE);
    return blockStmt;
}


// ================= EXPRESSIONS =================

ExprPtr Parser::expression() {
    return assignment();
}

ExprPtr Parser::assignment() {
    ExprPtr expr = logicalOr();

    if (match(TokenType::ASSIGN)) {
        ExprPtr value = assignment();
        return std::make_shared<BinaryExpr>(expr, "=", value);
    }

    return expr;
}

ExprPtr Parser::logicalOr() {
    ExprPtr expr = logicalAnd();
    while (match(TokenType::OR)) {
        std::string op = previous().value;
        ExprPtr right = logicalAnd();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::logicalAnd() {
    ExprPtr expr = equality();
    while (match(TokenType::AND)) {
        std::string op = previous().value;
        ExprPtr right = equality();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::equality() {
    ExprPtr expr = comparison();
    while (match(TokenType::EQUAL_EQUAL) || match(TokenType::NOT_EQUAL)) {
        std::string op = previous().value;
        ExprPtr right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::comparison() {
    ExprPtr expr = term();
    while (match(TokenType::LESS) || match(TokenType::GREATER) ||
           match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL)) {
        std::string op = previous().value;
        ExprPtr right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::term() {
    ExprPtr expr = factor();
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        std::string op = previous().value;
        ExprPtr right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::factor() {
    ExprPtr expr = unary();
    while (match(TokenType::STAR) || match(TokenType::SLASH) || match(TokenType::MOD)) {
        std::string op = previous().value;
        ExprPtr right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::unary() {
    if (match(TokenType::NOT) || match(TokenType::MINUS)) {
        std::string op = previous().value;
        ExprPtr right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return primary();
}

ExprPtr Parser::primary() {
    if (match(TokenType::INTEGER_LITERAL) ||
        match(TokenType::DECIMAL_LITERAL) ||
        match(TokenType::STRING_LITERAL) ||
        match(TokenType::CHAR_LITERAL) ||
        match(TokenType::TRUE) ||
        match(TokenType::FALSE)) {

        return std::make_shared<LiteralExpr>(previous().value);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(previous().value);
    }

    if (match(TokenType::LPAREN)) {
        ExprPtr expr = expression();
        match(TokenType::RPAREN);
        return expr;
    }

    throw std::runtime_error("Unexpected token at line " + std::to_string(peek().line));
}
