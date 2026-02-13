#include "vm.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

VM::VM()
    : ip(0) {}

VM::VM(const std::vector<Instruction>& instr)
    : instructions(instr), ip(0) {}

void VM::loadProgram(const std::vector<Instruction>& instr) {
    instructions = instr;
    ip = 0;
    stack.clear();
}

void VM::push(const std::string& value) {
    stack.push_back(value);
}

std::string VM::pop() {
    if (stack.empty()) {
        throw std::runtime_error("VM stack underflow");
    }
    std::string value = stack.back();
    stack.pop_back();
    return value;
}

double VM::toNumber(const std::string& value) {
    return std::stod(value);
}

bool VM::toBool(const std::string& value) {
    if (value == "true") return true;
    if (value == "false") return false;
    return std::stod(value) != 0.0;
}

void VM::run() {
    while (ip < static_cast<int>(instructions.size())) {
        const Instruction& instr = instructions[ip];

        switch (instr.op) {
            case OpCode::LOAD_CONST:
                push(instr.operand);
                break;
            case OpCode::LOAD_VAR:
                push(variables[instr.operand]);
                break;
            case OpCode::STORE_VAR: {
                std::string value = pop();
                variables[instr.operand] = value;
                break;
            }
            case OpCode::ADD: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(std::to_string(a + b));
                break;
            }
            case OpCode::SUB: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(std::to_string(a - b));
                break;
            }
            case OpCode::MUL: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(std::to_string(a * b));
                break;
            }
            case OpCode::DIV: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(std::to_string(a / b));
                break;
            }
            case OpCode::MOD: {
                int b = std::stoi(pop());
                int a = std::stoi(pop());
                push(std::to_string(a % b));
                break;
            }
            case OpCode::NEG: {
                double a = toNumber(pop());
                push(std::to_string(-a));
                break;
            }
            case OpCode::NOT: {
                bool a = toBool(pop());
                push(a ? "false" : "true");
                break;
            }
            case OpCode::EQUAL: {
                std::string b = pop();
                std::string a = pop();
                push(a == b ? "true" : "false");
                break;
            }
            case OpCode::NOT_EQUAL: {
                std::string b = pop();
                std::string a = pop();
                push(a != b ? "true" : "false");
                break;
            }
            case OpCode::LESS: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(a < b ? "true" : "false");
                break;
            }
            case OpCode::LESS_EQUAL: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(a <= b ? "true" : "false");
                break;
            }
            case OpCode::GREATER: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(a > b ? "true" : "false");
                break;
            }
            case OpCode::GREATER_EQUAL: {
                double b = toNumber(pop());
                double a = toNumber(pop());
                push(a >= b ? "true" : "false");
                break;
            }
            case OpCode::AND: {
                bool b = toBool(pop());
                bool a = toBool(pop());
                push((a && b) ? "true" : "false");
                break;
            }
            case OpCode::OR: {
                bool b = toBool(pop());
                bool a = toBool(pop());
                push((a || b) ? "true" : "false");
                break;
            }
            case OpCode::PRINT:
                std::cout << pop() << std::endl;
                break;
            case OpCode::JUMP:
                ip = std::stoi(instr.operand);
                continue;
            case OpCode::JUMP_IF_FALSE: {
                bool condition = toBool(pop());
                if (!condition) {
                    ip = std::stoi(instr.operand);
                    continue;
                }
                break;
            }
            case OpCode::HALT:
                return;
        }

        ip++;
    }
}
