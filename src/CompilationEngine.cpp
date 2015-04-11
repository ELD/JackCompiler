#include "../headers/CompilationEngine.h"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : outputFile(output), tokenizer(input)
{
    outputFile << "Test" << endl;
    // compileClass();
}
