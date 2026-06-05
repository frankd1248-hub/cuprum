#ifndef ferrum_token_h
#define ferrum_token_h

#include "common.h"

#include <print>
#include <unordered_map>

enum TokenType {
    TK_EOF,

    TK_LEFT_PAREN, TK_RIGHT_PAREN, TK_LEFT_BRACE, TK_RIGHT_BRACE,

    TK_ARROW, // =>

    TK_SEMICOLON, TK_COLON, TK_COMMA,

    TK_MINUS, TK_PLUS, TK_STAR, TK_SLASH,
    TK_EQUAL,

    TK_EQUAL_EQUAL, TK_BANG_EQUAL, TK_LESS, TK_LESS_EQUAL,
    TK_GREATER, TK_GREATER_EQUAL, TK_BANG,

    TK_NUMBER,

    TK_I32, TK_VOID, TK_BOOL,

    TK_TRUE, TK_FALSE,

    TK_IDENTIFIER,

    TK_LET, TK_CONST,

    TK_RETURN, TK_FN, TK_IF, TK_ELSE,
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int col;
};

extern std::unordered_map<TokenType, std::string> tokenTypeLookup;

inline void printTokens(std::ostream& out, std::vector<Token> tokens) {
    for (auto& token : tokens) {
        std::print(out, "Ln {:03d}, col {:02d}, type: {}, lexeme: '{}'\n",
            token.line, token.col, 
            tokenTypeLookup.at(token.type), 
            token.lexeme
        );
    }
}

#endif