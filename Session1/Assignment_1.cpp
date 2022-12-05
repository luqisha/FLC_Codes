#include <bits/stdc++.h>

using namespace std;

int main()
{
    char c;
    bool inQuote = false, flag = false;

    fstream inputFile("input.c", ios::in);
    fstream outputFile("output.txt", ios::out);

    if (inputFile.is_open())
    {
        cout << "\nInput: input.c" << endl;
        cout << "================" << endl;
        while (inputFile.get(c))
        {
            if (inputFile.eof())
            {
                break;
            }
            else if (c == '"')
            {
                inQuote = !inQuote;
            }

            cout << c;

            if (c == '\n' || c == '\t' || c == ' ' && !inQuote)
            {
                if (inputFile.peek() == '\n' || inputFile.peek() == '\t' || inputFile.peek() == ' ')
                {
                    continue;
                }
                else if (!flag)
                {
                    outputFile.put(' ');
                    flag = true;
                }
            }
            else if (c == '/')
            {
                if (inputFile.peek() == '/')
                {
                    while (inputFile.get() != '\n')
                    {
                        continue;
                    }
                }
                else if (inputFile.peek() == '*')
                {
                    while (!(inputFile.get() == '*' && inputFile.peek() == '/'))
                    {
                        continue;
                    }
                }
            }
            else
            {
                outputFile.put(c);
                flag = false;
            }
        }

        inputFile.close();
        outputFile.close();

        outputFile.open("output.txt", ios::in);
        cout << "\n\nOutput: output.txt" << endl;
        cout << "======================= " << endl;
        while (outputFile.get(c))
        {
            if (outputFile.eof())
                break;
            cout << c;
        }
        cout << endl;
    }
    else
    {
        cout << "input.c couldn't be opened.";
    }
    
    return 0;
}