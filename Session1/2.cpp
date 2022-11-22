
#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream inputFile ("input.txt");
    fstream outputFile ("output.txt", ios::out);

    string test, out;

    int lineNo = 0;

    while(getline(inputFile, test))
    {
        outputFile << lineNo << ". ";
        lineNo++;
        outputFile << test << endl;
    }

    cout << "Added Line Numbers to input file." << endl;
}



