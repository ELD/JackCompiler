#include "../headers/CompilationEngine.hpp"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : tokenizer(input), writer(output), className(""), currentSubroutineName(""), argCount(0)
{
    compileClass();
}

void CompilationEngine::compileClass()
{
    // Opening the class
    // outputFile << "<class>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::CLASS);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.keywordType()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    className = tokenizer.identifier();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

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

    // outputFile << "<symbol>"
    //     << tokenizer.symbol()
    //     << "</symbol>" << endl;
    //
    // outputFile << "</class>";
}

void CompilationEngine::compileClassVarDec()
{
    SymbolTypes symbolKind;
    string symbolType;

    // outputFile << "<classVarDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC);

    if (tokenizer.keywordType() == KeywordType::FIELD) {
        symbolKind = SymbolTypes::FIELD;
    } else {
       symbolKind = SymbolTypes::STATIC;
    }

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.keywordType()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolType = tokenizer.getToken();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), symbolType, symbolKind);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        symbolTable.define(tokenizer.identifier(), symbolType, symbolKind);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    }

    // outputFile << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine()
{
    // Reset the symbol table for another subroutine
    symbolTable.startSubroutine();

    // outputFile << "<subroutineDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::METHOD ||
        tokenizer.keywordType() == KeywordType::FUNCTION ||
        tokenizer.keywordType() == KeywordType::CONSTRUCTOR);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.keywordType()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else {
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
        BOOST_ASSERT(tokenizer.keywordType() == KeywordType::INT ||
            tokenizer.keywordType() == KeywordType::CHAR ||
            tokenizer.keywordType() == KeywordType::BOOLEAN ||
            tokenizer.keywordType() == KeywordType::VOID);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.keywordType()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    currentSubroutineName = tokenizer.identifier();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "<parameterList>" << endl;

    compileParameterList();

    // outputFile << "</parameterList>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    // outputFile << "<subroutineBody>" << endl;

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        if (tokenizer.peek() == "}") {
            break;
        }

        if (tokenizer.peekTokenType() == TokenType::KEYWORD && tokenizer.peekKeywordType() == KeywordType::VAR) {
            compileVarDec();
        } else {
            writer.writeFunction(className + "." + currentSubroutineName, symbolTable.varCount(SymbolTypes::VAR));
            compileStatements();
        }
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</subroutineBody>" << endl;
    //
    // outputFile << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList()
{
    string type;

    if (tokenizer.peek() == ")") {
        return;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    type = tokenizer.getToken();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), type, SymbolTypes::ARG);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ",") {
        tokenizer.advance();
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileParameterList();
    }
}

void CompilationEngine::compileVarDec()
{
    string type;

    // outputFile << "<varDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::VAR);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.keywordType()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    type = tokenizer.getToken();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), type, SymbolTypes::VAR);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        symbolTable.define(tokenizer.identifier(), type, SymbolTypes::VAR);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    }

    // outputFile << "</varDec>" << endl;
}

void CompilationEngine::compileStatements()
{
    // outputFile << "<statements>" << endl;

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

    // outputFile << "</statements>" << endl;
}

void CompilationEngine::compileDo()
{
    string subroutineClassName, subroutineName, type;
    bool localCall = false;
    argCount = 0;
    // outputFile << "<doStatement>" << endl;

    tokenizer.advance();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    subroutineName = tokenizer.identifier();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ".") {
        subroutineClassName = tokenizer.identifier();
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        subroutineName = tokenizer.identifier();

        if (symbolTable.indexOf(subroutineClassName) != -1) {
            SymbolTypes segment = symbolTable.kindOf(subroutineClassName);
            type = symbolTable.typeOf(subroutineClassName);
            int offset = symbolTable.indexOf(subroutineClassName);
            if (segment == SymbolTypes::STATIC) {
                writer.writePush(SegmentTypes::STATIC, offset);
            } else if (segment == SymbolTypes::FIELD) {
                writer.writePush(SegmentTypes::THIS, offset);
            } else if (segment == SymbolTypes::ARG) {
                writer.writePush(SegmentTypes::ARG, offset);
            } else if (segment == SymbolTypes::VAR) {
                writer.writePush(SegmentTypes::LOCAL, offset);
            }
            ++argCount;
        }

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else {
        // Handle class subroutine
        // Push implicit this, call ClassName.subroutine()
        localCall = true;
        writer.writePush(SegmentTypes::POINTER, 0);
        ++argCount;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpressionList();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ";");

    if (localCall) {
        writer.writeCall(className + "." + subroutineName, argCount);
        argCount = 0;
    } else {
        if (!type.empty()) {
            writer.writeCall(type + "." + subroutineName, argCount);
        } else {
            writer.writeCall(subroutineClassName + "." + subroutineName, argCount);
        }
        argCount = 0;
    }

    writer.writePop(SegmentTypes::TEMP, 0);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</doStatement>" << endl;
}

void CompilationEngine::compileLet()
{
    // outputFile << "<letStatement>" << endl;

    tokenizer.advance();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peek() == "[") {
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileExpression();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "]");

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "=");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ";");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</letStatement>" << endl;
}

void CompilationEngine::compileWhile()
{
    // outputFile << "<whileStatement>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</whileStatement>" << endl;
}

void CompilationEngine::compileReturn()
{
    // outputFile << "<returnStatement>" << endl;

    tokenizer.advance();

    // outputFile << "<keyword>"
    //     << tokenizer.getToken()
    //     << "</keyword>" << endl;

    if (tokenizer.peek() != ";") {
       compileExpression();
    } else {
        writer.writePush(SegmentTypes::CONST, 0);
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    writer.writeReturn();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</returnStatement>" << endl;
}

void CompilationEngine::compileIf()
{
    // outputFile << "<ifStatement>" << endl;

    tokenizer.advance();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileExpression();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peek() == "else") {
        tokenizer.advance();

        // outputFile << "<else>" << endl;

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.getToken()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "{");

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileStatements();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "}");

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        // outputFile << "</else>" << endl;
    }

    // outputFile << "</ifStatement>" << endl;
}

void CompilationEngine::compileExpression()
{
    // outputFile << "<expression>" << endl;

    compileTerm();

    while (tokenizer.isOperator(tokenizer.peek())) {
        tokenizer.advance();
        string op = tokenizer.symbol();

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileTerm();

        if (op == "+") {
            writer.writeArithmetic(OperationTypes::ADD);
        } else if (op == "-") {
            writer.writeArithmetic(OperationTypes::SUB);
        } else if (op == "=") {
            writer.writeArithmetic(OperationTypes::EQ);
        } else if (op == "<") {
            writer.writeArithmetic(OperationTypes::LT);
        } else if (op == ">") {
            writer.writeArithmetic(OperationTypes::GT);
        } else if (op == "&") {
            writer.writeArithmetic(OperationTypes::AND);
        } else if (op == "|") {
            writer.writeArithmetic(OperationTypes::OR);
        } else if (op == "*") {
            writer.writeCall("Math.multiply", 2);
        } else if (op == "/") {
            writer.writeCall("Math.divide", 2);
        }
    }

    // outputFile << "</expression>" << endl;
}

void CompilationEngine::compileTerm()
{
    // outputFile << "<term>" << endl;

    tokenizer.advance();
    if (tokenizer.tokenType() == TokenType::INT_CONST) {
        int val = tokenizer.intVal();
        if (val >= 0) {
            writer.writePush(SegmentTypes::CONST, val);
        } else {
            val *= -1;
            writer.writePush(SegmentTypes::CONST, val);
            writer.writeArithmetic(OperationTypes::NEG);
        }
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.intVal()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::STRING_CONST) {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.stringVal()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::KEYWORD &&
        (tokenizer.keywordType() == KeywordType::TRUE ||
        tokenizer.keywordType() == KeywordType::FALSE ||
        tokenizer.keywordType() == KeywordType::NULL ||
        tokenizer.keywordType() == KeywordType::THIS)) {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.getToken()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        if (tokenizer.peek() == "[") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpression();

            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
        }

        if (tokenizer.peek() == "(") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpressionList();

            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
        }

        if (tokenizer.peek() == ".") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.identifier()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpressionList();

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
        }
    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == "(") {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileExpression();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.isUnaryOperator(tokenizer.getToken())) {
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileTerm();
    }

    // outputFile << "</term>" << endl;
}

void CompilationEngine::compileExpressionList()
{
    // outputFile << "<expressionList>" << endl;

    while (true) {
        if (tokenizer.peek() == ")") {
            break;
        }

        compileExpression();

        if (tokenizer.peek() == ",") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
        }

        ++argCount;
    }

    // outputFile << "</expressionList>" << endl;
}
