using namespace std;
#include <iostream>
#include <fstream>
#include <vector>

enum class TokenType
{
    // Single-character tokens
    LEFT_PAREN,  // (         (0)
    RIGHT_PAREN, // )         (1)
    LEFT_BRACE,  // {         (2)
    RIGHT_BRACE, // }         (3)
    COMMA,       // ,         (4)
    DOT,         // .         (5)
    MINUS,       // -         (6)
    PLUS,        // +         (7)
    SEMICOLON,   // ;         (8)
    SLASH,       // /         (9)
    STAR,        // *         (10)

    // One or two character tokens
    BANG,          // !       (11)
    BANG_EQUAL,    // !=      (12)
    EQUAL,         // =       (13)
    EQUAL_EQUAL,   // ==      (14)
    GREATER,       // >       (15)
    GREATER_EQUAL, // >=      (16)
    LESS,          // <       (17)
    LESS_EQUAL,    // <=      (18)

    // Literals
    IDENTIFIER, // identifier (19)
    STRING,     // string     (20)
    NUMBER,     // number     (21)

    // Keywords
    AND,      // and          (22)
    AS,       // as           (23)
    ASSERT,   // assert       (24)
    BREAK,    // break        (25)
    CLASS,    // class        (26)
    CONTINUE, // continue     (27)
    DEF,      // def          (28)
    DEL,      // del          (29)
    ELIF,     // elif         (30)
    ELSE,     // else         (31)
    EXCEPT,   // except       (32)
    FALSE,    // False        (33)
    FINALLY,  // finally      (34)
    FOR,      // for          (35)
    FROM,     // from         (36)
    GLOBAL,   // global       (37)
    IF,       // if           (38)
    IMPORT,   // import       (39)
    IN,       // in           (40)
    IS,       // is           (41)
    LAMBDA,   // lambda       (42)
    NONE,     // None         (43)
    NONLOCAL, // nonlocal     (44)
    NOT,      // not          (45)
    OR,       // or           (46)
    PASS,     // pass         (47)
    RAISE,    // raise        (48)
    RETURN,   // return       (49)
    TRUE,     // True         (50)
    TRY,      // try          (51)
    WHILE,    // while        (52)
    WITH,     // with         (53)
    YIELD,    // yield        (54)
};

struct token
{
    TokenType typeOfToken;
    string contents;
};

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

    TokenType tokenTypeAt1 = TokenType::STAR;

    cout << "Token type: " << static_cast<int>(tokenTypeAt1) << endl;

    return 0;
}