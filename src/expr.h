#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <unordered_map>

#include "token.h"

using namespace std;

class Expr {
    public:
        vector<Token> literals;
        vector<Token> indices;

        Expr(vector<Token> literals);
        //~Expr();
        void expand();
        void index();
        //string evaluate();
        string toString() const;
        string indicesToString() const;

    private:
        int current = 0;
        int icurrent = 0;

        unordered_map<string, int> varCounters;

        void expandFunction();
        void expandAppl();

        int findEndParen();
        Token peek();
        Token peek(int index);
        void printToCurrent() const;
        void print(int start, int end) const;

        void expandInner();
};

#endif