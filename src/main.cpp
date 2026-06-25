#include <iostream>
#include <string>
#include <tuple>

#include "scanner.h"
#include "parser.h"

using namespace std;

string run(string source);
void runShell();
void runFile();

int main() {
    runShell();
}

void runShell() {
    string input = "";
    while (input != "exit") {
        cout << ">>> ";
        getline(cin, input);
        cout << run(input);
    }
}

string run(string source) {
    Scanner scanner = Scanner(source);
    vector<Token> tokens = scanner.scan();

    Parser parser = Parser(tokens);
    parser.parse();
   
    for (auto& [token, expr] : parser.commands) {
        expr.expand();
        cout << expr.toString() << endl;
    }
    return "";
}