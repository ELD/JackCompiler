#include "../headers/includes.hpp"
#include "../headers/JackTokenizer.hpp"
#include "../headers/CompilationEngine.hpp"

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Too few arguments!" << endl;
        return -1;
    }

    auto files = getFilesInProject(argv[1]);
    for (auto const& file : files) {
        ifstream inputFile{file.string()};
        string filename = file.string();
        filename = filename.substr(0, filename.find_last_of(".")) + ".vm";
        ofstream outputFile{filename};
        CompilationEngine jackCompiler(inputFile, outputFile);
        inputFile.close();
        outputFile.close();
    }
}

