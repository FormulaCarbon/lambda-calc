#include <iostream>

#include "parser.h"
#include "expr.h"

using namespace std;

Parser::Parser(vector<Token> tokens) {
    this->tokens = tokens;
}

void Parser::parse() {
    while (current < tokens.size()) {
        Token token = tokens.at(current);
        switch (token.type) {
            case TokenType::NAME: parseNamedExpr(); break;
            case TokenType::LEFT_PAREN: parseExpr(); break;
            case TokenType::OUT: parseOut(); break;
            case TokenType::INCLUDE: parseInclude(); break;

            default: break;
        }
        current++;
    }
}

Token Parser::peek() {
    return tokens.at(current + 1);
}

void Parser::parseExpr() {
    int start = current;
    int unmatched_paren = 1;
    current++; // consume (

    while (unmatched_paren != 0) {
        Token token = tokens.at(current);
        if (token.type == TokenType::LEFT_PAREN) { unmatched_paren++; }
        else if (token.type == TokenType::RIGHT_PAREN) { unmatched_paren--; }
        current++;
    }
    current--;
    vector<Token> exprTokens(tokens.begin() + start, tokens.begin() + current+1);
    commands.push_back(make_tuple(tokens.at(start), Expr(exprTokens)));
}

void Parser::parseNamedExpr() {
    current++; // consume "name"
    Token exprName = tokens.at(current);
    current++; // store and consume EXPRNAME
    current++; // consume {
    int start = current;
    while (tokens.at(current).type != TokenType::RIGHT_BRACKET) {
        current++;
    }

    vector<Token> exprTokens(tokens.begin() + start, tokens.begin() + current);
    commands.push_back(make_tuple(exprName, Expr(exprTokens)));
}

void Parser::parseOut() {
    Token exprName = tokens.at(current);
    current++; // store and consume "out"
    current++; // consume {
    int start = current;
    while (tokens.at(current).type != TokenType::RIGHT_BRACKET) {
        current++;
    }

    vector<Token> exprTokens(tokens.begin() + start, tokens.begin() + current);
    commands.push_back(make_tuple(exprName, Expr(exprTokens)));
}

void Parser::parseInclude() {
    Token exprName = tokens.at(current);
    current++; // store and consume "include"
    current++; // consume {
    int start = current;
    while (tokens.at(current).type != TokenType::RIGHT_BRACKET) {
        current++;
    }

    vector<Token> exprTokens(tokens.begin() + start, tokens.begin() + current);
    commands.push_back(make_tuple(exprName, Expr(exprTokens)));
}


