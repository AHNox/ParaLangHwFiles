using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <unordered_map>

enum class TokenType
{
    // Single-character tokens
    LEFT_PAREN,  // (          (0)
    RIGHT_PAREN, // )          (1)
    LEFT_BRACE,  // {          (2)
    RIGHT_BRACE, // }          (3)
    COMMA,       // ,          (4)
    DOT,         // .          (5)
    MINUS,       // -          (6)
    PLUS,        // +          (7)
    SEMICOLON,   // ;          (8)
    SLASH,       // /          (9)
    STAR,        // *          (10)
    DOUB_QUOTE,  // "          (11)

    // One or two character tokens
    BANG,          // !          (12)
    BANG_EQUAL,    // !=         (13)
    EQUAL,         // =          (14)
    EQUAL_EQUAL,   // ==         (15)
    GREATER,       // >          (16)
    GREATER_EQUAL, // >=         (17)
    LESS,          // <          (18)
    LESS_EQUAL,    // <=         (19)

    // Literals
    IDENTIFIER, // identifier (20)
    STRING,     // string     (21)
    NUMBER,     // number     (22)

    // Keywords
    AND,    // and        (23)
    BREAK,  // break      (24)
    DEF,    // def        (25)
    ELIF,   // elif       (26)
    ELSE,   // else       (27)
    FALSE,  // False      (28)
    GLOBAL, // global     (29)
    IF,     // if         (30)
    IN,     // in         (31)
    IS,     // is         (32)
    LAMBDA, // lambda     (33)
    NONE,   // None       (34)
    OR,     // or         (35)
    RETURN, // return     (36)
    TRUE,   // True       (37)
    TRY,    // try        (38)
    PRINT,  // print      (39)
    BLANK
};

enum charType
{
    blank,
    digit,
    letter,
    irregChar,
    whitespace,
};

unordered_map<string, TokenType> tokenMap = {
    {"(", TokenType::LEFT_PAREN},
    {")", TokenType::RIGHT_PAREN},
    {"{", TokenType::LEFT_BRACE},
    {"}", TokenType::RIGHT_BRACE},
    {",", TokenType::COMMA},
    {".", TokenType::DOT},
    {"-", TokenType::MINUS},
    {"+", TokenType::PLUS},
    {";", TokenType::SEMICOLON},
    {"/", TokenType::SLASH},
    {"*", TokenType::STAR},
    {"\"", TokenType::DOUB_QUOTE},
    {"!", TokenType::BANG},
    {"!=", TokenType::BANG_EQUAL},
    {"=", TokenType::EQUAL},
    {"==", TokenType::EQUAL_EQUAL},
    {">", TokenType::GREATER},
    {">=", TokenType::GREATER_EQUAL},
    {"<", TokenType::LESS},
    {"<=", TokenType::LESS_EQUAL},
    {"and", TokenType::AND},
    {"break", TokenType::BREAK},
    {"def", TokenType::DEF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"False", TokenType::FALSE},
    {"global", TokenType::GLOBAL},
    {"if", TokenType::IF},
    {"in", TokenType::IN},
    {"is", TokenType::IS},
    {"lambda", TokenType::LAMBDA},
    {"None", TokenType::NONE},
    {"or", TokenType::OR},
    {"return", TokenType::RETURN},
    {"True", TokenType::TRUE},
    {"try", TokenType::TRY},
    {"print", TokenType::PRINT}};

struct token
{
    TokenType typeOfToken;
    string contents;
};

class Lexer
{
private:
    const vector<string> &lines;
    // size_t lineIndexer;
    // size_t charIndexer;
    string lexeme;
    charType currCharType;
    TokenType currToken;
    char currChar;
    bool inQuote;
    int prevLineLevel, currLineLevel;

public:
    Lexer() : lines(vector<string>()),
              // lineIndexer(0),
              // charIndexer(0),
              lexeme(""),
              currCharType(blank),
              currToken(TokenType::BLANK),
              currChar('\0'),
              inQuote(false),
              prevLineLevel(-1),
              currLineLevel(0)
    {
    }

    Lexer(const vector<string> &lines) : lines(lines),
                                         // lineIndexer(0),
                                         // charIndexer(0),
                                         lexeme(""),
                                         currCharType(blank),
                                         currToken(TokenType::BLANK),
                                         currChar('\0'),
                                         inQuote(false),
                                         prevLineLevel(-1),
                                         currLineLevel(0)
    {
    }

    void printLine(int currLine)
    {
        cout << lines[currLine] << endl;
    }

    void
    lexLine(int currLine)
    {
        printLine(currLine);
        cout << endl;

        for (int charIndex = 0; charIndex < lines[currLine].size(); charIndex++)
        {
            getChar(currLine, charIndex);
            // cout << "|>" << lines[currLine][charIndex] << "<| " << endl
            //   << endl;
        }

        currCharType = blank;
        lexeme = "";
        currChar = '\0';
        inQuote = false;
        // charIndexer = 0;
        // lineIndexer++;

        cout << endl;
    }

    void getChar(int lineIndexer, int charIndexer)
    {
        currChar = lines[lineIndexer][charIndexer];
        currCharType = getCharType(currChar);
        char nextChar = lines[lineIndexer][charIndexer + 1];

        if (charIndexer < lines[lineIndexer].size())
        {
            if (currChar == '"')
            {
                if (inQuote == true)
                {
                    tokenLookUp();
                    cout << "Token: " << static_cast<int>(currToken) << " ";
                    cout << "lexeme = '" << lexeme << "'" << endl;
                    lexeme = "";
                    inQuote = false;
                    return;
                }
                else
                {
                    tokenLookUp();
                    cout << "Token: " << static_cast<int>(currToken) << " ";
                    cout << "lexeme = '" << lexeme << "'" << endl;
                    lexeme = "";
                    inQuote = true;
                    return;
                }
            }

            addChar();
            cout << "currChar: " << currChar << " lexeme: " << lexeme << " inQuote: " << inQuote << " currCharType: " << currCharType << " nextCharType: " << getCharType(nextChar) << endl;
            if (getCharType(nextChar) != currCharType && inQuote == false /* && (inQuote == false)*/)
            {
                // if (currCharType != blank)
                //{
                tokenLookUp();
                cout << "Token: " << static_cast<int>(currToken) << " ";
                cout << "lexeme = '" << lexeme << "'" << endl;
                lexeme = "";
                //}
            }
            currCharType = getCharType(currChar);
            charIndexer++;
        }
    }

    charType getCharType(char currChar)
    {
        if (currChar == ' ')
        {
            return whitespace;
        }
        else if (isalpha(currChar))
        {
            return letter;
        }
        else if (isdigit(currChar))
        {
            return digit;
        }
        else
        {
            return irregChar;
        }
    }

    void addChar()
    {
        lexeme += currChar;
    }

    bool isEndOfFile(int lineIndexer, int charIndexer) const
    {
        return lineIndexer >= lines.size() && charIndexer >= lines.back().size();
    }

    void tokenLookUp()
    {
        auto it = tokenMap.find(lexeme);
        if (it != tokenMap.end())
        {
            // cout << lexeme << " is found in the map with TokenType: " << static_cast<int>(it->second) << endl;
            currToken = (it->second);
        }
        else
        {
            // cout << lexeme << " is not found in the map." << endl;
            if (isdigit(lexeme[0]))
            {
                currToken = TokenType::NUMBER;
            }
            else
            {
                if (inQuote == true)
                {
                    currToken = TokenType::STRING;
                }
                else
                {
                    currToken = TokenType::IDENTIFIER;
                }
            }
        }
    }
};

int main(int argc, char *argv[])
{
    vector<string> inputLines;
    if (argc < 2)
    {
        cerr << "Error: No python file provided" << endl;
        return -1;
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
                    /*
                            cout
                        << tempString << endl;
                    cout << tempString.length() << " " << commentLength << endl;*/
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
        else
        {
            cerr << "Error: Unable to open file! || File does not exist!" << endl;
            return -1;
        }
    }

    Lexer theLexer(inputLines);

    for (int arrayIndex = 0; arrayIndex < inputLines.size(); arrayIndex++)
    {
        // theLexer.printLine(arrayIndex);
        theLexer.lexLine(arrayIndex);
        /*
                cout
            << inputLines[arrayIndex] << endl;
        for (int charIndex = 0; charIndex < inputLines[arrayIndex].size(); charIndex++)
        {
            cout << "|>" << inputLines[arrayIndex][charIndex] << "<|   ";
        }
        cout << endl;
        */
    }

    return 0;
}