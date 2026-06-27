#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <tuple>

#include "token.h"
#include "expr.h"

using namespace std;

class Parser {
    public:
        vector<Token> tokens;
        vector<tuple<Token, Expr>> commands;
        Parser(vector<Token> tokens);

        void parse();
    private:
        int current = 0;
        //unordered_map<Token, Expr> env;
        
        Token peek();

        void parseExpr();
        void parseNamedExpr();
        void parseOut();
        void parseInclude();
        void insertFunction(Token token);

        Expr createAST(vector<Token> exprTokens);

};

#endif