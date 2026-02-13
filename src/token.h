#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    // Keywords
    MEOW,
    INT,
    DECI,
    BOOL,
    CHAR,
    STRING,
    TRUE,
    FALSE,
    IF,
    ELSE,

    // Identifiers
    IDENTIFIER,

    // Literals
    INTEGER_LITERAL,
    DECIMAL_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,

    // Operators
    PLUS, MINUS, STAR, SLASH, MOD,
    ASSIGN,
    EQUAL_EQUAL, NOT_EQUAL,
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    AND, OR, NOT,
    SHIFT_LEFT,

    // Symbols
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    SEMICOLON,

    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;

    Token(TokenType t, std::string v, int l)
        : type(t), value(v), line(l) {}
};

#endif
