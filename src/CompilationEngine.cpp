#include "../headers/CompilationEngine.hpp"

CompilationEngine::CompilationEngine(istream& input, ostream& output) : tokenizer(input), symbolTable(), writer(output), className(""), currentSubroutineName(""), argCount(0)
{
    compileClass();
}

void CompilationEngine::compileClass()
{
    // Opening the class

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::CLASS);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    className = tokenizer.identifier();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

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
}

void CompilationEngine::compileClassVarDec()
{
    SymbolTypes symbolKind;
    string symbolType;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC);

    if (tokenizer.keywordType() == KeywordType::FIELD) {
        symbolKind = SymbolTypes::FIELD;
    } else {
       symbolKind = SymbolTypes::STATIC;
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolType = tokenizer.getToken();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), symbolType, symbolKind);

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        symbolTable.define(tokenizer.identifier(), symbolType, symbolKind);
    }

}

void CompilationEngine::compileSubroutine()
{
    // Reset the symbol table for another subroutine
    symbolTable.startSubroutine();
    KeywordType subroutineType;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::METHOD ||
        tokenizer.keywordType() == KeywordType::FUNCTION ||
        tokenizer.keywordType() == KeywordType::CONSTRUCTOR);
    subroutineType = tokenizer.keywordType();
    if (subroutineType == KeywordType::METHOD) {
        symbolTable.define("this", "this", SymbolTypes::ARG);
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    currentSubroutineName = tokenizer.identifier();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    compileParameterList();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

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

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), type, SymbolTypes::ARG);

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ",") {
        tokenizer.advance();
        compileParameterList();
    }
}

void CompilationEngine::compileVarDec()
{
    string type;

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.keywordType() == KeywordType::VAR);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
    type = tokenizer.getToken();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    symbolTable.define(tokenizer.identifier(), type, SymbolTypes::VAR);

    while (true) {
        tokenizer.advance();
        if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
            break;
        }

        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == ",");

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
        symbolTable.define(tokenizer.identifier(), type, SymbolTypes::VAR);
    }
}

void CompilationEngine::compileStatements()
{
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
}

void CompilationEngine::compileDo()
{
    string subroutineClassName, subroutineName, type;
    bool localCall = false;
    argCount = 0;

    tokenizer.advance();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    subroutineName = tokenizer.identifier();

    if (tokenizer.peekTokenType() == TokenType::SYMBOL && tokenizer.peek() == ".") {
        subroutineClassName = tokenizer.identifier();
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

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

    compileExpressionList();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

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
}

void CompilationEngine::compileLet()
{
    string varName;
    bool dereference = false;

    tokenizer.advance();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::IDENTIFIER);
    varName = tokenizer.identifier();

    if (tokenizer.peek() == "[") {
        dereference = true;
        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

        compileExpression();
        writer.writePush(memorySegmentFromSymbolTypes(symbolTable.kindOf(varName)), symbolTable.indexOf(varName));
        writer.writeArithmetic(OperationTypes::ADD);
        writer.writePop(SegmentTypes::TEMP, 0);

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "]");

    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "=");

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
}

void CompilationEngine::compileWhile()
{
    int whileCount = writer.getWhileCounter();
    stringstream whileLabels;
    whileLabels << "WHILE_EXP" << whileCount;
    string whileExpLabel = whileLabels.str();
    whileLabels.clear();
    whileLabels.str("");
    whileLabels << "WHILE_END" << whileCount;
    string whileEndLabel = whileLabels.str();
    writer.incrementWhileCounter();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::KEYWORD);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    writer.writeLabel(whileExpLabel);

    compileExpression();

    writer.writeArithmetic(OperationTypes::NOT);
    writer.writeIf(whileEndLabel);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    writer.writeGoto(whileExpLabel);
    writer.writeLabel(whileEndLabel);

}

void CompilationEngine::compileReturn()
{
    tokenizer.advance();

    if (tokenizer.peek() != ";") {
       compileExpression();
    } else {
        writer.writePush(SegmentTypes::CONST, 0);
    }

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    writer.writeReturn();

}

void CompilationEngine::compileIf()
{
    int ifElseCount = writer.getIfElseCounter();
    stringstream ifElseLabels;
    ifElseLabels << "IF_FALSE" << ifElseCount;
    string ifFalseLabel = ifElseLabels.str();
    ifElseLabels.clear();
    ifElseLabels.str("");
    ifElseLabels << "IF_END" << ifElseCount;
    string ifEndLabel = ifElseLabels.str();
    writer.incrementIfElseCounter();

    tokenizer.advance();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "(");

    compileExpression();
    writer.writeArithmetic(OperationTypes::NOT);

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == ")");

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "{");

    writer.writeIf(ifFalseLabel);

    compileStatements();

    tokenizer.advance();
    BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
    BOOST_ASSERT(tokenizer.symbol() == "}");

    writer.writeGoto(ifEndLabel);

    writer.writeLabel(ifFalseLabel);
    if (tokenizer.peek() == "else") {
        tokenizer.advance();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "{");

        compileStatements();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);
        BOOST_ASSERT(tokenizer.symbol() == "}");
    }

    writer.writeLabel(ifEndLabel);
}

void CompilationEngine::compileExpression()
{
    compileTerm();

    while (tokenizer.isOperator(tokenizer.peek())) {
        tokenizer.advance();
        string op = tokenizer.symbol();

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
}

void CompilationEngine::compileTerm()
{
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
    } else if (tokenizer.tokenType() == TokenType::STRING_CONST) {
        writer.writeStringConstant(tokenizer.stringVal());
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
    } else if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
        string subroutineClassName, subroutineName, type;
        bool subroutineCall = false;
        bool localCall = true;

        if (tokenizer.peek() == "(") {
            argCount = 0;
            subroutineCall = true;
            subroutineName = tokenizer.identifier();
            tokenizer.advance();

            writer.writePush(SegmentTypes::POINTER, 0);
            ++argCount;

            compileExpressionList();

            tokenizer.advance();

        }

        if (tokenizer.peek() == ".") {
            argCount = 0;
            subroutineCall = true;
            subroutineClassName = tokenizer.identifier();
            tokenizer.advance();
            localCall = false;

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

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

            compileExpressionList();

            tokenizer.advance();
            BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

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

            compileExpression();

            writer.writeArithmetic(OperationTypes::ADD);
            writer.writePop(SegmentTypes::POINTER, 1);
            writer.writePush(SegmentTypes::THAT, 0);

            tokenizer.advance();
        }
    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == "(") {
        compileExpression();

        tokenizer.advance();
        BOOST_ASSERT(tokenizer.tokenType() == TokenType::SYMBOL);

    } else if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.isUnaryOperator(tokenizer.getToken())) {
        compileTerm();
        writer.writeArithmetic(OperationTypes::NOT);
    }
}

void CompilationEngine::compileExpressionList()
{
    while (true) {
        if (tokenizer.peek() == ")") {
            break;
        }

        compileExpression();
        ++argCount;

        if (tokenizer.peek() == ",") {
            tokenizer.advance();
        }
    }
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
