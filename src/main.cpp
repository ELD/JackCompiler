#include "../headers/includes.hpp"
#include "../headers/JackTokenizer.hpp"
#include "../headers/CompilationEngine.hpp"

using FuturesVec = vector<future<void>>;

void printTokenXml(string, JackTokenizer&);

int main(int argc, char** argv)
{
    FuturesVec results;
    if (argc < 2) {
        cerr << "Too few arguments!" << endl;
        return -1;
    }

    auto files = getFilesInProject(argv[1]);
    for (auto const& file : files) {
        results.emplace_back(async(launch::async, [&](){
            ifstream inputFile{file.string()};
            string filename = file.string();
            filename = filename.substr(0, filename.find_last_of(".")) + ".vm";
            ofstream outputFile{filename};
            CompilationEngine jackCompiler(inputFile, outputFile);
        }));
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
