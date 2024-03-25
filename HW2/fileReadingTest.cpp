#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("example.txt"); // Open the file for reading
    std::string line;

    if (file.is_open())
    { // Check if the file is open successfully
        // Read and output each line of the file
        while (std::getline(file, line))
        {
            std::cout << line << std::endl;
        }
        file.close(); // Close the file when done
    }
    else
    {
        std::cerr << "Unable to open file!" << std::endl;
    }

    return 0;
}
