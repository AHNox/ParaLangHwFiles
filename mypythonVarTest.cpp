#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>

using namespace std;

void Print(string line, unordered_map <string, string> variables)// Work in progress
{
    line = line.substr(6, line.size() - 7);
    stringstream input(line);
    string temp;
    while(input.good())
    {
        getline(input, temp, ',');
        if (temp[0] == '"')
        {
            cout << temp.substr(1,temp.size() - 2);
        }
        else
        {
            cout << variables[temp.substr(1,temp.size() -1)];
        }
        cout << " ";
    }
    cout << endl;
}

bool isInt(string line)
{
    if (line.size() == 0)
        return true;
    else
    {
        if (isdigit(line[0]))
            return isInt(line.substr(1));
        else
            return false;
    }
}

int operations(int a, int b, char oprt)
{
    if (oprt == '+') {
        return a + b ;
    } 
    else if (oprt == '-') {
        return a - b;
    } 
    else if (oprt == '*') {
        return a * b;
    } 
    else if (oprt == '/') {
        return a / b;
    }
    else 
        return 0;
}

int EvalExpression(string line, unordered_map <string, string> variables)
{
    istringstream input(line);
    string first;
    char oprt;
    string operand;
    int temp;
    int temp2;
    stack <char> oprtStack;
    stack <int> oprndStack;

    input >> first;
    if (isInt(first))
        temp = stoi(first);
    else
        temp = stoi(variables[first]);

    oprndStack.push(temp);

    while (input >> oprt >> operand)
    {
        if (!isInt(operand))
            operand = variables[operand];

        oprtStack.push(oprt);

        if (oprtStack.top() == '*' || oprtStack.top() == '/')
        {
            temp = oprndStack.top();
            oprndStack.pop();
            //cout << temp << " " << oprtStack.top() << " " << operand << endl;
            oprndStack.push(operations(temp, stoi(operand), oprtStack.top()));
            oprtStack.pop();
        }
        else 
        {
            oprndStack.push(stoi(operand));
        }
    }

    while (oprndStack.size() != 1)
    {
            temp = oprndStack.top();
            oprndStack.pop();
            temp2 = oprndStack.top();
            oprndStack.pop();
            oprndStack.push(operations(temp2, temp, oprtStack.top()));
            oprtStack.pop();
    }

    return oprndStack.top();
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

        //cout << line << endl;

        if (line.substr(0,6) == "print(") //If a print statement is detected, it runs our print function
        {
            Print(line, variables);
        }

        
        else if (line.find(" = ") != string::npos) //Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0,mid - 1);
            string exp = line.substr(mid + 1);
            //cout << exp << endl;

            variables[name] = to_string(EvalExpression(exp, variables));
            //cout << variables[name] << endl;
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
