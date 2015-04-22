#include "../headers/includes.h"
#include "../headers/JackTokenizer.h"
#include "../headers/CompilationEngine.h"

void printTokenXml(string, JackTokenizer&);

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Too few arguments!" << endl;
        return -1;
    }

    auto files = getFilesInProject(argv[1]);
    for (auto& file : files) {
        // ifstream inputFile{file.string()};
        // JackTokenizer tokens(inputFile);
        // string filename = file.filename().string();
        // filename = filename.substr(0, filename.find_last_of(".")) + "T.xml";
        // cout << "input: " << file.string() << "\toutput: " << filename << endl;
        // printTokenXml(filename, tokens);
        ifstream inputFile{file.string()};
        string filename = file.filename().string();
        filename = filename.substr(0, filename.find_last_of(".")) + ".xml";
        ofstream outputFile{filename};
        CompilationEngine jackCompiler(inputFile, outputFile);
    }
}

void printTokenXml(string outFileName, JackTokenizer& tokens)
{
    ofstream outXml(outFileName);
    outXml << "<tokens>" << endl;
    while (tokens.hasMoreTokens()) {
        tokens.advance();
        auto tokenType = tokens.tokenType();
        if (tokenType == TokenType::SYMBOL) {
            outXml << "<" << tokenType << ">" << tokens.symbol() << "</" << tokenType << ">" << endl;
        } else if (tokenType == TokenType::KEYWORD) {
            outXml << "<" << tokenType << ">" << tokens.keywordType() << "</" << tokenType << ">" << endl;
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
