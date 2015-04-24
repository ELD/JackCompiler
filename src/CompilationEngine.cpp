#include "../headers/CompilationEngine.hpp"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : outputFile(output), tokenizer(input)
{
    compileClass();
}

void CompilationEngine::compileClass()
{
    // Opening the class
    outputFile << "<class>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::CLASS);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;
    className = tokenizer.identifier();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (tokenizer.hasMoreTokens()) {

        // ClassVarDec and Subroutine Decs
        if (tokenizer.peekTokenType() == TokenType::KEYWORD &&
            (tokenizer.peekKeywordType() == KeywordType::FIELD || tokenizer.peekKeywordType() == KeywordType::STATIC)) {
            compileClassVarDec();
        } else if (tokenizer.peekTokenType() == TokenType::KEYWORD &&
            (tokenizer.peekKeywordType() == KeywordType::FUNCTION ||
                tokenizer.peekKeywordType() == KeywordType::METHOD ||
                tokenizer.peekKeywordType() == KeywordType::CONSTRUCTOR)) {
            compileSubroutine();
        } else {
            break;
        }
    }

    // Closing the class
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    outputFile << "<symbol>"
        << tokenizer.symbol()
        << "</symbol>" << endl;
    outputFile << "</class>";
}

void CompilationEngine::compileClassVarDec()
{
    outputFile << "<classVarDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    outputFile << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine()
{
    outputFile << "<subroutineDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::METHOD ||
        tokenizer.keywordType() == KeywordType::FUNCTION ||
        tokenizer.keywordType() == KeywordType::CONSTRUCTOR);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else {
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
        BOOST_ASSERT(tokenizer.keywordType() == KeywordType::INT ||
            tokenizer.keywordType() == KeywordType::CHAR ||
            tokenizer.keywordType() == KeywordType::BOOLEAN ||
            tokenizer.keywordType() == KeywordType::VOID);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.keywordType()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "<parameterList>" << endl;
    compileParameterList();
    outputFile << "</parameterList>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    outputFile << "<subroutineBody>" << endl;

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        if (tokenizer.peek() == "}") {
            break;
        }

        if (tokenizer.peekTokenType() == TokenType::KEYWORD && tokenizer.peekKeywordType() == KeywordType::VAR) {
            compileVarDec();
        } else {
            compileStatements();
        }
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</subroutineBody>" << endl;

    outputFile << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList()
{
    if (tokenizer.peek() == ")") {
        return;
    }
    tokenizer.advance();

    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ",") {
        tokenizer.advance();
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileParameterList();
    }
}

void CompilationEngine::compileVarDec()
{
    outputFile << "<varDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::VAR);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.keywordType()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    outputFile << "</varDec>" << endl;
}

void CompilationEngine::compileStatements()
{
    outputFile << "<statements>" << endl;

    while (true) {
        if (tokenizer.peek() == "}") {
            break;
        }

        BOOST_ASSERT(tokenizer.peekTokenType() == TokenType::KEYWORD);

        if (tokenizer.peekKeywordType() == KeywordType::DO) {
            compileDo();
        } else if (tokenizer.peekKeywordType() == KeywordType::LET) {
            compileLet();
        } else if (tokenizer.peekKeywordType() == KeywordType::WHILE) {
            compileWhile();
        } else if (tokenizer.peekKeywordType() == KeywordType::RETURN) {
            compileReturn();
        } else if (tokenizer.peekKeywordType() == KeywordType::IF) {
            compileIf();
        }
    }

    outputFile << "</statements>" << endl;
}

void CompilationEngine::compileDo()
{
    outputFile << "<doStatement>" << endl;

    tokenizer.advance();

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();

    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ".") {
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpressionList();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ";");
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</doStatement>" << endl;
}

void CompilationEngine::compileLet()
{
    outputFile << "<letStatement>" << endl;

    tokenizer.advance();

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.identifier()
        << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peek() == "[") {
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileExpression();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "]");
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "=");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ";");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</letStatement>" << endl;
}

void CompilationEngine::compileWhile()
{
    outputFile << "<whileStatement>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</whileStatement>" << endl;
}

void CompilationEngine::compileReturn()
{
    outputFile << "<returnStatement>" << endl;

    tokenizer.advance();
    outputFile << "<keyword>"
        << tokenizer.getToken()
        << "</keyword>" << endl;

    if (tokenizer.peek() != ";") {
       compileExpression();
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    outputFile << "</returnStatement>" << endl;
}

void CompilationEngine::compileIf()
{
    outputFile << "<ifStatement>" << endl;

    tokenizer.advance();

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.getToken()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    outputFile << "<" << tokenizer.tokenType() << ">"
        << tokenizer.symbol()
        << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peek() == "else") {
        tokenizer.advance();

        outputFile << "<else>" << endl;

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.getToken()
            << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "{");

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileStatements();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "}");

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        outputFile << "</else>" << endl;
    }

    outputFile << "</ifStatement>" << endl;
}

void CompilationEngine::compileExpression()
{
    outputFile << "<expression>" << endl;

    compileTerm();

    while (tokenizer.isOperator(tokenizer.peek())) {
        tokenizer.advance();

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileTerm();
    }

    outputFile << "</expression>" << endl;
}

void CompilationEngine::compileTerm()
{
    outputFile << "<term>" << endl;

    tokenizer.advance();
    if (tokenizer.tokenType() == TokenType::INT_CONST) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.intVal()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::STRING_CONST) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.stringVal()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::KEYWORD &&
        (tokenizer.keywordType() == KeywordType::TRUE ||
        tokenizer.keywordType() == KeywordType::FALSE ||
        tokenizer.keywordType() == KeywordType::NULL ||
        tokenizer.keywordType() == KeywordType::THIS)) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.getToken()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.identifier()
            << "</" << tokenizer.tokenType() << ">" << endl;

        if (tokenizer.peek() == "[") {
            tokenizer.advance();
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpression();

            tokenizer.advance();
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
        }

        if (tokenizer.peek() == "(") {
            tokenizer.advance();
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpressionList();

            tokenizer.advance();
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
        }

        if (tokenizer.peek() == ".") {
            tokenizer.advance();

            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.identifier()
                << "</" << tokenizer.tokenType() << ">" << endl;

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpressionList();

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
        }
    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == "(") {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileExpression();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.isUnaryOperator(tokenizer.getToken())) {
        outputFile << "<" << tokenizer.tokenType() << ">"
            << tokenizer.symbol()
            << "</" << tokenizer.tokenType() << ">" << endl;

        compileTerm();
    }

    outputFile << "</term>" << endl;
}

void CompilationEngine::compileExpressionList()
{
    outputFile << "<expressionList>" << endl;

    while (true) {
        if (tokenizer.peek() == ")") {
            break;
        }

        compileExpression();

        if (tokenizer.peek() == ",") {
            tokenizer.advance();
            outputFile << "<" << tokenizer.tokenType() << ">"
                << tokenizer.symbol()
                << "</" << tokenizer.tokenType() << ">" << endl;
        }
    }

    outputFile << "</expressionList>" << endl;
}
