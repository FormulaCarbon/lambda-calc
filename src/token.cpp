#include "token.h"

using namespace std;

Token::Token(TokenType type, string lexeme) {
    this->type = type;
    this->lexeme = lexeme;
}

Token::Token(TokenType type, int index) {
    this->type = type;
    this->index = index;
}