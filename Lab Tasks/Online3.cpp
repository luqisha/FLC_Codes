/*
ID: 190104140
Group : C2
Online : 3
Date: 10/01/23
Task: Take a Symbol Table as input from a file and insert a new row in that table to a given index taken from user
*/

#include <bits/stdc++.h>

using namespace std;

struct symbolInfo
{
    int slNo;
    string name, idType;
} s;

vector<symbolInfo> symbolTable;
fstream inputFile("input_online3.txt", ios::in);
fstream outputFile("output_online3.txt", ios::out);

int main()
{
    string str, word;
    while (!inputFile.eof())
    {
        inputFile >> s.slNo >> s.name >> s.idType;
        symbolTable.push_back(s);
    }
    cout << "The updated Symbol Table: " << endl;
    for (int i = 0; i < symbolTable.size(); i++)
    {
        cout << symbolTable[i].slNo << "\t" << symbolTable[i].name << "\t" << symbolTable[i].idType << endl;
    }

    string newName, newType;
    cout << "Insert Data: ";
    cin >> newName >> newType;

    int insertAt;
    cout << "Insert At: ";
    cin >> insertAt;

    s.slNo = insertAt;
    s.name = newName;
    s.idType = newType;

    symbolTable.insert(symbolTable.begin()+insertAt , s);

    cout << "The updated Symbol Table: " << endl;
    for (int i = 0; i < symbolTable.size(); i++)
    {
        cout << symbolTable[i].slNo << "\t" << symbolTable[i].name << "\t" << symbolTable[i].idType << endl;
    }
}
