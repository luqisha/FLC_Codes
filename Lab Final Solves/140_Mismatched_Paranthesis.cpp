#include <bits/stdc++.h>
using namespace std;

int main(){
    fstream inputFile("input.txt", ios::in);
    char c;
    string par, errorLine;
    int lineNum = 1;;
    while (!inputFile.eof()) {
        c = inputFile.get();

        if(c == '\n') lineNum++;
        if (c == '{' || c == '(')
        {
            par.push_back(c);
            errorLine.push_back(lineNum +'0');
        }
        else if(c == ')')
        {
            if(par.empty() || par.back() !=  '(') cout << "Mismatched Paranthesis at line " << lineNum << endl;
            else
            {
                par.pop_back();
                errorLine.pop_back();
            }
        }
        else if(c == '}')
        {
            if(par.empty() || par.back() !=  '{') cout << "Mismatched Paranthesis at line " << lineNum << endl;
            else
            {
                par.pop_back();
                errorLine.pop_back();
            }
        }      
    }

    if(!errorLine.empty())
    {
        for(int i = 0; i < errorLine.size(); i++)
        {
            cout << "Mismatched Paranthesis at line " << errorLine[i] << endl;
        }
    }
}