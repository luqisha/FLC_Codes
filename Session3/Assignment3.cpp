#include <bits/stdc++.h>

using namespace std;

struct symbolInfo
{
    int slNo;
    string name, idType, dataType, scope, value;

    symbolInfo() {}
    symbolInfo(int _slNo, string _name, string _idType, string _dataType, string _scope)
    {
        slNo = _slNo;
        name = _name;
        idType = _idType;
        dataType = _dataType;
        scope = _scope;
    }
};

vector<symbolInfo> symbolTable;
fstream inputFile("input.txt", ios::in);
fstream outputFile("output.txt", ios::out);

int lookupSymbolTable(string name, string idType, string scope)
{
    for (int i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].name == name && symbolTable[i].idType == idType &&
            symbolTable[i].scope == scope)
        {
            return i;
        }
    }
    return -1;
}

int lookupSymbolTable(string name, string scope)
{
    for (int i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].name == name && symbolTable[i].scope == scope)
        {
            return i;
        }
    }
    return -1;
}

bool insertSymbol(symbolInfo s)
{
    if (lookupSymbolTable(s.name, s.idType, s.scope) == -1)
    {
        symbolTable.push_back(s);
        return true;
    }
    else
        return false;
}

bool updateSymbolInfo(string toUpdate, string name, string scope, string updatedValue)
{
    int index;
    if (toUpdate == "value" && lookupSymbolTable(name, "var", scope) != -1)
    {
        index = lookupSymbolTable(name, "var", scope);
        symbolTable[index].value = updatedValue;
        return true;
    }
    else if (toUpdate == "idType" && lookupSymbolTable(name, "var", scope) != -1 && lookupSymbolTable(name, "func", scope) == -1)
    {
        index = lookupSymbolTable(name, "var", scope);
        symbolTable[index].idType = updatedValue;
        return true;
    }
    else if (toUpdate == "idScope" && lookupSymbolTable(name, "func", scope) != -1)
    {
        index = lookupSymbolTable(name, "func", scope);
        symbolTable[index].scope = "global";
        return true;
    }
    else if (toUpdate == "idScope" && lookupSymbolTable(name, "var", scope) != -1)
    {
        index = lookupSymbolTable(name, "var", scope);
        symbolTable[index].scope = updatedValue;
        return true;
    }
    else if (toUpdate == "value" && lookupSymbolTable(name, scope) != -1)
    {
        index = lookupSymbolTable(name, scope);
        symbolTable[index].scope = updatedValue;
        return true;
    }
    else
        return false;
}

bool freeSymbolTable()
{
    symbolTable.clear();
    if (symbolTable.size() == 0)
        return true;
    else
        return false;
}

void displaySymbolTable()
{
    cout << "\n_________________________________________________" << endl;
    cout << "Symbol Table" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < symbolTable.size(); i++)
    {
        cout << symbolTable[i].slNo << "\t" << symbolTable[i].name << "\t"
             << symbolTable[i].idType << "\t" << symbolTable[i].dataType << "\t"
             << symbolTable[i].scope << "\t" << symbolTable[i].value << endl;
    }
    cout << "-------------------------------------------------" << endl;
}

string findScope()
{
    for (int i = symbolTable.size() - 1; i >= 0; i--)
    {
        if (symbolTable[i].idType == "func")
            return symbolTable[i].name;
    }
    return "global";
}

int main()
{
    string output, currToken, prevToken = "", currSymbol, prevSymbol, varName, idScope;
    int count = 0, choice;

    getline(inputFile, output);
    cout << "Tokenized Input : \n" << output << endl;

    inputFile.close();
    inputFile.open("input.txt", ios::in);

    while (!inputFile.eof())
    {
        getline(inputFile, currToken, ']');
        currToken.erase(0, 2);

        currSymbol = currToken.substr(currToken.find(" ") + 1);
        prevSymbol = prevToken.empty() ? "" : prevToken.substr(prevToken.find(" ") + 1);

        if (currToken.substr(0, 2) == "id")
        {
            varName = currSymbol;
            idScope = findScope();
        }

        if (currToken.substr(0, 2) == "id" && prevToken.substr(0, 2) == "kw" &&
            prevSymbol != "return" && lookupSymbolTable(currSymbol, "var", idScope) == -1)
        {
            insertSymbol(symbolInfo(++count, currSymbol, "var", prevSymbol, idScope));
            outputFile << "["
                       << "id " << to_string(count) << "] ";
        }
        else if (currSymbol == "(" && prevToken.substr(0, 2) == "id")
        {
            updateSymbolInfo("idType", prevSymbol, idScope, "func");
            updateSymbolInfo("idScope", prevSymbol, idScope, "global");
            outputFile << "[" << currSymbol << "] ";
        }
        else if (currToken.substr(0, 3) == "num" && prevSymbol == "=")
        {
            updateSymbolInfo("value", varName, idScope, currSymbol);
            outputFile << "[" << currSymbol << "] ";
        }
        else if (!currToken.empty())
        {
            if (currToken.substr(0, 2) == "id")
            {
                if (prevSymbol == "=")
                {
                    idScope = "global";
                }
                int idSl = lookupSymbolTable(currSymbol, idScope) + 1;
                outputFile << "[" << "id " << to_string(idSl) << "] ";
            }
            else
                outputFile << "[" << currSymbol << "] ";
        }
        prevToken = currToken;
    }
    inputFile.close();
    outputFile.close();

    fstream outputFile("output.txt", ios::in);
    getline(outputFile, output);
    cout << "\nModified Token stream for Syntax Analysis: \n"<< output << endl;
    outputFile.close();

    displaySymbolTable();
}