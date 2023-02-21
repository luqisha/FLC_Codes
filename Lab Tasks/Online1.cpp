/*
ID : 190104140
Group : C2
Online : 01
Date : 6/12/2022
Task: Count number of header files included in a C source code
*/

#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream inputFile ("input.c", ios::in);
    fstream outputFile ("output_online1.txt", ios::out);

    string test;

    int c = 0;
    while(getline(inputFile, test))
    {
        if(test.find("#include") != string::npos)
        {
            c++;
        }
    }

    outputFile << "Number of header files : " << c;
    cout << "Number of header files : " << c << endl;

    inputFile.close();
    outputFile.close();


    return 0;
}