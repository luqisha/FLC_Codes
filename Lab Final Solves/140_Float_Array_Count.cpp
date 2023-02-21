#include <bits/stdc++.h>
using namespace std;

int main()
{
    fstream inputFile("input.txt", ios::in);
    string line;
    int cnt = 0;
    while (getline(inputFile, line))
    {
        int loc = line.find("float");
        
        if (loc != -1)
        {
            int i = loc, open = 0, close = 0;
            while (line[i] != '\0')
            {
                if (line[i] == '[') open++;
                else if (line[i] == ']') close++;
                if ((line[i] == ',' || line[i] == ';') && open == close && open !=0 && close != 0 )
                {
                    cnt ++;
                    open = 0;
                    close = 0;
                }
                i++;
            }

        }
    }
    cout << "No. of float arrays declared: " << cnt << endl;
}