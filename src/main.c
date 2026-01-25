#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "parser.h"
#include "codegen.h"

#ifdef _WIN32
#define SYSTEM_CMD(cmd) system(cmd)
#else
#define SYSTEM_CMD(cmd) system(cmd)
#endif

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    return content;
}

char* change_extension(const char* filename, const char* new_ext) {
    char* result = (char*)malloc(strlen(filename) + 10);
    strcpy(result, filename);
    
    char* dot = strrchr(result, '.');
    if (dot) {
        strcpy(dot, new_ext);
    } else {
        strcat(result, new_ext);
    }
    
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: meow <file.meow>\n");
        fprintf(stderr, "Example: meow test.meow\n");
        return 1;
    }
    
    const char* meow_file = argv[1];
    
    // Read the .meow file
    char* source = read_file(meow_file);
    if (!source) {
        return 1;
    }
    
    // Parse the source code
    Parser* parser = parser_new(source);
    int stmt_count = 0;
    Stmt** statements = parser_parse(parser, &stmt_count);
    
    // Generate C code
    char* c_file = change_extension(meow_file, ".c");
    FILE* c_output = fopen(c_file, "w");
    if (!c_output) {
        fprintf(stderr, "Error: Could not create C output file\n");
        return 1;
    }
    
    CodeGen* gen = codegen_new(c_output);
    codegen_generate(gen, statements, stmt_count);
    
    fclose(c_output);
    codegen_free(gen);
    
    // Compile the C code
    char* exec_file = change_extension(meow_file, "");
    #ifdef _WIN32
    char* exe_file = change_extension(meow_file, ".exe");
    char compile_cmd[512];
    snprintf(compile_cmd, sizeof(compile_cmd), "gcc -o \"%s\" \"%s\" -lm", exe_file, c_file);
    #else
    char compile_cmd[512];
    snprintf(compile_cmd, sizeof(compile_cmd), "gcc -o \"%s\" \"%s\" -lm", exec_file, c_file);
    #endif
    
    int compile_result = SYSTEM_CMD(compile_cmd);
    if (compile_result != 0) {
        fprintf(stderr, "Error: Compilation failed\n");
        return 1;
    }
    
    // Execute the compiled binary
    #ifdef _WIN32
    char run_cmd[512];
    snprintf(run_cmd, sizeof(run_cmd), "\"%s\"", exe_file);
    SYSTEM_CMD(run_cmd);
    #else
    char run_cmd[512];
    snprintf(run_cmd, sizeof(run_cmd), "./%s", exec_file);
    SYSTEM_CMD(run_cmd);
    #endif
    
    // Clean up
    remove(c_file);
    free(c_file);
    free(exec_file);
    free(source);
    parser_free(parser);
    
    for (int i = 0; i < stmt_count; i++) {
        stmt_free(statements[i]);
    }
    free(statements);
    
    return 0;
}
