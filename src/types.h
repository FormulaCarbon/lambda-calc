#ifndef TYPES_H
#define TYPES_H

#include <string>

using namespace std;

enum class TokenType {
    // Symbols
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, COLON,

    // Values
    VARIABLE, EXPRNAME,

    // Keywords
    LAMBDA, NAME, INCLUDE, OUT
};

#endif