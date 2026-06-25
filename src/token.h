#ifndef TOKEN_H
#define TOKEN_H

#include "types.h"

class Token {
    public:
        TokenType type;
        string lexeme;
        int index;

        Token(TokenType type, string lexeme);
        Token(TokenType type, int index);
};

#endif