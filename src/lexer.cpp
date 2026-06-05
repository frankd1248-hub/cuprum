#include "lexer.h"

std::vector<Token> Lexer::tokenize() {
    tokens = {};
    start = 0;
    current = 0;
    lineStart = 0;
    line = 1;

    while (!isAtEnd()) {
        skipWhitespace();
        start = current;
        if (isAtEnd()) break;

        char c = advance();
        if (isAlpha(c)) {
            while (isAlpha(peek()) || isDigit(peek())) advance();
            std::string lexeme = src.substr(start, current - start);
            addToken(identifierType(lexeme), lexeme);
        } else if (isDigit(c)) {
            while (isDigit(peek())) advance();
            std::string lexeme = src.substr(start, current - start);
            addToken(TK_NUMBER, lexeme);
        } else {
            switch (c) {
                case '(': addToken(TK_LEFT_PAREN, "("); break;
                case ')': addToken(TK_RIGHT_PAREN, ")"); break;
                case '{': addToken(TK_LEFT_BRACE, "{"); break;
                case '}': addToken(TK_RIGHT_BRACE, "}"); break;
                case ';': addToken(TK_SEMICOLON, ";"); break;
                case ':': addToken(TK_COLON, ":"); break;
                case ',': addToken(TK_COMMA, ","); break;
                case '-': addToken(TK_MINUS, "-"); break;
                case '+': addToken(TK_PLUS, "+"); break;
                case '*': addToken(TK_STAR, "*"); break;
                case '/': 
                    if (!match('/')) {
                        addToken(TK_SLASH, "/"); break;
                    }

                    while (!match('\n')) {
                        advance();
                    }

                    line++;
                    lineStart = current;
                    break;
                case '!': match('=') ? addToken(TK_BANG_EQUAL, "!=") : addToken(TK_BANG, "!"); break;
                case '<': match('=') ? addToken(TK_LESS_EQUAL, "<=") : addToken(TK_LESS, "<"); break;
                case '>': match('=') ? addToken(TK_GREATER_EQUAL, ">=") : addToken(TK_GREATER, ">"); break;
                case '=': 
                    if (match('>'))      addToken(TK_ARROW, "=>");
                    else if (match('=')) addToken(TK_EQUAL_EQUAL, "==");
                    else                 addToken(TK_EQUAL, "=");
                    break;
                default:
                    err.report(line, current - lineStart, "Unexpected character.");
            }
        }
    }
    
    tokens.push_back({ TK_EOF, "", line, 0 });
    return tokens;
}