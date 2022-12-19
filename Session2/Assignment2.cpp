#include <bits/stdc++.h>

using namespace std;

vector<string> separators = {";", "=", ",", "+", "-", "*", "/", ">", "<", "\"", "'"},

               operators = {"=", "+", "-", "*", "/", ">", "<", "&", "!", "~", "^", "|", "=="
                            "!=", ">=", "<=", "&&", "||", ">>", "<<"},

               parantheses = {"(", ")", "{", "}", "[", "]"},

               keywords = {"auto", "double", "int", "struct", "break", "else", "long", "switch",
                           "case", "enum", "register", "typedef", "char", "extern", "return", "union",
                           "continue", "for", "signed", "void", "do", "if", "static", "while",
                           "default", "goto", "sizeof", "volatile", "const", "float", "short", "unsigned"};

bool identifyToken(string token, vector <string> words)
{
    if (find(words.begin(), words.end(), token) == words.end())
    {
        return false;
    }
        return true;
}

bool isIdentifier(string id)
{
    for (int i = 0; i < id.size(); i++)
    {
        if ((!isalpha(id[0]) && id[0]!= '_')  || (!isalnum(id[i]) && !(id[i] == '_')))
        {
            return false;
        }
    }
    return true;
}

bool isNumeric(string num)
{
    for (int i = 0; i < num.size(); i++)
    {
        if ((!isdigit(num[i]) && (num[i] != '.')) || num.back() == '.' || num.front() == '.')
        {
            return false;
        }
    }
    return true;
}

int main()
{
    string str, token;
    fstream inputFile("input.c", ios::in);
    fstream outputFile("output.txt", ios::out);

    getline(inputFile, str);
    cout << "Input : \n" << str << endl;

    int i = 0;
    while (i < str.size())
    {
        string curr(1, str[i]);
        string next(1, str[i + 1]);

        if (identifyToken(curr, operators) && identifyToken(next, operators))
        {
                str.insert(i, " ");
                str.insert(i + 3, " ");
                i += 4; 
        }
        else if (identifyToken(curr, separators) || identifyToken(curr, parantheses) || 
                (identifyToken(curr, operators) && !identifyToken(next, operators)))
        {
            str.insert(i, " ");
            str.insert(i + 2, " ");
            i += 3;
        }
        else
        {
            i++;
        }
    }

    for(int i = 0 ; i < str.size(); i++)
    {
        if(str[i] == ' ' && str[i+1] == ' ') str.erase(i, 1);
    }

    stringstream ss(str);
    while (getline(ss, token, ' '))
    {
        if(identifyToken(token, keywords))
        {
            outputFile << "[kw " << token << "] ";
        }
        else if(identifyToken(token, operators))
        {
            outputFile << "[op " << token << "] ";
        }
        else if(identifyToken(token, separators))
        {
            outputFile << "[sep " << token << "] ";
        }
        else if(identifyToken(token, parantheses))
        {
            outputFile << "[par " << token << "] ";
        }
        else if (isIdentifier(token))
        {
            outputFile << "[id " << token << "] ";
        }
        else if (isNumeric(token))
        {
            outputFile << "[num " << token << "] ";
        }
        else
        {
            outputFile << "[unkn " << token << "] ";
        }
    }

    inputFile.close();
    outputFile.close();
    
    outputFile.open("output.txt", ios::in);

    getline(outputFile, str);
    cout << "\n\nOutput: \n" << str << endl;
    outputFile.close();
}
