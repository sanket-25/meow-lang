#ifndef COMPILER_H
#define COMPILER_H

#include "ast.h"
#include "bytecode.h"
#include <vector>

class Compiler {
private:
    std::vector<Instruction> instructions;

    void compileStatement(const StmtPtr& stmt);
    void compileExpression(const ExprPtr& expr);

public:
    std::vector<Instruction> compile(const std::vector<StmtPtr>& statements);
};

#endif
