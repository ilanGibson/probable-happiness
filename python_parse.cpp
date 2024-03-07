#include <iostream>
#include <fstream>
#include <string>
#include <list>

int main() {
    std::ifstream file("helloworld.py");
    std::ofstream outputFile("newFile.py"); // Open a temporary file to store corrected lines

    if(!file) {
        std::cerr << "File not found" << std::endl;
        return 1;
    }

    std::string line;
    int indentationLevel = 0;
    bool firstLineAfterColon = false;
    std::list<int> ifLoopPositions; // Store the position of if loops to be used for else loops
    while(std::getline(file, line)) {
        if(line.find("if ") != std::string::npos) { // Store the position of if loops
            ifLoopPositions.push_back(indentationLevel);
        }
        
        if(line.find("else:") != std::string::npos && !ifLoopPositions.empty()) { // Add the position of if loop to the else loop
            size_t leadingSpaces = line.find_first_not_of(" \t"); // Get the leading spaces of the line
            outputFile << std::string(ifLoopPositions.back() * 4, ' ') << line.substr(leadingSpaces) << std::endl; // Add the if loop position to the else loop
            ifLoopPositions.pop_back(); // Remove the last if loop position
            continue;
        }

        size_t leadingSpaces = line.find_first_not_of(" \t"); // Get the leading spaces of the line
        int expectedSpaces = indentationLevel * 4; // Calculate the expected indentation

        std::cout << "leading spaces: " << leadingSpaces << std::endl;
        std::cout << "indentation level: " << indentationLevel << std::endl;
        std::cout << "expected indentation: " << expectedSpaces << std::endl;

        if(leadingSpaces == expectedSpaces) { // If the line has the correct indentation, write it to the file
            outputFile << line << std::endl;
        }
        else if(leadingSpaces > expectedSpaces) { // If the line has more indentation than expected, write it with correct indentation to the file
            outputFile << std::string(expectedSpaces, ' ') << line.substr(leadingSpaces) << std::endl; 
        } 
        else if(leadingSpaces < expectedSpaces) { // If the line has less indentation than expected, write it with correct indentation to the file
            if(firstLineAfterColon) { // If the line is the first line after a colon, line must be indented
                outputFile << std::string(expectedSpaces, ' ') << line.substr(leadingSpaces) << std::endl;
                firstLineAfterColon = false;
            }
            else{ // If the line is not the first line after a colon, line isnt required to be indented 
                outputFile << line.substr((leadingSpaces % 4)) << std::endl; // Remove the extra spaces from the line
                indentationLevel -= (expectedSpaces - (leadingSpaces - (leadingSpaces % 4))) / 4; // Decrease the indentation level by subtracting the extra spaces from the expected indentation
                
            }
        }

        if (firstLineAfterColon && line.find_last_of(":") != line.length() - 1) { // If the line is the first line after a colon and the line does not end with a colon, set firstLineAfterColon to false
            firstLineAfterColon = false;
        }

        if(line.find_last_of(":") == line.length() - 1){ // If the line ends with a colon, set firstLineAfterColon to true and increase the indentation level
            firstLineAfterColon = true;
            indentationLevel++;
        }
    }
    outputFile.close();
    file.close();

    return 0;
}
