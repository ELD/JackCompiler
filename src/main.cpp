#include "../headers/includes.hpp"
#include "../headers/JackTokenizer.hpp"
#include "../headers/CompilationEngine.hpp"

using FuturesVec = vector<future<void>>;

int main(int argc, char** argv)
{
    FuturesVec doNothingCollection;
    if (argc < 2) {
        cerr << "Too few arguments!" << endl;
        return -1;
    }

    auto files = getFilesInProject(argv[1]);
    for (auto const& file : files) {
        doNothingCollection.emplace_back(async(launch::async, [&](){
            ifstream inputFile{file.string()};
            string filename = file.string();
            filename = filename.substr(0, filename.find_last_of(".")) + ".vm";
            ofstream outputFile{filename};
            CompilationEngine jackCompiler(inputFile, outputFile);
            inputFile.close();
            outputFile.close();
        }));
    }
}
