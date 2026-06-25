#include <cctype>

#include "scanner.h"

using namespace std;

Scanner::Scanner(string source) {
    this->source = source;
}

vector<Token> Scanner::scan() {
    while (current < source.length()) {
        char c = source.at(current);
        switch (c) {
            case '{': tokens.push_back(Token(TokenType::LEFT_BRACKET, "{")); break;
            case '}': tokens.push_back(Token(TokenType::RIGHT_BRACKET, "}")); break;
            case '(': tokens.push_back(Token(TokenType::LEFT_PAREN, "(")); break;
            case ')': tokens.push_back(Token(TokenType::RIGHT_PAREN, ")")); break;
            case ':': tokens.push_back(Token(TokenType::COLON, ":")); break;

            case '\n':
            case '\t':
            case ' ':
                break;
            
            default:
                string str = scanString();
                if (str == "lambda") {
                    tokens.push_back(Token(TokenType::LAMBDA, "kwlambda"));
                } else if (str == "name") {
                    tokens.push_back(Token(TokenType::NAME, "kwname"));
                } else if (str == "include") {
                    tokens.push_back(Token(TokenType::INCLUDE, "kwinclude"));
                } else if (str == "out") {
                    tokens.push_back(Token(TokenType::INCLUDE, "kwout"));
                } else if (isUpperCase(str)) {
                    tokens.push_back(Token(TokenType::EXPRNAME, str));
                } else {
                    tokens.push_back(Token(TokenType::VARIABLE, str));
                }
        }
        current++;
    } 
    return tokens;   
}

string Scanner::scanString() {
    int start = current;
    while (current + 1 < source.length() && (isalnum(source.at(current + 1)) || source.at(current + 1) == '_')) {
        current++;
    }
    return source.substr(start, (current + 1) - start);
}

bool Scanner::isUpperCase(string str) const {
    for (char c : str) {
        if (isalpha(c) && islower(c)) return false;
    }
    return true;
}