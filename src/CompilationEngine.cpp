#include "../headers/CompilationEngine.h"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : outputFile(output), tokenizer(input)
{
    compileClass();
}

void CompilationEngine::compileClass()
{
    auto token = tokenizer.nextToken();
    auto tokenType = tokenizer.tokenType();

    assert(tokenType == TokenType::KEYWORD);
    assert(tokenizer.keywordType() == KeywordType::CLASS);

    outputFile << "<class>" << endl;
    outputFile << "\t<keyword>" << token << "</keyword>" << endl;

    token = tokenizer.nextToken();
    tokenType = tokenizer.tokenType();
    assert(tokenType == TokenType::IDENTIFIER);

    outputFile << "\t<identifier>" << token << "</identifier>" << endl;

    token = tokenizer.nextToken();
    tokenType = tokenizer.tokenType();

    assert(tokenType == TokenType::SYMBOL);

    outputFile << "\t<symbol>" << token << "</symbol>" << endl;

    outputFile << "</class>" << endl;
}
