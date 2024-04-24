#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

struct boundaries
{
    string inputs;
    int start;
    int end;
};

void Print(string line, unordered_map<string, string> variables)
{
    line = line.substr(6); // Shaves down the line to not include "print(" and ")"
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

int FunctionCall(string, vector<string>, unordered_map<string, string>, unordered_map<string, boundaries>, string, int);

int EvalExpression(vector<string> inputLines, string line, unordered_map<string, string> variables, unordered_map<string, boundaries> functions)
{
    // cout << "Expression is: "<< line << endl;
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
    string function;

    input >> first;
    if (isInt(first)) // Checks if a variable is passed. If a variable is detected, then the map is accessed to find its value.
        temp = stoi(first);
    else
    {
        if (variables.find(first) != variables.end())
            temp = stoi(variables[first]);
        else if (functions.find(first.substr(0, first.find('('))) != functions.end())
        {
            // cout << first << endl;
            function = first.substr(0, first.find('('));
            temp = FunctionCall(first, inputLines, variables, functions, function, functions[function].start);
        }
    }

    oprndStack.push(temp); // Pushes the first integer into the stack

    // EXPRESSION EVALUATION BELOW

    while (input >> oprt >> operand)
    {
        if (!isInt(operand)) // Accesses value of variable if one is passed
        {
            if (variables.find(operand) != variables.end())
                operand = variables[operand];
            else if (functions.find(operand.substr(0, operand.find('('))) != functions.end())
            {
                function = operand.substr(0, operand.find('('));
                operand = FunctionCall(operand, inputLines, variables, functions, function, functions[function].start);
            }
        }

        oprtStack.push(oprt); // Pushes next operator in the line to the operator stack

        if (oprtStack.top() == '*' || oprtStack.top() == '/') // If multiplication or division are detected, they are applied immediately because they have the highest precedence in these test cases
        {
            temp = oprndStack.top();
            oprndStack.pop();
            // cout << temp << " " << oprtStack.top() << " " << operand << endl;
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
        // cout << temp << " " << oprtStack.top() << " " << temp2 << endl;
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

void IfElseStatements(vector<string> inputLines, unordered_map<string, string> &variables, unordered_map<string, boundaries> functions, int indentLvl, int arrayIndex)
{
    cout << "Running...'" << inputLines[arrayIndex] << "'" << endl;
    string line = inputLines[arrayIndex];
    string logicalExp = line.substr(indentLvl + 3, line.size() - 4);
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

    string statementLine = inputLines[arrayIndex];
    int baseIndex = indentLvl;

    // cout << "Line: '" << inputLines[arrayIndex] << "'" << indentLvl << " " << baseIndex << endl;

    cout << "'" << logicalExp << "'" << endl;
    string left = logicalExp.substr(0, logicalExp.find(oprt) - 1);
    cout << "'" << left << "'" << endl;
    string right = logicalExp.substr(logicalExp.find(oprt) + 2);
    cout << "'" << right << "'" << endl;

    arrayIndex += 1;

    // cout << "statement level indent: " << stmtIndenLvl << endl;

    vector<string> ifStmts;
    vector<string> elseStmts;

    while (LineIndentLevel(inputLines[arrayIndex], 0) >= indentLvl + 4)
    {
        // cout << "this is inside if statement: " << inputLines[arrayIndex] << endl;
        ifStmts.push_back(inputLines[arrayIndex]);
        arrayIndex++;
    }
    if (inputLines[arrayIndex].substr(indentLvl, indentLvl + 4) == "else")
    {
        arrayIndex++;
        while (LineIndentLevel(inputLines[arrayIndex], 0) >= indentLvl + 4 && !inputLines[arrayIndex].empty())
        {
            // cout << "this is inside else statement: " << inputLines[arrayIndex] << endl;
            elseStmts.push_back(inputLines[arrayIndex]);
            arrayIndex++;
        }
    }

    cout << EvalExpression(inputLines, left, variables, functions) << " " << EvalExpression(inputLines, right, variables, functions) << " " << oprt << endl;

    if (LogicOps(EvalExpression(inputLines, left, variables, functions), EvalExpression(inputLines, right, variables, functions), oprt))
    {
        for (int indexer = 0; indexer < ifStmts.size(); indexer++)
        {
            // cout << "running if line: " << ifStmts[indexer] << endl;
            line = ifStmts[indexer];

            if (line.find(" = ") != string::npos)
            {
                int mid = line.find('=');
                string name = line.substr(0, mid - 1); // Determines what the name of the variable is
                string exp = line.substr(mid + 1);     // Determines what the variable will be assigned

                name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
                variables[name] = to_string(EvalExpression(inputLines, exp, variables, functions)); // Evaluates the right side of the variable declaration
            }

            else if (line.substr(indentLvl + 4, indentLvl + 2) == "if") // Work in progress
            {
                IfElseStatements(ifStmts, variables, functions, indentLvl + 4, indexer);
            }
        }
    }
    else
    {
        for (int indexer = 0; indexer < elseStmts.size(); indexer++)
        {
            // cout << "running else line: " << elseStmts[indexer] << endl;
            line = elseStmts[indexer];
            // run elseStmts;

            if (line.find(" = ") != string::npos)
            {

                int mid = line.find('=');
                string name = line.substr(0, mid - 1); // Determines what the name of the variable is
                string exp = line.substr(mid + 1);     // Determines what the variable will be assigned

                name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
                variables[name] = to_string(EvalExpression(inputLines, exp, variables, functions)); // Evaluates the right side of the variable declaration
            }

            else if (line.substr(indentLvl + 4, indentLvl + 2) == "if") // Work in progress
            {
                IfElseStatements(elseStmts, variables, functions, indentLvl + 4, indexer);
            }
        }
    }
}

void FunctionDeclaration(vector<string> inputLines, unordered_map<string, boundaries> &functions, int indentLvl, int arrayIndex)
{
    // cout << arrayIndex << endl;
    string line = inputLines[arrayIndex];
    int varStart = line.find('(');
    int lowerBound = arrayIndex;
    string defName = line.substr(4, varStart - 4);
    string inputs = line.substr(varStart + 1, line.find(')') - varStart - 1);

    arrayIndex++;
    while (arrayIndex < inputLines.size() && LineIndentLevel(inputLines[arrayIndex], indentLvl) > indentLvl)
        arrayIndex++;
    // cout << line << endl;
    // cout << defName << endl;
    // cout << inputs << endl;
    // cout << arrayIndex;
    functions[defName].start = lowerBound;
    functions[defName].end = arrayIndex;
    functions[defName].inputs = inputs;
}

int FunctionCall(string introLine, vector<string> inputLines, unordered_map<string, string> variables, unordered_map<string, boundaries> functions, string function, int arrayIndex)
{
    unordered_map<string, string> funcVars; // Local Variable map
    // Variable assignment for passed values
    stringstream input(functions[function].inputs);
    stringstream passed(introLine.substr(introLine.find('(') + 1, introLine.find(')') - (introLine.find('(') + 1)));
    string temp;
    string temp2;
    // cout << temp << endl << temp2;

    while (input.good() && passed.good())
    {
        getline(input, temp, ',');
        getline(passed, temp2, ',');
        if (!isInt(temp2))
            temp2 = variables[temp2];
        funcVars[temp] = temp2;
    }

    while (arrayIndex <= functions[function].end)
    {
        string line = inputLines[arrayIndex].substr(5);

        int indenLevel = 0, indenIndex = 0;

        indenLevel = LineIndentLevel(line, indenIndex);

        if (line.substr(0, 6) == "print(")
        {
            Print(line, funcVars);
        }

        else if (line.substr(0, 2) == "if")
        {
            IfElseStatements(inputLines, funcVars, functions, indenLevel, arrayIndex);
        }

        else if (line.find(" = ") != string::npos && indenLevel == 0) // Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0, mid - 1); // Determines what the name of the variable is
            string exp = line.substr(mid + 1);     // Determines what the variable will be assigned
            cout << "Problem?";
            funcVars[name] = to_string(EvalExpression(inputLines, exp, funcVars, functions)); // Evaluates the right side of the variable declaration
        }
        else if (line.substr(0, 6) == "return")
        {
            cout << "Return detected" << endl;
            return 777;
        }
        else
            ;

        arrayIndex++;
    }
    return -1;
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
    unordered_map<string, boundaries> functions;
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

        else if (line.substr(0, 3) == "def")
        {
            cout << "Function definition detected" << endl;
            FunctionDeclaration(inputLines, functions, indenLevel, arrayIndex);
        }

        else if (line.substr(0, 2) == "if") // Work in progress
        {
            IfElseStatements(inputLines, variables, functions, indenLevel, arrayIndex);
        }

        else if (line.find(" = ") != string::npos && indenLevel == 0) // Checks to see if "=" is present in input line (will have to be more specific, but this works for now)
        {
            // If a variable declaration is detected, then the variable is added to our variable map
            int mid = line.find('=');
            string name = line.substr(0, mid - 1);                                              // Determines what the name of the variable is
            string exp = line.substr(mid + 1);                                                  // Determines what the variable will be assigned
            variables[name] = to_string(EvalExpression(inputLines, exp, variables, functions)); // Evaluates the right side of the variable declaration
        }
    }
    return 0;
}