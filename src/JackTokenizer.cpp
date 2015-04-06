#include "../headers/JackTokenizer.h"

JackTokenizer::JackTokenizer(istream& input)
{
    // While you can get tokens broken on whitespace, store into a vector
    // If it's a comment, ignore the rest of the line
    // If it's multiline comment or doc block comment, ignore until closing delimiter
    string token;
    while (input >> token) {
        // If comments exist, strip them
        if (token.find("//") != string::npos) {
            getline(input, token);
            continue;
        }

        // If multiline comments exist do something more complex
        if (token.find("/*") != string::npos || token.find("/**") != string::npos) {
            bool finished = false;
            while (input >> token && !finished) {
                while ((token.find("*/") == string::npos) != (token.find("**/") == string::npos)) {
                    finished = true;
                    break;
                }
            }
        }

        tokens.emplace_back(token);
    }
}

// ------------ Private methods ------------ //
//void JackTokenizer::stripComments()
//{
//}

//void JackTokenizer::Onelineify(string& fileContents)
//{
    //fileContents.erase(std::remove(fileContents.begin(), fileContents.end(), '\r'), fileContents.end());
    //fileContents.erase(std::remove(fileContents.begin(), fileContents.end(), '\n'), fileContents.end());
    //fileContents.erase(std::remove(fileContents.begin(), fileContents.end(), '\t'), fileContents.end());
    //string::iterator newEnd = std::unique(fileContents.begin(), fileContents.end(), [&](char lhs, char rhs) -> bool { return (lhs == rhs) && (lhs == ' '); });
    //fileContents.erase(newEnd, fileContents.end());
//}

