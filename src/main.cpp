#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"

namespace {
const char* kVersion = "meowlang 1.0.0";
#ifdef _WIN32
const char* kBinaryName = "meow.exe";
#else
const char* kBinaryName = "meow";
#endif

void printUsage() {
    std::cout << "Usage:\n"
              << "  " << kBinaryName << " <file.meow>\n"
              << "  " << kBinaryName << " run <file.meow>\n"
              << "  " << kBinaryName << " repl\n"
              << "  " << kBinaryName << " --version\n"
              << "  " << kBinaryName << " --help\n";
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<Instruction> compileSource(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    Compiler compiler;
    return compiler.compile(ast);
}

int runFile(VM& vm, const std::string& path) {
    try {
        auto bytecode = compileSource(readFile(path));
        vm.loadProgram(bytecode);
        vm.run();
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}

int runRepl(VM& vm) {
    std::cout << "MeowLang REPL. Type 'exit' to quit.\n";
    std::string line;
    while (true) {
        std::cout << "meow> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }
        if (line == "exit" || line == "quit") {
            break;
        }
        if (line.empty()) {
            continue;
        }
        try {
            auto bytecode = compileSource(line);
            vm.loadProgram(bytecode);
            vm.run();
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }
    return 0;
}
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    VM vm;

    if (command == "--help" || command == "-h") {
        printUsage();
        return 0;
    }

    if (command == "--version") {
        std::cout << kVersion << "\n";
        return 0;
    }

    if (command == "repl") {
        return runRepl(vm);
    }

    if (command == "run") {
        if (argc < 3) {
            std::cerr << "Error: missing file path.\n";
            printUsage();
            return 1;
        }
        return runFile(vm, argv[2]);
    }

    return runFile(vm, command);
}
