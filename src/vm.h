#ifndef VM_H
#define VM_H

#include "bytecode.h"
#include <string>
#include <unordered_map>
#include <vector>

class VM {
private:
    std::vector<Instruction> instructions;
    std::vector<std::string> stack;
    std::unordered_map<std::string, std::string> variables;
    int ip;

    std::string pop();
    void push(const std::string& value);

    double toNumber(const std::string& value);
    bool toBool(const std::string& value);

public:
    VM();
    explicit VM(const std::vector<Instruction>& instr);
    void loadProgram(const std::vector<Instruction>& instr);
    void run();
};

#endif
