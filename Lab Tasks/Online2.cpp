/*
ID: 190104140
Group : C2
Online : 2
Date: 20/12/22
Task: Counting how many numeric constants are there in a source code of C
*/

#include <bits/stdc++.h>

using namespace std;

int main()
{
    string str;
    int cnt = 0, dpCount = 0;

    fstream inputFile("input.txt", ios::in);
    fstream outputFile("output_online2.txt", ios::out);

    while(getline(inputFile, str, ' '))
    {
        bool flag =  true;
        for(int i = 0; i< str.size(); i++)
        {
            if((!isdigit(str[i]) && str[i] != '.') || str[str.size() -1] == '.')
            {
                flag = false;
                break;
            }

        }

        dpCount = count(str.begin(), str.end(), '.');

        if(flag == true && dpCount <= 1)
        {
            cnt ++;
        }
    }

    outputFile << "Number of numeric constants: " << cnt << endl;
    cout << "Number of numeric constants: " << cnt << endl;

}
