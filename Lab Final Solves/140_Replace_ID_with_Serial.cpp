
#include <bits/stdc++.h>
using namespace std;

struct symbolInfo
{
    string slNo, name, idType;
} s;

vector<symbolInfo> symbolTable;

fstream inputFile1("symbol_table.txt", ios::in);
fstream inputFile2("tokens.txt", ios::in);
fstream outputFile("output.txt", ios::out);

string lookup(string n)
{
    for(int i = 0; i < symbolTable.size(); i++)
    {
        if(symbolTable[i].name == n)
            return symbolTable[i].slNo;
    }
    return "-1";
}

int main()
{
    while (!inputFile1.eof())
    {
        inputFile1 >> s.slNo >> s.name >> s.idType;
        symbolTable.push_back(s);
    }
    inputFile1.close();

    string tokens; 
    while(!inputFile2.eof())
    {
        tokens.push_back(inputFile2.get());
    }
    inputFile2.close();

    for(int i = 0; i < tokens.length(); i++)
    {
        if(tokens[i] == 'i' && tokens[i+1] == 'd')
        {
            i = i + 3;
            string idName, serial;
            while(tokens[i] != ']')
            {
                idName.push_back(tokens[i]);
                i++;
            }

            serial = lookup(idName);
            outputFile << "id " << serial << "]";
        }
        else
        {
            outputFile << tokens[i];
        }
    }

    outputFile.close();
    outputFile.open("output.txt", ios::in);

    while(!outputFile.eof())
    {
        cout << outputFile.get();
    }
    outputFile.close();

}
