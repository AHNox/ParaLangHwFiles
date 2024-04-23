#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

void Print(string line, unordered_map<string, string> variables)
{
    line = line.substr(6);    // Shaves down the line to not include "print(" and ")"
    line.erase(line.find(')'));
    stringstream input(line); // Declaration of stringstream to use passed lin as input
    string temp;              // Stand-in variable to help deal with inputs
    while (input.good())      // While loop runs until the stringstream is empty
    {
        getline(input, temp, ','); // Gets input up until the next comma
        if (temp[0] == '"')        // Prints everything surrounded by quotation marks as it is passed in
        {
            cout << temp.substr(1, temp.size() - 2);
        }
        else // Anything that is not in quotation marks is a variable for these testcases so we access the map and print its value
        {
            cout << variables[temp.substr(1, temp.size() - 1)];
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
    // Includes all operations that are present in the testcases

    if (oprt == '+')
    {
        return a + b;
    }
    else if (oprt == '-')
    {
        return a - b;
    }
    else if (oprt == '*')
    {
        return a * b;
    }
    else if (oprt == '/')
    {
        return a / b;
    }
    else
        return 0;
}

int EvalExpression(string line, unordered_map<string, string> variables)
{
    cout << "Expression is: "<< line << endl;
    istringstream input(line); // Stringstream is used to utilize strings as inputs (rather than depending on console inputs)
    string first;              // Variable for the first number in the expression
    char oprt;                 // Variable for operators
    string operand;            // Variable for operands (numbers)
    int temp;                  // Stand-in varible for stack usage
    int temp2;                 // Another stand-in
    stack<char> oprtStack;     // Stack to keep track of operators. Used to establish precedence.
    stack<int> oprndStack;     // Stack to keep track of operands. Used to establish precedence.
    stack<int> helper1;        // Stack for flipping operand stack
    stack<char> helper2;       // Stack for flipping operator stack

    input >> first;
    if (isInt(first)) // Checks if a variable is passed. If a variable is detected, then the map is accessed to find its value.
        temp = stoi(first);
    else
        temp = stoi(variables[first]);

    oprndStack.push(temp); // Pushes the first integer into the stack

    // EXPRESSION EVALUATION BELOW

    while (input >> oprt >> operand)
    {
        if (!isInt(operand)) // Accesses value of variable if one is passed
            operand = variables[operand];

        oprtStack.push(oprt); // Pushes next operator in the line to the operator stack

        if (oprtStack.top() == '*' || oprtStack.top() == '/') // If multiplication or division are detected, they are applied immediately because they have the highest precedence in these test cases
        {
            temp = oprndStack.top();
            oprndStack.pop();
            cout << temp << " " << oprtStack.top() << " " << operand << endl;
            oprndStack.push(operations(temp, stoi(operand), oprtStack.top()));
            oprtStack.pop();
        }
        else
        {
            oprndStack.push(stoi(operand)); // If there's no multiplying or dividing, then we simply push the operand into the stack
        }
    }

    while (oprndStack.size() != 0)
    {
        helper1.push(oprndStack.top());
        oprndStack.pop();
    }
    while (oprtStack.size() != 0)
    {
        helper2.push(oprtStack.top());
        oprtStack.pop();
    }

    oprndStack = helper1;
    oprtStack = helper2;

    while (oprndStack.size() != 1) // After all the multiplication and division has been done, we work out way through the stack and do out addition and subtraction
    {
        temp = oprndStack.top();
        oprndStack.pop();
        temp2 = oprndStack.top();
        oprndStack.pop();
        oprndStack.push(operations(temp, temp2, oprtStack.top()));
        cout << temp << " " << oprtStack.top() << " " << temp2 << endl;
        oprtStack.pop();
    }

    return oprndStack.top(); // The only value left in the stack by this point is the final value, so we return it
}

int LineIndentLevel(string line, int baseIden)
{
    int indenLevel = baseIden, indenIndex = 0;
    while (line[indenIndex] == ' ')
    {
        indenIndex++;
        indenLevel++;
    }
    return indenLevel;
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

void IfElseStatements(vector<string> inputLines, unordered_map<string, string> &variables, int indentLvl, int arrayIndex)
{
    //cout << "Running..." << endl;
    string line = inputLines[arrayIndex];
    string logicalExp = line.substr(3, line.size() - 4);
    string oprt;

    if (logicalExp.find("==") != string::npos)
        oprt = "==";
    else if (logicalExp.find(">=") != string::npos)
        oprt = ">=";
    else if (logicalExp.find("<=") != string::npos)
        oprt = "<=";
    else if (logicalExp.find("!=") != string::npos)
        oprt = "!=";
    else if (logicalExp.find("<") != string::npos)
        oprt = "<";
    else if (logicalExp.find(">") != string::npos)
        oprt = ">";
    else
        return;

    //cout << "'" << logicalExp << "'" << endl;
    string left = logicalExp.substr(0, logicalExp.find(oprt));
    //cout << left << endl;
    string right = logicalExp.substr(logicalExp.find(oprt)+ 2);
    //cout << right << endl;

    arrayIndex += 1;
    string statementLine = inputLines[arrayIndex];
    int baseIndex = indentLvl, stmtIndenLvl = 0;
    stmtIndenLvl = LineIndentLevel(line, baseIndex);
    // cout << "statement level indent: " << stmtIndenLvl << endl;

    vector<string> ifStmts;

    vector<string> elseStmts;

    while (LineIndentLevel(inputLines[arrayIndex], 0) == 4)
    {
        // cout << "this is inside if statement: " << inputLines[arrayIndex] << endl;
        ifStmts.push_back(inputLines[arrayIndex]);
        arrayIndex++;
    }
    if (inputLines[arrayIndex].substr(0, 4) == "else")
    {
        arrayIndex++;
        while (LineIndentLevel(inputLines[arrayIndex], 0) == 4)
        {
            // cout << "this is inside else statement: " << inputLines[arrayIndex] << endl;
            elseStmts.push_back(inputLines[arrayIndex]);
            arrayIndex++;
        }
    }
    if (LogicOps(EvalExpression(left, variables), EvalExpression(right, variables), oprt))
    {
        for (int indexer = 0; indexer < ifStmts.size(); indexer++)
        {
            line = ifStmts[indexer];

            if (line.find(" = ") != string::npos)
            {
                int mid = line.find('=');
                string name = line.substr(0, mid - 1); // Determines what the name of the variable is
                string exp = line.substr(mid + 1);     // Determines what the variable will be assigned

                name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
                variables[name] = to_string(EvalExpression(exp, variables)); // Evaluates the right side of the variable declaration
            }
        }
    }
    else
    {
        for (int indexer = 0; indexer < elseStmts.size(); indexer++)
        {
            line = elseStmts[indexer];
            // run elseStmts;
            if (line.find(" = ") != string::npos)
            {
                
                int mid = line.find('=');
                string name = line.substr(0, mid - 1); // Determines what the name of the variable is
                string exp = line.substr(mid + 1);     // Determines what the variable will be assigned

                name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
                variables[name] = to_string(EvalExpression(exp, variables)); // Evaluates the right side of the variable declaration
            }
        }
    }
}

int main(int argc, char *argv[])
{
    vector<string> inputLines;
    // Check if filename is provided
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename.py>" << endl;
        return 1;
    }

    // Open the Python script file
    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }
    else
    {
        char ch;
        string tempString = "";
        bool lineIsComment = false;
        int commentLength = 0;
        while (file.get(ch))
        {
            // Checks if the current value is a newline
            if (ch == '\n' || ch == '\r')
            { // If it is then we add the current temp string into the array
                bool isSpace = true;
                int endPosition = tempString.length() - commentLength;
                for (int index = 0; index < tempString.length(); index++)
                {
                    if (tempString[index] == '#' && isSpace == true)
                    {
                        endPosition -= index;
                    }
                    if (tempString[index] != ' ')
                    {
                        isSpace = false;
                    }
                }
                if (tempString.length() > 0 && tempString.length() > commentLength && endPosition != 0)
                {
                    inputLines.push_back(tempString.substr(0, endPosition));
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

            inputLines.push_back(tempString.substr(0, endPosition));
            tempString = "";
        }
        tempString = "";
        lineIsComment = false;
        commentLength = 0;
    }

    unordered_map<string, string> variables; // Map used to store variable's names and assignments
    bool isIfInsideIfElse = false;

    for (int arrayIndex = 0; arrayIndex < inputLines.size(); arrayIndex++)
    {
        // Read and print the content of the file
        string line = inputLines[arrayIndex];

        int indenLevel = 0, indenIndex = 0;

        indenLevel = LineIndentLevel(line, indenIndex);

        if (line.substr(0, 6) == "print(") // If a print statement is detected, it runs our print function
        {
            Print(line, variables);
        }

        else if (line.substr(0,3) == "def")
        {
            cout << "Function definition detected" << endl;
        }

        else if (line.substr(0, 2) == "if") // Work in progress
        {
            IfElseStatements(inputLines, variables, indenLevel, arrayIndex);
        }

        else if (line.find(" = ") != string::npos && indenLevel == 0) // Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0, mid - 1); // Determines what the name of the variable is
            string exp = line.substr(mid + 1);     // Determines what the variable will be assigned

            variables[name] = to_string(EvalExpression(exp, variables)); // Evaluates the right side of the variable declaration
        }
        else;
        
    }
    return 0;
}