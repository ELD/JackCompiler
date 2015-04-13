#include "../headers/CompilationEngine.h"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : outputFile(output), tokenizer(input)
{
    compileClass();
}

void CompilationEngine::compileClass()
{
    // Opening the class
    tokenizer.advanceToken();

    outputFile << "<class>" << endl;

    assert(tokenizer.tokenType() == TokenType::KEYWORD);
    assert(tokenizer.keywordType() == KeywordType::CLASS);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advanceToken();

    assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;
    className = tokenizer.identifier();

    tokenizer.advanceToken();
    assert(tokenizer.tokenType() == TokenType::SYMBOL);
    assert(tokenizer.symbol() == "{");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (tokenizer.hasMoreTokens()) {
        tokenizer.advanceToken();
        // ClassVarDec and Subroutine Decs
        if (tokenizer.tokenType() == TokenType::KEYWORD &&
            (tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC)) {
            compileClassVarDec();
        } else if (tokenizer.tokenType() == TokenType::KEYWORD &&
            (tokenizer.keywordType() == KeywordType::FUNCTION ||
                tokenizer.keywordType() == KeywordType::METHOD ||
                tokenizer.keywordType() == KeywordType::CONSTRUCTOR)) {
            compileSubroutine();
        } else {
            // break;
            continue;
        }
    }

    // Closing the class
    // tokenizer.advanceToken();
    // assert(tokenizer.tokenType() == TokenType::SYMBOL);
    // assert(tokenizer.symbol() == "}");

    outputFile << "</class>";
}

void CompilationEngine::compileClassVarDec()
{
    outputFile << "<classVarDec>" << endl;

    while (true) {
        assert(tokenizer.tokenType() == TokenType::KEYWORD);
        assert(tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.keywordType()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advanceToken();
        assert(tokenizer.tokenType() == TokenType::KEYWORD);
        assert(tokenizer.keywordType() == KeywordType::BOOLEAN ||
            tokenizer.keywordType() == KeywordType::INT ||
            tokenizer.keywordType() == KeywordType::CHAR);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.keywordType()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advanceToken();
        assert(tokenizer.tokenType() == TokenType::IDENTIFIER);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;

        while (true) {
            tokenizer.advanceToken();
            if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
                outputFile << "<" << tokenizer.tokenType() << ">"
                    << tokenizer.symbol()
                    << "</" << tokenizer.tokenType() << ">" << endl;
                break;
            }

            assert(tokenizer.tokenType() == TokenType::SYMBOL);
            assert(tokenizer.symbol() == ",");
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;

            tokenizer.advanceToken();
            assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.identifier()
                << "</" << tokenizer.tokenType() << ">" << endl;
        }

        break;
    }

    outputFile << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine()
{
    outputFile << "<subroutineDec>" << endl;

    assert(tokenizer.tokenType() == TokenType::KEYWORD);
    assert(tokenizer.keywordType() == KeywordType::METHOD ||
        tokenizer.keywordType() == KeywordType::FUNCTION ||
        tokenizer.keywordType() == KeywordType::CONSTRUCTOR);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advanceToken();
    if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else {
        assert(tokenizer.tokenType() == TokenType::KEYWORD);
        assert(tokenizer.keywordType() == KeywordType::INT ||
            tokenizer.keywordType() == KeywordType::CHAR ||
            tokenizer.keywordType() == KeywordType::BOOLEAN ||
            tokenizer.keywordType() == KeywordType::VOID);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.keywordType()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advanceToken();
    assert(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advanceToken();
    assert(tokenizer.tokenType() == TokenType::SYMBOL);
    assert(tokenizer.symbol() == "(");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileParameterList();

    assert(tokenizer.tokenType() == TokenType::SYMBOL);
    assert(tokenizer.symbol() == ")");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList()
{
    outputFile << "<parameterList>" << endl;

    while (true) {
        tokenizer.advanceToken();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ")") {
            break;
        }

        assert(tokenizer.tokenType() == TokenType::KEYWORD);
        assert(tokenizer.keywordType() == KeywordType::INT ||
            tokenizer.keywordType() == KeywordType::CHAR ||
            tokenizer.keywordType() == KeywordType::BOOLEAN);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.keywordType()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advanceToken();
        assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advanceToken();
        if (tokenizer.symbol() != ",") {
            break;
        }

        assert(tokenizer.tokenType() == TokenType::SYMBOL);
        assert(tokenizer.symbol() == ",");
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    outputFile << "</parameterList>" << endl;
}
