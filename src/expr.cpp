#include "expr.h"
#include <iostream>
#include <span>
#include <vector>
#include <memory>
using namespace std;

class Scope {
    public:
        unordered_map<string, int> env;
        Scope* parent;
        int lambdaCount;

        Scope(unordered_map<string, int> e, Scope* p, int l) : env(e), parent(p), lambdaCount(l) {}

        Scope* createFunctionalChildScope() {
            unordered_map<string, int> outenv = env;
            for (auto& [key, value] : outenv) {
                value++;
            }
            return new Scope(outenv, this, lambdaCount+1);
        }

        Scope* createChildScope() {
            return new Scope(env, this, lambdaCount);
        }
};

Expr::Expr(vector<Token> literals) {
    this->literals = literals;
}

string Expr::toString() const {
    string out = "";
    for (Token token : literals) {
        out += token.lexeme + " ";
    }
    return out;
}

string Expr::indicesToString() const {
    string out = "";
    for (Token token : indices) {
        out += token.lexeme + " ";
    }
    return out;
}

void Expr::printToCurrent() const {
    for (int i = 0; i <= current; ++i) {
        cout << literals.at(i).lexeme << " ";
    }
    cout << endl;
}

void Expr::print(int start, int end) const {
    for (int i = start; i <= end; ++i) {
        cout << literals.at(i).lexeme << " ";
    }
    cout << endl;
}

void Expr::expand() {
    while (current < literals.size()) {
        switch(literals.at(current).type) {
            case TokenType::LEFT_PAREN:
                if (peek().type == TokenType::LAMBDA) { expandFunction(); }
                else { expandAppl(); }
                break;
        }
        current++;
    }
}

void Expr::expandInner() {
    switch(literals.at(current).type) {
        case TokenType::LEFT_PAREN:
            if (peek().type == TokenType::LAMBDA) { expandFunction(); }
            else { expandAppl(); }
    }
}

void Expr::expandFunction() {
    int start = current;
    int end = findEndParen();

    //cout << "func: ";
    //print(start, end);

    current++; // consume (
    current++; // consume "lambda"

    int numExtraArgs = 0;

    vector<Token> argSplitter = {Token(TokenType::COLON, ":"), Token(TokenType::LEFT_PAREN, "("), Token(TokenType::LAMBDA, "kwlambda")};

    while (peek().type != TokenType::COLON) {
        literals.insert(literals.begin() + current + 1, argSplitter.begin(), argSplitter.end());
        end += 4;
        literals.insert(literals.begin() + end, Token(TokenType::RIGHT_PAREN, ")"));
        current+=4;
        numExtraArgs++;
        
    }
    current++; // consume ":"
    if (peek().type == TokenType::LEFT_PAREN) {
        current++;
        expandInner();
        current += numExtraArgs + 1;
    }
    else {
        current = end;
    }
    //cout << "funcout: ";
    //print(start, current);

}

void Expr::expandAppl() {
    int start = current;
    int end = findEndParen();

    //cout << "appl: ";
    //print(start, end);

    current++; // consume (, current token should be ( or EXPRNAME or Variable
    if (literals.at(current).type == TokenType::EXPRNAME || literals.at(current).type == TokenType::VARIABLE) { current++; } // consume EXPRNAME/var
    else { expandInner(); current++; } // expand inner function
    while (current < literals.size()) {
        
        if (literals.at(current).type == TokenType::LEFT_PAREN) { expandInner();}

        if (peek().type == TokenType::RIGHT_PAREN) { break; }
        literals.insert(literals.begin() + current+1, Token(TokenType::RIGHT_PAREN, ")"));
        literals.insert(literals.begin() + start, Token(TokenType::LEFT_PAREN, "("));
        current += 3;
    }
    current += 1;
    //cout << "applout: ";
    //print(start, current);

}

void Expr::index() {
    int tmpCounter = 0;

    vector<unique_ptr<Scope>> scopes;

    unique_ptr<Scope> global =  make_unique<Scope>(varCounters, nullptr, 0);
    Scope* curScope = global.get();

    scopes.push_back(move(global));

    while (tmpCounter < literals.size()) {
        Token token = literals.at(tmpCounter);
        cout << scopes.size();
        switch (token.type) {

            case TokenType::LEFT_PAREN:
                indices.push_back(token);
                if (peek(tmpCounter).type == TokenType::LAMBDA) { 
                    scopes.push_back( unique_ptr<Scope>(curScope->createFunctionalChildScope()) ); 
                } 
                else {
                    scopes.push_back( unique_ptr<Scope>(curScope->createChildScope()) ); 
                }
                

                curScope = scopes.back().get();
                break;

            case TokenType::RIGHT_PAREN:
                indices.push_back(token);
                if (curScope->parent == nullptr) {
                    cout << "nptr err";
                }
                curScope = curScope->parent;
                break;

            case TokenType::LAMBDA:
                indices.push_back(token);
                curScope->env[peek(tmpCounter).lexeme] = 0;
                tmpCounter++; // consume param
                break;
            case TokenType::VARIABLE:
                try {
                    indices.push_back(Token(TokenType::VARIABLE, curScope->env.at(token.lexeme)));
                }
                catch (out_of_range) {
                    indices.push_back(Token(TokenType::VARIABLE, curScope->lambdaCount));
                }
            break;
                
        }
        tmpCounter++;
    }
}

Token Expr::peek() {
    return literals.at(current+1);
}

Token Expr::peek(int index) {
    return literals.at(index+1);
}

int Expr::findEndParen() {
    int tempcur = current;
    int unmatchedParen = 1;
    tempcur++; // consume (
    while (unmatchedParen != 0) {
        Token token = literals.at(tempcur);
        if (token.type == TokenType::LEFT_PAREN) { unmatchedParen++; }
        else if (token.type == TokenType::RIGHT_PAREN ) { unmatchedParen--; }
        tempcur++;
    }
    return tempcur - 1;
}