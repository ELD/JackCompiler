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
    KeywordType subroutineType;

    // outputFile << "<subroutineDec>" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::METHOD ||
        tokenizer.keywordType() == KeywordType::FUNCTION ||
        tokenizer.keywordType() == KeywordType::CONSTRUCTOR);
    subroutineType = tokenizer.keywordType();
    if (subroutineType == KeywordType::METHOD) {
        symbolTable.define("this", "this", SymbolTypes::ARG);
    }


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
        if (subroutineType == KeywordType::CONSTRUCTOR) {
            int memCount = symbolTable.varCount(SymbolTypes::FIELD);
            writer.writePush(SegmentTypes::CONST, memCount);
            writer.writeCall("Memory.alloc", 1);
            writer.writePop(SegmentTypes::POINTER, 0);
        } else if (subroutineType == KeywordType::METHOD) {
            writer.writePush(SegmentTypes::ARG, 0);
            writer.writePop(SegmentTypes::POINTER, 0);
        }
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
            writer.writePush(memorySegmentFromSymbolTypes(segment), offset);
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
    string varName;
    bool dereference = false;
    // outputFile << "<letStatement>" << endl;

    tokenizer.advance();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.getToken()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    varName = tokenizer.identifier();

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.identifier()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    if (tokenizer.peek() == "[") {
        dereference = true;
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.symbol()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        compileExpression();
        writer.writePush(memorySegmentFromSymbolTypes(symbolTable.kindOf(varName)), symbolTable.indexOf(varName));
        writer.writeArithmetic(OperationTypes::ADD);
        writer.writePop(SegmentTypes::TEMP, 0);

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

    if (dereference) {
        writer.writePush(SegmentTypes::TEMP, 0);
        writer.writePop(SegmentTypes::POINTER, 1);
        writer.writePop(SegmentTypes::THAT, 0);
    } else {
        writer.writePop(memorySegmentFromSymbolTypes(symbolTable.kindOf(varName)), symbolTable.indexOf(varName));
    }

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    // outputFile << "</letStatement>" << endl;
}

void CompilationEngine::compileWhile()
{
    int whileCount = writer.getWhileCounter();
    string whileExpLabel = (stringstream() << "WHILE_EXP" << whileCount).str();
    string whileEndLabel = (stringstream() << "WHILE_END" << whileCount).str();
    writer.incrementWhileCounter();
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

    writer.writeLabel(whileExpLabel);

    compileExpression();

    writer.writeArithmetic(OperationTypes::NOT);
    writer.writeIf(whileEndLabel);

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

    writer.writeGoto(whileExpLabel);
    writer.writeLabel(whileEndLabel);

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
    int ifElseCount = writer.getIfElseCounter();
    string ifFalseLabel = (stringstream() << "IF_FALSE" << ifElseCount).str();
    string ifEndLabel = (stringstream() << "IF_END" << ifElseCount).str();
    writer.incrementIfElseCounter();
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
    writer.writeArithmetic(OperationTypes::NOT);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    writer.writeIf(ifFalseLabel);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    writer.writeGoto(ifEndLabel);

    // outputFile << "<" << tokenizer.tokenType() << ">"
    //     << tokenizer.symbol()
    //     << "</" << tokenizer.tokenType() << ">" << endl;

    writer.writeLabel(ifFalseLabel);
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

    writer.writeLabel(ifEndLabel);

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
        } else if (op == "&lt;") {
            writer.writeArithmetic(OperationTypes::LT);
        } else if (op == "&gt;") {
            writer.writeArithmetic(OperationTypes::GT);
        } else if (op == "&amp;") {
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
        writer.writeStringConstant(tokenizer.stringVal());
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.stringVal()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::KEYWORD &&
        (tokenizer.keywordType() == KeywordType::TRUE ||
        tokenizer.keywordType() == KeywordType::FALSE ||
        tokenizer.keywordType() == KeywordType::NULL ||
        tokenizer.keywordType() == KeywordType::THIS)) {
            if (tokenizer.keywordType() == KeywordType::TRUE) {
                writer.writePush(SegmentTypes::CONST, 0);
                writer.writeArithmetic(OperationTypes::NOT);
            } else if (tokenizer.keywordType() == KeywordType::FALSE) {
                writer.writePush(SegmentTypes::CONST, 0);
            } else if (tokenizer.keywordType() == KeywordType::THIS) {
                writer.writePush(SegmentTypes::POINTER, 0);
            } else {
                writer.writePush(SegmentTypes::CONST, 0);
            }
        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.getToken()
        //     << "</" << tokenizer.tokenType() << ">" << endl;
    } else if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        string subroutineClassName, subroutineName, type;
        bool subroutineCall = false;
        bool localCall = true;

        // outputFile << "<" << tokenizer.tokenType() << ">"
        //     << tokenizer.identifier()
        //     << "</" << tokenizer.tokenType() << ">" << endl;

        if (tokenizer.peek() == "(") {
            argCount = 0;
            subroutineCall = true;
            subroutineName = tokenizer.identifier();
            tokenizer.advance();

            writer.writePush(SegmentTypes::POINTER, 0);
            ++argCount;

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
            argCount = 0;
            subroutineCall = true;
            subroutineClassName = tokenizer.identifier();
            tokenizer.advance();
            localCall = false;

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
                writer.writePush(memorySegmentFromSymbolTypes(segment), offset);
                ++argCount;
            }

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

        if (subroutineCall && localCall) {
            writer.writeCall(className + "." + subroutineName, argCount);
            argCount = 0;
        } else if (subroutineCall) {
            if (!type.empty()) {
                writer.writeCall(type + "." + subroutineName, argCount);
            } else {
                writer.writeCall(subroutineClassName + "." + subroutineName, argCount);
            }
            argCount = 0;
        } else {
            writer.writePush(memorySegmentFromSymbolTypes(symbolTable.kindOf(tokenizer.identifier())), symbolTable.indexOf(tokenizer.identifier()));
        }

        if (tokenizer.peek() == "[") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;

            compileExpression();

            writer.writeArithmetic(OperationTypes::ADD);
            writer.writePop(SegmentTypes::POINTER, 1);
            writer.writePush(SegmentTypes::THAT, 0);

            tokenizer.advance();

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
        writer.writeArithmetic(OperationTypes::NOT);
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
        ++argCount;

        if (tokenizer.peek() == ",") {
            tokenizer.advance();

            // outputFile << "<" << tokenizer.tokenType() << ">"
            //     << tokenizer.symbol()
            //     << "</" << tokenizer.tokenType() << ">" << endl;
        }
    }

    // outputFile << "</expressionList>" << endl;
}

SegmentTypes CompilationEngine::memorySegmentFromSymbolTypes(SymbolTypes const& type)
{
    if (type == SymbolTypes::STATIC) {
        return SegmentTypes::STATIC;
    } else if (type == SymbolTypes::FIELD) {
        return SegmentTypes::THIS;
    } else if (type == SymbolTypes::ARG) {
        return SegmentTypes::ARG;
    } else {
        return SegmentTypes::LOCAL;
    }
}
