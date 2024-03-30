/*

This is the main file that we would submit and place into the server

*/

using namespace std;
#include <iostream>
#include <fstream>

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

        if (file.is_open())
        { // Check if the file is open successfully
            // Read and output each line of the file
            while (file.get(ch))
            {
                int charInt = ch;
                cout << charInt << " '" << ch << "' " << endl;
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