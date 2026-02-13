#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string source;
    size_t pos;
    int line;

    char peek() const;
    char advance();
    void skipWhitespace();
    Token identifier();
    Token number();
    Token stringLiteral();
    Token charLiteral();

public:
    explicit Lexer(const std::string& src);
    std::vector<Token> tokenize();
};

#endif
