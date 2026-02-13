#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include <vector>

enum class OpCode {
    LOAD_CONST,
    LOAD_VAR,
    STORE_VAR,

    ADD,
    SUB,
    MUL,
    DIV,
    MOD,

    NEG,
    NOT,

    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    AND,
    OR,

    PRINT,

    JUMP,
    JUMP_IF_FALSE,

    HALT
};

struct Instruction {
    OpCode op;
    std::string operand;

    Instruction(OpCode o, const std::string& opd = "")
        : op(o), operand(opd) {}
};

#endif
