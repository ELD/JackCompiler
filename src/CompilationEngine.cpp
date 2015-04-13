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
            if (tokenizer.getTokens().size() == 0) {
                break;
            }
            // break;
            continue;
        }
    }

    // Closing the class
    assert(tokenizer.tokenType() == TokenType::SYMBOL);
    assert(tokenizer.symbol() == "}");

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
        assert(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.getToken()
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

    tokenizer.advanceToken();
    assert(tokenizer.tokenType() == TokenType::SYMBOL);
    assert(tokenizer.symbol() == "{");

    outputFile << "<subroutineBody>" << endl;

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        tokenizer.advanceToken();
        cout << tokenizer.getToken() << endl;
        if (tokenizer.tokenType() == TokenType::SYMBOL) {
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
            break;
        }

        assert(tokenizer.tokenType() == TokenType::KEYWORD);

        if (tokenizer.keywordType() == KeywordType::VAR) {
            compileVarDec();
        } else {
            compileStatements();
        }
    }

    outputFile << "</subroutineBody>" << endl;

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

void CompilationEngine::compileVarDec()
{
    outputFile << "<varDec>" << endl;

    assert(tokenizer.keywordType() == KeywordType::VAR);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advanceToken();
    assert(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
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

    outputFile << "</varDec>" << endl;
}

void CompilationEngine::compileStatements()
{
    assert(tokenizer.tokenType() == TokenType::KEYWORD);

    if (tokenizer.keywordType() == KeywordType::DO) {
        // compileDo();
    } else if (tokenizer.keywordType() == KeywordType::LET) {
        // compileLet();
    } else if (tokenizer.keywordType() == KeywordType::WHILE) {
        // compileWhile();
    } else if (tokenizer.keywordType() == KeywordType::RETURN) {
        // compileReturn();
    } else if (tokenizer.keywordType() == KeywordType::IF) {
        // compileIf();
    }
}
