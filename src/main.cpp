#include "../headers/includes.h"
#include "../headers/JackTokenizer.h"

void printTokenXml(string, JackTokenizer&);
// void printParserXml(string, JackTokenizer&);

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Too few arguments!" << endl;
        return -1;
    }

    string inputFile{argv[1]};
    JackTokenizer tokens(inputFile);

    // printTokenXml("OutT.xml", tokens);
    auto files = getFilesInProject(inputFile);
    for (auto& file : files) {
        cout << file.string() << endl;
    }
}

void printTokenXml(string outFileName, JackTokenizer& tokens)
{
    ofstream outXml(outFileName);
    outXml << "<tokens>" << endl;
    while (tokens.hasMoreTokens()) {
        auto token = tokens.nextToken();
        auto tokenType = tokens.tokenType();
        if (tokenType == TokenType::SYMBOL) {
            if (token == ">") {
                outXml << "<" << tokenType << "> &gt; </" << tokenType << ">" << endl;
            } else if (token == "<") {
                outXml << "<" << tokenType << "> &lt; </" << tokenType << ">" << endl;
            } else if (token == "&") {
                outXml << "<" << tokenType << "> &amp; </" << tokenType << ">" << endl;
            } else {
                outXml << "<" << tokenType << ">" << tokens.symbol() << "</" << tokenType << ">" << endl;
            }
        } else if (tokenType == TokenType::KEYWORD) {
            outXml << "<" << tokenType << ">" << token << "</" << tokenType << ">" << endl;
        } else if (tokenType == TokenType::IDENTIFIER) {
            outXml << "<" << tokenType << ">" << tokens.identifier() << "</" << tokenType << ">" << endl;
        } else if (tokenType == TokenType::INT_CONST) {
            outXml << "<" << tokenType << ">" << tokens.intVal() << "</" << tokenType << ">" << endl;
        } else {
            outXml << "<" << tokenType << ">" << tokens.stringVal() << "</" << tokenType << ">" << endl;
        }
    }
    outXml << "</tokens>" << endl;
}

// void printParserXml(string outFileName, JackTokenizer& tokens)
// {
//
// }
