#include "../headers/CompilationEngine.h"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : outputFile(output), tokenizer(input)
{
    compileClass();
    outputFile << "Test";
}

void CompilationEngine::compileClass()
{
    // Assert that the first keyword is class, get the identifier
    // Assert that an opening brace appears
    // Until you reach the closing brace, compile ClassVarDecs and SubroutineDecs
}

void CompilationEngine::compileClassVarDec()
{
}
