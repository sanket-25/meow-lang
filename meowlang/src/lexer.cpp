#include "lexer.h"
#include <cctype>
#include <unordered_map>

Lexer::Lexer(const std::string& src)
    : source(src), pos(0), line(1) {}

char Lexer::peek() const {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char Lexer::advance() {
    if (pos >= source.length()) return '\0';
    return source[pos++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) {
        if (peek() == '\n') line++;
        advance();
    }
}

Token Lexer::identifier() {
    std::string value;
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        value += advance();
    }

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"meow", TokenType::MEOW},
        {"int", TokenType::INT},
        {"deci", TokenType::DECI},
        {"bool", TokenType::BOOL},
        {"char", TokenType::CHAR},
        {"string", TokenType::STRING},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE}
    };

    auto it = keywords.find(value);
    if (it != keywords.end()) {
        return Token(it->second, value, line);
    }

    return Token(TokenType::IDENTIFIER, value, line);
}

Token Lexer::number() {
    std::string value;
    bool isDecimal = false;

    while (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
        if (peek() == '.') {
            if (isDecimal) break;
            isDecimal = true;
        }
        value += advance();
    }

    if (isDecimal) {
        return Token(TokenType::DECIMAL_LITERAL, value, line);
    }

    return Token(TokenType::INTEGER_LITERAL, value, line);
}

Token Lexer::stringLiteral() {
    std::string value;
    advance(); // skip opening quote

    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') line++;
        value += advance();
    }

    if (peek() == '"') {
        advance(); // closing quote
    }

    return Token(TokenType::STRING_LITERAL, value, line);
}

Token Lexer::charLiteral() {
    std::string value;
    advance(); // skip opening quote

    if (peek() != '\0' && peek() != '\n') {
        value += advance();
    }

    if (peek() == '\'') {
        advance(); // closing quote
    }

    return Token(TokenType::CHAR_LITERAL, value, line);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < source.length()) {
        skipWhitespace();
        char c = peek();

        if (c == '\0') break;

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(identifier());
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(number());
        } else {
            switch (c) {
                case '+':
                    tokens.emplace_back(TokenType::PLUS, "+", line);
                    advance();
                    break;
                case '-':
                    tokens.emplace_back(TokenType::MINUS, "-", line);
                    advance();
                    break;
                case '*':
                    tokens.emplace_back(TokenType::STAR, "*", line);
                    advance();
                    break;
                case '/':
                    tokens.emplace_back(TokenType::SLASH, "/", line);
                    advance();
                    break;
                case '%':
                    tokens.emplace_back(TokenType::MOD, "%", line);
                    advance();
                    break;

                case '=':
                    advance();
                    if (peek() == '=') {
                        advance();
                        tokens.emplace_back(TokenType::EQUAL_EQUAL, "==", line);
                    } else {
                        tokens.emplace_back(TokenType::ASSIGN, "=", line);
                    }
                    break;

                case '!':
                    advance();
                    if (peek() == '=') {
                        advance();
                        tokens.emplace_back(TokenType::NOT_EQUAL, "!=", line);
                    } else {
                        tokens.emplace_back(TokenType::NOT, "!", line);
                    }
                    break;

                case '<':
                    advance();
                    if (peek() == '<') {
                        advance();
                        tokens.emplace_back(TokenType::SHIFT_LEFT, "<<", line);
                    } else if (peek() == '=') {
                        advance();
                        tokens.emplace_back(TokenType::LESS_EQUAL, "<=", line);
                    } else {
                        tokens.emplace_back(TokenType::LESS, "<", line);
                    }
                    break;

                case '>':
                    advance();
                    if (peek() == '=') {
                        advance();
                        tokens.emplace_back(TokenType::GREATER_EQUAL, ">=", line);
                    } else {
                        tokens.emplace_back(TokenType::GREATER, ">", line);
                    }
                    break;

                case '&':
                    advance();
                    if (peek() == '&') {
                        advance();
                        tokens.emplace_back(TokenType::AND, "&&", line);
                    } else {
                        tokens.emplace_back(TokenType::UNKNOWN, "&", line);
                    }
                    break;

                case '|':
                    advance();
                    if (peek() == '|') {
                        advance();
                        tokens.emplace_back(TokenType::OR, "||", line);
                    } else {
                        tokens.emplace_back(TokenType::UNKNOWN, "|", line);
                    }
                    break;

                case '(':
                    tokens.emplace_back(TokenType::LPAREN, "(", line);
                    advance();
                    break;

                case ')':
                    tokens.emplace_back(TokenType::RPAREN, ")", line);
                    advance();
                    break;

                case '{':
                    tokens.emplace_back(TokenType::LBRACE, "{", line);
                    advance();
                    break;

                case '}':
                    tokens.emplace_back(TokenType::RBRACE, "}", line);
                    advance();
                    break;

                case ';':
                    tokens.emplace_back(TokenType::SEMICOLON, ";", line);
                    advance();
                    break;

                case '"':
                    tokens.push_back(stringLiteral());
                    break;

                case '\'':
                    tokens.push_back(charLiteral());
                    break;

                default:
                    tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c), line);
                    advance();
                    break;
            }
        }
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "EOF", line);
    return tokens;
}
