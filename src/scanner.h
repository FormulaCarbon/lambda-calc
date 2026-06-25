#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

#include "types.h"
#include "token.h"

using namespace std;

class Scanner {
    public:
        string source;
        vector<Token> tokens;

        Scanner(string source);
        
        vector<Token> scan();

    private:
        int current = 0;

        string scanString();
        bool isUpperCase(string str) const;
};

#endif