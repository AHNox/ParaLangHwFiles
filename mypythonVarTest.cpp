#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>

using namespace std;

void Print(string line, unordered_map <string, string> variables)
{
    line = line.substr(6);                      // Shaves down the line to not include "print(" and ")"
    line.erase(line.find(')'));
    stringstream input(line);                   // Declaration of stringstream to use passed lin as input
    string temp;                                // Stand-in variable to help deal with inputs
    while(input.good())                         // While loop runs until the stringstream is empty
    {
        getline(input, temp, ',');              // Gets input up until the next comma
        if (temp[0] == '"')                     // Prints everything surrounded by quotation marks as it is passed in
        {
            cout << temp.substr(1,temp.size() - 2);
        }
        else                                    // Anything that is not in quotation marks is a variable for these testcases so we access the map and print its value
        {
            cout << variables[temp.substr(1,temp.size() -1)];
        }
        cout << " ";
    }
    cout << endl;
}

bool isInt(string line)
{
    // Recursively checks every character in passed string to determine if it's a variable or an integer
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
    //Includes all operations that are present in the testcases

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

bool LogicOps(int a, int b, string oprt)
{
    if (oprt == "==")
        return a == b;
    else if (oprt == ">=")
        return a >= b;
    else if (oprt == "<=")
        return a <= b;
    else if (oprt == "!=")
        return a != b;
    else if (oprt == "<")
        return a < b;
    else if (oprt == ">")
        return a > b;
    else
        return false;
}

int EvalExpression(string line, unordered_map <string, string> variables)
{
    istringstream input(line);  // Stringstream is used to utilize strings as inputs (rather than depending on console inputs)
    string first;               // Variable for the first number in the expression
    char oprt;                  // Variable for operators
    string operand;             // Variable for operands (numbers)
    int temp;                   // Stand-in varible for stack usage
    int temp2;                  // Another stand-in
    stack <char> oprtStack;     // Stack to keep track of operators. Used to establish precedence.
    stack <int> oprndStack;     // Stack to keep track of operands. Used to establish precedence.

    input >> first;
    if (isInt(first))           // Checks if a variable is passed. If a variable is detected, then the map is accessed to find its value.
        temp = stoi(first);
    else
        temp = stoi(variables[first]);

    oprndStack.push(temp);      // Pushes the first integer into the stack

    //EXPRESSION EVALUATION BELOW

    while (input >> oprt >> operand)
    {
        if (!isInt(operand))                // Accesses value of variable if one is passed
            operand = variables[operand];

        oprtStack.push(oprt);               // Pushes next operator in the line to the operator stack

        if (oprtStack.top() == '*' || oprtStack.top() == '/')   // If multiplication or division are detected, they are applied immediately because they have the highest precedence in these test cases
        {
            temp = oprndStack.top();
            oprndStack.pop();
            //cout << temp << " " << oprtStack.top() << " " << operand << endl;
            oprndStack.push(operations(temp, stoi(operand), oprtStack.top()));
            oprtStack.pop();
        }
        else 
        {
            oprndStack.push(stoi(operand)); // If there's no multiplying or dividing, then we simply push the operand into the stack
        }
    }

    while (oprndStack.size() != 1)          // After all the multiplication and division has been done, we work out way through the stack and do out addition and subtraction
    {
            temp = oprndStack.top();
            oprndStack.pop();
            temp2 = oprndStack.top();
            oprndStack.pop();
            oprndStack.push(operations(temp2, temp, oprtStack.top()));
            oprtStack.pop();
    }

    return oprndStack.top();                // The only value left in the stack by this point is the final value, so we return it
}

bool LogicEval(string line, unordered_map <string, string> variables)
{
    line = line.substr(3);
    line.erase(line.find(":"));
    string oprt;

    if (line.find("==")!= string::npos)
        oprt = "==";
    else if (line.find(">=")!= string::npos)
        oprt = ">=";
    else if (line.find("<=")!= string::npos)
        oprt = "<=";
    else if (line.find("!=")!= string::npos)
        oprt = "!=";
    else if (line.find("<")!= string::npos)
        oprt = "!=";
    else if (line.find(">")!= string::npos)
        oprt = ">";
    else
        return false;
    string left = line.substr(0, line.find(oprt));
    string right = line.substr(line.find(oprt) + 3);

    return LogicOps(EvalExpression(left, variables), EvalExpression(right, variables), oprt);
    
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

    unordered_map<string, string> variables; // Map used to store variable's names and assignments

    // Read and print the content of the file
    string line;
    while (getline(file, line)) {
        if (line.substr(0,6) == "print(") //If a print statement is detected, it runs our print function
        {
            Print(line, variables);
        }

        else if (line.substr(0,2) == "if") 
        {
            
            if(LogicEval(line, variables))
            {
                //This is where we put what happens if the statement is true
            }
            else
            {
                //This is where we decide what to do in case of false
            }
        }
        
        else if (line.find(" = ") != string::npos) //Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0,mid - 1); // Determines what the name of the variable is
            string exp = line.substr(mid + 1); // Determines what the variable will be assigned

            variables[name] = to_string(EvalExpression(exp, variables)); // Evaluates the right side of the variable declaration
        }

        else
        {
            // 
        }
    }

    // Close the file
    file.close();

    return 0;
}