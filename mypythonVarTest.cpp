#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

void print(string line)// Work in progress
{
    line = line.substr(6, line.size() - 7);
    //stringstream input(line);
    //string temp;

    cout << line << endl;
}

int main(int argc, char* argv[]) {
    // Check if filename is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename.py>" << endl;
        return 1;
    }

    // Open the Python script file
    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    unordered_map<string, string> variables; //used to store variables

    // Read and print the content of the file
    string line;
    while (getline(file, line)) {

        cout << line << endl;

        if (line.substr(0,6) == "print(") //If a print statement is detected, it runs our print function
        {
            print(line);
        }

        
        else if (line.find('=') != string::npos) //Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0,mid - 1);
            string exp = line.substr(mid + 1);

            variables[name] = exp;
            cout << variables[name] << endl;
        }

        else // Work in progress
        {
            //cout << "Implementation not finished for line" << endl;
        }
    }

    // Close the file
    file.close();

    return 0;
}
