#include <bits/stdc++.h>
using namespace std;

int line = 1;
int op_count = 0;

bool isOperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        return true;
    else
        return false;
}

void printOutput() {
    if (op_count > 0) {
        cout << "\nLine no. " << line << ": " << op_count << endl;
    }
    op_count = 0;
    line++;
}

int main(){
    fstream inputFile("input.txt", ios::in);
    char c;
    while (!inputFile.eof()) {
        c = inputFile.get();

        if (c == '\n'){
            printOutput();
        }
        else if (c == '#') {
            while (inputFile.get() != '\n')
                continue;
            printOutput();
        }
        else if (c == '/') {
            if (inputFile.get() == '/') {
                while (inputFile.get() != '\n')
                    continue;
                printOutput();
            }
            else if (inputFile.get() == '*') {
                while (c = inputFile.get() != '/') {
                    if (c == '\n'){
                        printOutput();
                    }
                }
            }
        }
        else if (c == '"') {
            while (inputFile.get() != '"')
                continue;
        }
        else {
            bool curr, next;
            curr = isOperator(c);
            if (curr == true) {
                c = inputFile.get();
                next = isOperator(c);
                if (next == true) {
                    op_count++;
                }
                else {
                    op_count++;
                }
            }
        }
    }
}