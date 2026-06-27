#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <unordered_map>
#include <variant>
#include <memory>

#include "token.h"

using namespace std;

struct ExprTree;

struct FuncTree {
    unique_ptr<ExprTree> body;

    string toString() const; 
};

struct ApplTree {
    unique_ptr<ExprTree> func;
    unique_ptr<ExprTree> arg;

    string toString() const;
};

struct VarTree {
    Token token;
    string toString() const;
};

struct ExprTree {
    variant<VarTree, ApplTree, FuncTree> node;

    string toString() const;
};

class Expr {
    public:
        vector<Token> literals;
        vector<Token> indices;

        Expr(vector<Token> literals);
        //~Expr();
        void expand();
        void index();
        ExprTree toAST();
        //string evaluate();
        string toString() const;
        string indicesToString() const;

    private:
        int current = 0;
        int icurrent = 0;
        int astcurrent = 0;

        unordered_map<string, int> varCounters;

        void expandFunction();
        void expandAppl();

        int findEndParen();
        int findEndParen(vector<Token> tokens, int current);
        Token peek();
        Token peek(int index);
        void printToCurrent() const;
        void print(int start, int end) const;

        void expandInner();
};

#endif