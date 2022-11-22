#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream inputFile ("input.txt");
    fstream outputFile ("output.txt", ios::out);

    string test, out;

    while(getline(inputFile, test))
    {
        if(test.find("#include") != string::npos)
        {
            int s = test.find("<");
            if(s == string::npos)
                s = test.find("\"");

            int e = test.find(".h");
            s++;
            e+=2;

            out = test.substr(s, e-s);
            cout << out << endl;

            outputFile << out << endl;
        }
    }

}
