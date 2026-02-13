#include "compiler.h"
#include <memory>

std::vector<Instruction> Compiler::compile(const std::vector<StmtPtr>& statements) {
    instructions.clear();
    instructions.reserve(statements.size() * 2);

    for (const auto& stmt : statements) {
        compileStatement(stmt);
    }

    instructions.emplace_back(OpCode::HALT);
    return instructions;
}


// ================= STATEMENTS =================

void Compiler::compileStatement(const StmtPtr& stmt) {
    if (auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt)) {
        compileExpression(printStmt->expression);
        instructions.emplace_back(OpCode::PRINT);
    }
    else if (auto varDecl = std::dynamic_pointer_cast<VarDeclStmt>(stmt)) {
        if (varDecl->initializer) {
            compileExpression(varDecl->initializer);
            instructions.emplace_back(OpCode::STORE_VAR, varDecl->name);
        }
    }
    else if (auto exprStmt = std::dynamic_pointer_cast<ExprStmt>(stmt)) {
        compileExpression(exprStmt->expression);
    }
    else if (auto blockStmt = std::dynamic_pointer_cast<BlockStmt>(stmt)) {
        for (const auto& s : blockStmt->statements) {
            compileStatement(s);
        }
    }
    else if (auto ifStmt = std::dynamic_pointer_cast<IfStmt>(stmt)) {
        compileExpression(ifStmt->condition);

        int jumpIndex = static_cast<int>(instructions.size());
        instructions.emplace_back(OpCode::JUMP_IF_FALSE, "0");

        compileStatement(ifStmt->thenBranch);

        instructions[jumpIndex].operand =
            std::to_string(instructions.size());
    }
}


// ================= EXPRESSIONS =================

void Compiler::compileExpression(const ExprPtr& expr) {
    if (auto literal = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        instructions.emplace_back(OpCode::LOAD_CONST, literal->value);
    }
    else if (auto variable = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        instructions.emplace_back(OpCode::LOAD_VAR, variable->name);
    }
    else if (auto binary = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        if (binary->op == "=") {
            compileExpression(binary->right);
            auto target = std::dynamic_pointer_cast<VariableExpr>(binary->left);
            if (target) {
                instructions.emplace_back(OpCode::STORE_VAR, target->name);
            }
            return;
        }

        compileExpression(binary->left);
        compileExpression(binary->right);

        if (binary->op == "+") instructions.emplace_back(OpCode::ADD);
        else if (binary->op == "-") instructions.emplace_back(OpCode::SUB);
        else if (binary->op == "*") instructions.emplace_back(OpCode::MUL);
        else if (binary->op == "/") instructions.emplace_back(OpCode::DIV);
        else if (binary->op == "%") instructions.emplace_back(OpCode::MOD);
        else if (binary->op == "==") instructions.emplace_back(OpCode::EQUAL);
        else if (binary->op == "!=") instructions.emplace_back(OpCode::NOT_EQUAL);
        else if (binary->op == "<") instructions.emplace_back(OpCode::LESS);
        else if (binary->op == "<=") instructions.emplace_back(OpCode::LESS_EQUAL);
        else if (binary->op == ">") instructions.emplace_back(OpCode::GREATER);
        else if (binary->op == ">=") instructions.emplace_back(OpCode::GREATER_EQUAL);
        else if (binary->op == "&&") instructions.emplace_back(OpCode::AND);
        else if (binary->op == "||") instructions.emplace_back(OpCode::OR);
    }
    else if (auto unary = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        compileExpression(unary->right);

        if (unary->op == "-") instructions.emplace_back(OpCode::NEG);
        else if (unary->op == "!") instructions.emplace_back(OpCode::NOT);
    }
}
