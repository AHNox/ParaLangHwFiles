using namespace std;
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char *argv[])
{
    vector<string> tokenStringVector;
    if (argc < 2)
    {
        cout << "Error: No python file provided" << endl;
    }
    else
    {
        ifstream file(argv[1]); // Open the file for reading
        char ch;

        if (file.is_open())
        {
            string tempString = "";
            bool lineIsComment = false;
            int commentLength = 0;
            while (file.get(ch))
            {
                // Checks if the current value is a newline
                if (ch == '\n' || ch == '\r')
                { // If it is then we add the current temp string into the array
                    if (tempString.length() > 0 && tempString.length() > commentLength)
                    {
                        int endPosition = tempString.length() - commentLength;

                        tokenStringVector.push_back(tempString.substr(0, endPosition));
                        tempString = "";
                    }
                    tempString = "";
                    lineIsComment = false;
                    commentLength = 0;
                }
                else
                { // If it isnt then we add the current char into the temp string
                    // Checks if the character is a # which is the comment
                    if (ch == '#')
                    {
                        lineIsComment = true;
                    }
                    // If this line is a part of the commnet, it adds 1 to the length
                    if (lineIsComment == true)
                    {
                        commentLength += 1;
                    }
                    tempString += ch;
                }
            }
            file.close(); // Close the file when done
            if (tempString.length() > 0 && tempString.length() > commentLength)
            {
                int endPosition = tempString.length() - commentLength;

                tokenStringVector.push_back(tempString.substr(0, endPosition));
                tempString = "";
            }
            tempString = "";
            lineIsComment = false;
            commentLength = 0;
        }
        else
        {
            cerr << "Unable to open file!" << endl;
        }
    }

    for (int arrayIndex = 0; arrayIndex < tokenStringVector.size(); arrayIndex++)
    {
        cout << tokenStringVector[arrayIndex] << endl;
    }

    return 0;
}