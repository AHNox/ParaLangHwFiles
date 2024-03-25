using namespace std;
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Error: No python file provided" << endl;
    }
    else
    {
        ifstream file(argv[1]); // Open the file for reading
        char ch;
        vector<string> splitUpLines;

        if (file.is_open())
        {
            string tempString = "";
            while (file.get(ch))
            {
                int charInt = ch;
                if (charInt == 10)
                {
                    cout << tempString << endl;
                        splitUpLines.push_back(tempString);
                    tempString = "";
                }
                else
                {
                    tempString += ch;
                }
            }
            file.close(); // Close the file when done
        }
        else
        {
            cerr << "Unable to open file!" << endl;
        }
    }

    return 0;
}