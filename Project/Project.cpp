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
fstream inputFile;
fstream outputFile;

vector<string> separators = {";", "=", ",", "+", "-", "*", "/", ">", "<", "\"", "'"},
               operators = {"=", "+", "-", "*", "/", ">", "<", "&", "!", "~", "^", "|", "=="
                                                                                        "!=",
                            ">=", "<=", "&&", "||", ">>", "<<"},
               parantheses = {"(", ")", "{", "}", "[", "]"},
               keywords = {"auto", "double", "int", "struct", "break", "else", "long", "switch",
                           "case", "enum", "register", "typedef", "char", "extern", "return", "union",
                           "continue", "for", "signed", "void", "do", "if", "static", "while",
                           "default", "goto", "sizeof", "volatile", "const", "float", "short", "unsigned"};
vector<string> headers;
vector<pair<string, int>> errors;
stack<pair<string, int>> parStack;
stack<pair<string, string>> ifStack;

bool identifyToken(string token, vector<string> words)
{
    if (find(words.begin(), words.end(), token) == words.end())
    {
        return false;
    }
    return true;
}

bool checkExpression(string str, int* i)
{
    if(str[*i]=='i'||str[*i]=='j'||str[*i]=='k')
    {
        (*i)++;
        if(str[*i] == '+' || str[*i] == '-' || str[*i] == '*' || str[*i] == '/')
        {
            (*i)++;
            if(str[*i]=='i'||str[*i]=='j'||str[*i]=='k') 
                return true;
            else if(isdigit(str[*i])) return true;
            else return false;
        }
        else return false;
    }
    else if(isdigit(str[*i])) return true;
    else return false;
}

bool checkRelExpression(string str, int* i)
{
    if(str[*i]=='i'||str[*i]=='j'||str[*i]=='k')
    {
        (*i)++;
        if(str[*i] == '<' || str[*i] == '>' || str[*i] == '=')
        {
            if (str[*i+1] == '=') (*i) += 2;
            else (*i)++;
            if(str[*i]=='i'||str[*i]=='j'||str[*i]=='k')
                return true;
            else if(isdigit(str[*i]))
                return true;
            else return false;
        }
        else return false;
    }
    else return false;
}

bool checkAssignment(string str, int* i)
{
    if(str[*i]=='i'||str[*i]=='j'||str[*i]=='k')
    {
        (*i)++;
        if(str[*i] == '=')
        {
            (*i)++;
            if(checkExpression(str, i))
                return true;
            else return false;
        }
        else return false;
    }
    else return false;
}

bool checkFor(string line)
{
    int i = 0;
    if (line[i] == '(')
    {
        i++;
        if (checkAssignment(line, &i))
        {              
            i++;
            if (line[i] == ';')
            {
                i++;
                if (checkRelExpression(line, &i))
                {
                    i++;
                    if (line[i] == ';')
                    {
                        i++;
                        if (checkAssignment(line, &i))
                        {
                            i++;
                            if (line[i] == ')')
                            {
                                return true;
                            }    
                            else return false;
                        }
                        else return false;
                    }
                    else return false;
                }
                else return false;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}
    


bool isIdentifier(string id)
{
    for (int i = 0; i < id.size(); i++)
    {
        if ((!isalpha(id[0]) && id[0] != '_') || (!isalnum(id[i]) && !(id[i] == '_')))
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
    else
        return false;
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

void printFile(string file_name)
{
    char c;
    int lineNum = 1;
    outputFile.open(file_name, ios::in);
    std::cout << "File: " << file_name << endl;
    std::cout << "=============================================" << endl;
    cout << lineNum << ". ";
    while (outputFile.get(c))
    {
        if (outputFile.eof())
            break;
        std::cout << c;
        if (c == '\n')
        {
            cout << ++lineNum << ". ";
        }
    }
    outputFile.close();
}

bool comparator(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
{
    return a.second < b.second;
}

bool filterSource(string file_name)
{
    string line, headerName;
    bool inQuote = false, lastCharWasSpace = false, multiLineComment = false;

    inputFile.open(file_name, ios::in);
    outputFile.open("output1.txt", ios::out);

    if (inputFile.is_open())
    {
        while (!inputFile.eof())
        {
            getline(inputFile, line);
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == '#')
                {
                    headerName.erase();
                    while (line[i] != '\0')
                    {
                        headerName.push_back(line[i]);
                        outputFile << line[i];
                        i++;
                    }
                    headers.push_back(headerName);
                    continue;
                }

                if (line[i] == '"')
                {
                    inQuote = !inQuote;
                }

                if (multiLineComment && line[i] == '/' && line[i - 1] == '*')
                {
                    multiLineComment = false;
                }
                else if (multiLineComment)
                {
                    continue;
                }
                else if ((line[i] == '\n' || line[i] == '\t' || line[i] == ' ') && !inQuote)
                {
                    if (lastCharWasSpace)
                    {
                        continue;
                    }
                    else
                    {
                        lastCharWasSpace = true;
                        outputFile << line[i];
                    }
                }
                else if (line[i] == '/' && line[i + 1] == '/')
                {
                    i = line.length() - 1;
                }
                else if (line[i] == '/' && line[i + 1] == '*')
                {
                    multiLineComment = true;
                }
                else
                {
                    lastCharWasSpace = false;
                    outputFile << line[i];
                }
            }
            if (!multiLineComment)
                outputFile << "\n";
        }

        inputFile.close();
        outputFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

void tokenizeSource(string file_name)
{
    string token, str;
    bool inQuote = false;
    int lineNum = 1;
    inputFile.open(file_name, ios::in);
    outputFile.open("output2.txt", ios::out);
    while (getline(inputFile, str))
    {
        outputFile << " ";
        int i = 0;

        // LEXEME SEPARATION STARTS HERE
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

        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == ' ' && str[i + 1] == ' ')
                str.erase(i, 1);
        }
        // LEXEME SEPARATION ENDS HERE
        stringstream ss(str);
        while (getline(ss, token, ' '))
        {
            if (token.empty())
            {
                continue;
            }
            else if (token == "#include")
            {
                char c;
                outputFile << "[hd ";
                while (ss.get(c))
                {
                    if (c != '"' && c != '<' && c != '>' && c != ' ')
                        outputFile << c;
                }
                outputFile << ']';
                break;
            }
            else if (token == "\"")
            {
                while (getline(ss, token, ' '))
                {
                    if (token == "\"")
                    {
                        outputFile << "[str] ";
                        break;
                    }
                }
            }
            else if (token == "for")
            {
                string toParse = str;
                toParse.erase(remove(toParse.begin(), toParse.end(), ' '), toParse.end());

                auto pos = toParse.find("for");
                toParse = toParse.substr(pos + 3);
                
                if(!checkFor(toParse))
                {
                    errors.push_back(make_pair("\nInvalid syntax of 'for loop' at line: ", lineNum));
                }
            }

            if (identifyToken(token, keywords))
            {
                outputFile << "[kw " << token << "] ";
            }
            else if (identifyToken(token, operators))
            {
                outputFile << "[op " << token << "] ";
            }
            else if (identifyToken(token, separators))
            {
                outputFile << "[sep " << token << "] ";
            }
            else if (identifyToken(token, parantheses))
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

        outputFile << "\n";
        lineNum++;
    }
    inputFile.close();
    outputFile.close();
}

void generateSymbolTable(string file_name)
{
    string line, output, currToken, prevToken = "", currSymbol, prevSymbol, varName, idScope;
    int count = 0, choice, lineNum = 1, idSl;
    bool inFor;

    inputFile.open(file_name, ios::in);
    outputFile.open("output3.txt", ios::out);

    while (getline(inputFile, line))
    {
        stringstream lineStream(line);
        while (!lineStream.eof())
        {
            getline(lineStream, currToken, ']');
            currToken.erase(0, 2);

            currSymbol = currToken.substr(currToken.find(" ") + 1);
            prevSymbol = prevToken.empty() ? "" : prevToken.substr(prevToken.find(" ") + 1);

            if (currToken.substr(0, 2) == "id")
            {
                varName = currSymbol;
                idScope = findScope();
            }

            if (identifyToken(currSymbol, parantheses))
            {
                bool skip = false;
                if (!parStack.empty())
                {
                    if (currSymbol == ")" && parStack.top().first == "(")
                    {
                        skip = true;
                    }
                    else if (currSymbol == "}" && parStack.top().first == "{")
                    {
                        skip = true;
                    }
                    else if (currSymbol == "]" && parStack.top().first == "[")
                    {
                        skip = true;
                    }
                }
                if (skip)
                {
                    parStack.pop();
                }
                else
                {
                    parStack.push(make_pair(currSymbol, lineNum));
                }
            }

            if (currSymbol == "if")
            {
                if (prevSymbol == "else")
                {
                    if ((ifStack.top().first != "else if" || ifStack.top().first != "if" ) && ifStack.top().second != findScope())
                    {
                        errors.push_back(make_pair("\nMismatched token 'else if' at line: ", lineNum));
                    }
                    else
                    {
                        ifStack.push(make_pair("else if", findScope()));
                    }
                }

                else
                {
                    ifStack.push(make_pair("if", findScope()));
                }
            }
            else if (currSymbol == "else")
            {
                if (ifStack.empty())
                {
                    errors.push_back(make_pair("\nUnmatched 'else' at line: ", lineNum));
                }
                else
                {
                    while (!ifStack.empty())
                    {
                        ifStack.pop();
                    }
                }
            }
            else if ( currToken.substr(0, 2) == "kw" && currSymbol == prevSymbol)
            {
                errors.push_back(make_pair("\nDuplicate token/keyword '" + currSymbol + "' at line: ", lineNum));
            }
            else if (currToken.substr(0, 2) == "id" && prevToken.substr(0, 2) == "kw" &&
                     prevSymbol != "return" && prevSymbol != "else")
            {
                if (lookupSymbolTable(varName, "var", idScope) == -1)
                {
                    insertSymbol(symbolInfo(++count, currSymbol, "var", prevSymbol, idScope));
                    outputFile << "["
                               << "id " << to_string(count) << "] ";
                }
                else
                {
                    errors.push_back(make_pair("\nRe-declarion of identifier '" + varName + "' at line: ", lineNum));
                }
            }
            else if (currSymbol == "(" && prevToken.substr(0, 2) == "id")
            {
                updateSymbolInfo("idType", prevSymbol, idScope, "func");
                updateSymbolInfo("idScope", prevSymbol, idScope, "global");
                outputFile << "[" << currSymbol << "] ";
            }
            else if (currToken.substr(0, 3) == "num" && prevSymbol == "=")
            {
                if (lookupSymbolTable(varName, "var", idScope) == -1)
                {

                    errors.push_back(make_pair("\nUndeclared identifier '" + varName + "' at line: ", lineNum));
                }
                else
                {
                    updateSymbolInfo("value", varName, idScope, currSymbol);
                    outputFile << "[" << currSymbol << "] ";
                }
            }
            else if (!currToken.empty())
            {
                if (currToken.substr(0, 2) == "id")
                {
                    if (lookupSymbolTable(currSymbol, idScope) == -1)
                    {
                        idSl = lookupSymbolTable(currSymbol, "global") + 1;
                    }
                    else
                    {
                        idSl = lookupSymbolTable(currSymbol, idScope) + 1;
                    }

                    outputFile << "["
                               << "id " << to_string(idSl) << "] ";
                }
                else
                    outputFile << "[" << currSymbol << "] ";
            }
            prevToken = currToken;
        }
        outputFile << "\n";
        lineNum++;
    }
    inputFile.close();
    outputFile.close();

    while (!parStack.empty())
    {
        errors.push_back(make_pair("\nMisplaced '" + parStack.top().first + "' at line: ", parStack.top().second));
        parStack.pop();
    }
}

int main()
{
    bool x = filterSource("input.c");
    if (x == true)
    {
        tokenizeSource("output1.txt");
        generateSymbolTable("output2.txt");

        cout << "Source Code @ ";
        printFile("input.c");
        cout << "\n\nFiltered Source @ ";
        printFile("output1.txt");
        cout << "\n\nTokenized Source @ ";
        printFile("output2.txt");
        cout << "\n\nModified Token Stream @ ";
        printFile("output3.txt");

        displaySymbolTable();

        std::sort(errors.begin(), errors.end(), comparator);
        if (errors.empty())
            cout << "\nNo errors detected!";
        else
        {
            cout << "\nDetected Errors:\n";
            std::cout << "======================= " << endl;
            for (int i = 0; i < errors.size(); i++)
                cout << errors[i].first << errors[i].second;
        }
    }
    else
    {
        std::cout << "Source code file couldn't be opened.";
    }

    return 0;
}