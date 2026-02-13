#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

using ASTNodePtr = std::shared_ptr<ASTNode>;


// ================= EXPRESSIONS =================

class Expression : public ASTNode {
};

using ExprPtr = std::shared_ptr<Expression>;

class LiteralExpr : public Expression {
public:
    std::string value;
    explicit LiteralExpr(const std::string& val) : value(val) {}
};

class VariableExpr : public Expression {
public:
    std::string name;
    explicit VariableExpr(const std::string& n) : name(n) {}
};

class BinaryExpr : public Expression {
public:
    ExprPtr left;
    std::string op;
    ExprPtr right;

    BinaryExpr(ExprPtr l, const std::string& o, ExprPtr r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
};

class UnaryExpr : public Expression {
public:
    std::string op;
    ExprPtr right;

    UnaryExpr(const std::string& o, ExprPtr r)
        : op(o), right(std::move(r)) {}
};


// ================= STATEMENTS =================

class Statement : public ASTNode {
};

using StmtPtr = std::shared_ptr<Statement>;

class PrintStmt : public Statement {
public:
    ExprPtr expression;
    explicit PrintStmt(ExprPtr expr) : expression(std::move(expr)) {}
};

class VarDeclStmt : public Statement {
public:
    std::string type;
    std::string name;
    ExprPtr initializer;

    VarDeclStmt(const std::string& t, const std::string& n, ExprPtr init)
        : type(t), name(n), initializer(std::move(init)) {}
};

class ExprStmt : public Statement {
public:
    ExprPtr expression;
    explicit ExprStmt(ExprPtr expr) : expression(std::move(expr)) {}
};

class BlockStmt : public Statement {
public:
    std::vector<StmtPtr> statements;
};

class IfStmt : public Statement {
public:
    ExprPtr condition;
    StmtPtr thenBranch;

    IfStmt(ExprPtr cond, StmtPtr thenB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)) {}
};

#endif
