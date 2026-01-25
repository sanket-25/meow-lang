#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* lexer_new(const char* source) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->current = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->start = 0;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    if (lexer) free(lexer);
}

static int peek(Lexer* lexer) {
    if (lexer->current >= strlen(lexer->source)) return '\0';
    return lexer->source[lexer->current];
}

static int peek_next(Lexer* lexer) {
    if (lexer->current + 1 >= strlen(lexer->source)) return '\0';
    return lexer->source[lexer->current + 1];
}

static int advance(Lexer* lexer) {
    int ch = lexer->source[lexer->current];
    lexer->current++;
    if (ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return ch;
}

static Token* make_token(Lexer* lexer, TokenType type) {
    int len = lexer->current - lexer->start;
    char* lexeme = (char*)malloc(len + 1);
    strncpy(lexeme, &lexer->source[lexer->start], len);
    lexeme[len] = '\0';
    
    Token* token = token_new(type, lexeme, lexer->line, lexer->column - len);
    free(lexeme);
    return token;
}

static void skip_whitespace(Lexer* lexer) {
    while (1) {
        int ch = peek(lexer);
        if (ch == ' ' || ch == '\r' || ch == '\t') {
            advance(lexer);
        } else if (ch == '/' && peek_next(lexer) == '/') {
            // Single line comment
            while (peek(lexer) != '\n' && peek(lexer) != '\0') {
                advance(lexer);
            }
        } else if (ch == '/' && peek_next(lexer) == '*') {
            // Multi-line comment
            advance(lexer); // skip /
            advance(lexer); // skip *
            while (!(peek(lexer) == '*' && peek_next(lexer) == '/') && peek(lexer) != '\0') {
                advance(lexer);
            }
            if (peek(lexer) == '*') {
                advance(lexer); // skip *
                advance(lexer); // skip /
            }
        } else {
            break;
        }
    }
}

static Token* scan_number(Lexer* lexer) {
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    
    if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
        advance(lexer); // consume .
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
    }
    
    Token* token = make_token(lexer, TOKEN_NUMBER);
    token->value.number = atof(token->lexeme);
    return token;
}

static Token* scan_string(Lexer* lexer, char quote) {
    advance(lexer); // consume opening quote
    
    int start = lexer->current;
    while (peek(lexer) != quote && peek(lexer) != '\0') {
        if (peek(lexer) == '\\') {
            advance(lexer);
        }
        advance(lexer);
    }
    
    if (peek(lexer) == '\0') {
        return token_new(TOKEN_ERROR, "Unterminated string", lexer->line, lexer->column);
    }
    
    advance(lexer); // consume closing quote
    
    Token* token = make_token(lexer, TOKEN_STRING);
    return token;
}

static Token* scan_identifier(Lexer* lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    
    Token* token = make_token(lexer, TOKEN_IDENTIFIER);
    
    // Check for keywords
    if (strcmp(token->lexeme, "var") == 0) token->type = TOKEN_VAR;
    else if (strcmp(token->lexeme, "func") == 0) token->type = TOKEN_FUNC;
    else if (strcmp(token->lexeme, "return") == 0) token->type = TOKEN_RETURN;
    else if (strcmp(token->lexeme, "if") == 0) token->type = TOKEN_IF;
    else if (strcmp(token->lexeme, "else") == 0) token->type = TOKEN_ELSE;
    else if (strcmp(token->lexeme, "while") == 0) token->type = TOKEN_WHILE;
    else if (strcmp(token->lexeme, "for") == 0) token->type = TOKEN_FOR;
    else if (strcmp(token->lexeme, "break") == 0) token->type = TOKEN_BREAK;
    else if (strcmp(token->lexeme, "continue") == 0) token->type = TOKEN_CONTINUE;
    else if (strcmp(token->lexeme, "true") == 0) token->type = TOKEN_TRUE;
    else if (strcmp(token->lexeme, "false") == 0) token->type = TOKEN_FALSE;
    else if (strcmp(token->lexeme, "null") == 0) token->type = TOKEN_NULL;
    else if (strcmp(token->lexeme, "import") == 0) token->type = TOKEN_IMPORT;
    
    return token;
}

Token* lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    lexer->start = lexer->current;
    
    int ch = peek(lexer);
    if (ch == '\0') {
        return token_new(TOKEN_EOF, "", lexer->line, lexer->column);
    }
    
    if (ch == '\n') {
        advance(lexer);
        return token_new(TOKEN_NEWLINE, "\n", lexer->line - 1, lexer->column);
    }
    
    if (isdigit(ch)) {
        return scan_number(lexer);
    }
    
    if (ch == '"' || ch == '\'') {
        return scan_string(lexer, ch);
    }
    
    if (isalpha(ch) || ch == '_') {
        return scan_identifier(lexer);
    }
    
    advance(lexer);
    
    switch (ch) {
        case '(': return make_token(lexer, TOKEN_LEFT_PAREN);
        case ')': return make_token(lexer, TOKEN_RIGHT_PAREN);
        case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
        case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
        case '[': return make_token(lexer, TOKEN_LEFT_BRACKET);
        case ']': return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '.': return make_token(lexer, TOKEN_DOT);
        case ':': return make_token(lexer, TOKEN_COLON);
        case ';': return make_token(lexer, TOKEN_SEMICOLON);
        
        case '+':
            if (peek(lexer) == '+') {
                advance(lexer);
                return make_token(lexer, TOKEN_INCREMENT);
            } else if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_PLUS_EQUAL);
            }
            return make_token(lexer, TOKEN_PLUS);
        
        case '-':
            if (peek(lexer) == '-') {
                advance(lexer);
                return make_token(lexer, TOKEN_DECREMENT);
            } else if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_MINUS_EQUAL);
            } else if (peek(lexer) == '>') {
                advance(lexer);
                return make_token(lexer, TOKEN_ARROW);
            }
            return make_token(lexer, TOKEN_MINUS);
        
        case '*':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_STAR_EQUAL);
            }
            return make_token(lexer, TOKEN_STAR);
        
        case '/':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_SLASH_EQUAL);
            }
            return make_token(lexer, TOKEN_SLASH);
        
        case '%': return make_token(lexer, TOKEN_PERCENT);
        
        case '=':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_EQUAL_EQUAL);
            }
            return make_token(lexer, TOKEN_EQUAL);
        
        case '!':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_BANG_EQUAL);
            }
            return make_token(lexer, TOKEN_BANG);
        
        case '<':
            if (peek(lexer) == '<') {
                advance(lexer);
                return make_token(lexer, TOKEN_LSHIFT);
            } else if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_LESS_EQUAL);
            }
            return make_token(lexer, TOKEN_LESS);
        
        case '>':
            if (peek(lexer) == '>') {
                advance(lexer);
                return make_token(lexer, TOKEN_RSHIFT);
            } else if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_GREATER_EQUAL);
            }
            return make_token(lexer, TOKEN_GREATER);
        
        case '&':
            if (peek(lexer) == '&') {
                advance(lexer);
                return make_token(lexer, TOKEN_AND);
            }
            return make_token(lexer, TOKEN_AMPERSAND);
        
        case '|':
            if (peek(lexer) == '|') {
                advance(lexer);
                return make_token(lexer, TOKEN_OR);
            }
            return make_token(lexer, TOKEN_PIPE);
        
        case '^': return make_token(lexer, TOKEN_CARET);
        case '~': return make_token(lexer, TOKEN_TILDE);
        
        default:
            return token_new(TOKEN_ERROR, "Unexpected character", lexer->line, lexer->column - 1);
    }
}
