#include <iostream>
#include <fstream>
#include <string>

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
    while(std::getline(file, line)) {
        size_t leadingSpaces = line.find_first_not_of(" \t");
        int expectedSpaces = indentationLevel * 4;

        std::cout << "leading spaces: " << leadingSpaces << std::endl;
        std::cout << "indentation level: " << indentationLevel << std::endl;
        std::cout << "expected indentation: " << expectedSpaces << std::endl;

        if(leadingSpaces == expectedSpaces) {
            outputFile << line << std::endl;
        }
        else if(leadingSpaces > expectedSpaces) {
            outputFile << std::string(expectedSpaces, ' ') << line.substr(leadingSpaces) << std::endl;
        } 
        else if(leadingSpaces < expectedSpaces) {
            if(firstLineAfterColon) {
                outputFile << std::string(expectedSpaces, ' ') << line << std::endl;
                firstLineAfterColon = false;
            }
            outputFile << line << std::endl;
            indentationLevel--;
        }


        if(line.find_last_of(":") == line.length() - 1){
            firstLineAfterColon = true;
            indentationLevel++;
        }
    }
    outputFile.close();
    file.close();

    return 0;
}