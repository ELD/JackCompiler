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
        tokenizer.advance();
        // ClassVarDec and Subroutine Decs
        if (tokenizer.peekTokenType() == TokenType::KEYWORD &&
            (tokenizer.peekKeywordType() == KeywordType::FIELD || tokenizer.peekKeywordType() == KeywordType::STATIC)) {
            // compileClassVarDec();
        } else if (tokenizer.peekTokenType() == TokenType::KEYWORD &&
            (tokenizer.peekKeywordType() == KeywordType::FUNCTION ||
                tokenizer.peekKeywordType() == KeywordType::METHOD ||
                tokenizer.peekKeywordType() == KeywordType::CONSTRUCTOR)) {
            // compileSubroutine();
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

    outputFile << "<symbol>"
        << tokenizer.symbol()
        << "</symbol>" << endl;
    outputFile << "</class>";
}

// void CompilationEngine::compileClassVarDec()
// {
//     outputFile << "<classVarDec>" << endl;

//     while (true) {
//         assert(tokenizer.tokenType() == TokenType::KEYWORD);
//         assert(tokenizer.keywordType() == KeywordType::FIELD || tokenizer.keywordType() == KeywordType::STATIC);

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.keywordType()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         tokenizer.advance();
//         assert(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.getToken()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         tokenizer.advance();
//         assert(tokenizer.tokenType() == TokenType::IDENTIFIER);

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.identifier()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         while (true) {
//             tokenizer.advance();
//             if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
//                 outputFile << "<" << tokenizer.tokenType() << ">"
//                     << tokenizer.symbol()
//                     << "</" << tokenizer.tokenType() << ">" << endl;
//                 break;
//             }

//             assert(tokenizer.tokenType() == TokenType::SYMBOL);
//             assert(tokenizer.symbol() == ",");
//             outputFile << "<" << tokenizer.tokenType() << ">"
//                 << tokenizer.symbol()
//                 << "</" << tokenizer.tokenType() << ">" << endl;

//             tokenizer.advance();
//             assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//             outputFile << "<" << tokenizer.tokenType() << ">"
//                 << tokenizer.identifier()
//                 << "</" << tokenizer.tokenType() << ">" << endl;
//         }

//         break;
//     }

//     outputFile << "</classVarDec>" << endl;
// }

// void CompilationEngine::compileSubroutine()
// {
//     outputFile << "<subroutineDec>" << endl;

//     assert(tokenizer.tokenType() == TokenType::KEYWORD);
//     assert(tokenizer.keywordType() == KeywordType::METHOD ||
//         tokenizer.keywordType() == KeywordType::FUNCTION ||
//         tokenizer.keywordType() == KeywordType::CONSTRUCTOR);

//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.keywordType()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     tokenizer.advance();
//     if (tokenizer.tokenType() == TokenType::IDENTIFIER) {
//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.identifier()
//             << "</" << tokenizer.tokenType() << ">" << endl;
//     } else {
//         assert(tokenizer.tokenType() == TokenType::KEYWORD);
//         assert(tokenizer.keywordType() == KeywordType::INT ||
//             tokenizer.keywordType() == KeywordType::CHAR ||
//             tokenizer.keywordType() == KeywordType::BOOLEAN ||
//             tokenizer.keywordType() == KeywordType::VOID);

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.keywordType()
//             << "</" << tokenizer.tokenType() << ">" << endl;
//     }

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::IDENTIFIER);

//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.identifier()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == "(");

//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.symbol()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     compileParameterList();

//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == ")");

//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.symbol()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == "{");

//     outputFile << "<subroutineBody>" << endl;

//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.symbol()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     while (true) {
//         tokenizer.advance();
//         cout << tokenizer.getToken() << endl;
//         if (tokenizer.tokenType() == TokenType::SYMBOL) {
//             outputFile << "<" << tokenizer.tokenType() << ">"
//                 << tokenizer.symbol()
//                 << "</" << tokenizer.tokenType() << ">" << endl;
//             break;
//         }

//         assert(tokenizer.tokenType() == TokenType::KEYWORD);

//         if (tokenizer.keywordType() == KeywordType::VAR) {
//             compileVarDec();
//         } else {
//             compileStatements();
//         }
//     }

//     outputFile << "</subroutineBody>" << endl;

//     outputFile << "</subroutineDec>" << endl;
// }

// void CompilationEngine::compileParameterList()
// {
//     outputFile << "<parameterList>" << endl;

//     while (true) {
//         tokenizer.advance();
//         if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ")") {
//             break;
//         }

//         assert(tokenizer.tokenType() == TokenType::KEYWORD);
//         assert(tokenizer.keywordType() == KeywordType::INT ||
//             tokenizer.keywordType() == KeywordType::CHAR ||
//             tokenizer.keywordType() == KeywordType::BOOLEAN);

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.keywordType()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         tokenizer.advance();
//         assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.identifier()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         tokenizer.advance();
//         if (tokenizer.symbol() != ",") {
//             break;
//         }

//         assert(tokenizer.tokenType() == TokenType::SYMBOL);
//         assert(tokenizer.symbol() == ",");
//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.symbol()
//             << "</" << tokenizer.tokenType() << ">" << endl;
//     }

//     outputFile << "</parameterList>" << endl;
// }

// void CompilationEngine::compileVarDec()
// {
//     outputFile << "<varDec>" << endl;

//     assert(tokenizer.keywordType() == KeywordType::VAR);
//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.keywordType()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::KEYWORD || tokenizer.tokenType() == TokenType::IDENTIFIER);
//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.getToken()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//     outputFile << "<" << tokenizer.tokenType() << ">"
//         << tokenizer.identifier()
//         << "</" << tokenizer.tokenType() << ">" << endl;

//     while (true) {
//         tokenizer.advance();
//         if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ";") {
//             outputFile << "<" << tokenizer.tokenType() << ">"
//                 << tokenizer.symbol()
//                 << "</" << tokenizer.tokenType() << ">" << endl;
//             break;
//         }

//         assert(tokenizer.tokenType() == TokenType::SYMBOL);
//         assert(tokenizer.symbol() == ",");

//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.symbol()
//             << "</" << tokenizer.tokenType() << ">" << endl;

//         tokenizer.advance();
//         assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//         outputFile << "<" << tokenizer.tokenType() << ">"
//             << tokenizer.identifier()
//             << "</" << tokenizer.tokenType() << ">" << endl;
//     }

//     outputFile << "</varDec>" << endl;
// }

// void CompilationEngine::compileStatements()
// {
//     outputFile << "<statements>" << endl;
//     assert(tokenizer.tokenType() == TokenType::KEYWORD);

//     if (tokenizer.keywordType() == KeywordType::DO) {
//         compileDo();
//     } else if (tokenizer.keywordType() == KeywordType::LET) {
//         // compileLet();
//     } else if (tokenizer.keywordType() == KeywordType::WHILE) {
//         // compileWhile();
//     } else if (tokenizer.keywordType() == KeywordType::RETURN) {
//         compileReturn();
//     } else if (tokenizer.keywordType() == KeywordType::IF) {
//         // compileIf();
//     }
//     outputFile << "</statements>" << endl;
// }

// void CompilationEngine::compileDo()
// {
//     outputFile << "<doStatement>" << endl;
//     outputFile << "<keyword>"
//         << tokenizer.getToken()
//         << "</keyword>" << endl;

//     tokenizer.advance();

//     assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//     outputFile << "<identifier>"
//         << tokenizer.identifier()
//         << "</identifier>" << endl;

//     tokenizer.advance();

//     if (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ".") {
//         assert(tokenizer.tokenType() == TokenType::SYMBOL);
//         outputFile << "<symbol>"
//             << tokenizer.symbol()
//             << "</symbol>" << endl;

//         tokenizer.advance();

//         assert(tokenizer.tokenType() == TokenType::IDENTIFIER);
//         outputFile << "<identifier>"
//             << tokenizer.identifier()
//             << "</identifier>" << endl;
//     }

//     tokenizer.advance();

//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == "(");

//     outputFile << "<symbol>"
//         << tokenizer.symbol()
//         << "</symbol>" << endl;

//     compileParameterList();
//     // compileExpressionList();

//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == ")");
//     outputFile << "<symbol>"
//         << tokenizer.symbol()
//         << "</symbol>" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::SYMBOL);
//     assert(tokenizer.symbol() == ";");
//     outputFile << "<symbol>"
//         << tokenizer.symbol()
//         << "</symbol>" << endl;

//     outputFile << "</doStatement>" << endl;
// }

// void CompilationEngine::compileLet()
// {
//     outputFile << "<letStatement>" << endl;

//     outputFile << "<keyword>"
//         << tokenizer.getToken()
//         << "</keyword>" << endl;

//     tokenizer.advance();
//     assert(tokenizer.tokenType() == TokenType::IDENTIFIER);

//     outputFile << "<identifier>"
//         << tokenizer.identifier()
//         << "</identifier>" << endl;

//     tokenizer.advance();
//     if (tokenizer.tokenType() == TokenType::SYMBOL) {
//         assert(tokenizer.symbol() == "[");
//         outputFile << "<symbol>"
//             << tokenizer.symbol()
//             << "</symbol>" << endl;

//         while (true) {
//             tokenizer.advance();
//             if (tokenizer.symbol() == "]") {
//                 break;
//             }
//             compileExpression();
//         }
//     }

//     outputFile << "</letStatement>" << endl;
// }

// void CompilationEngine::compileReturn()
// {
//     outputFile << "<returnStatement>" << endl;

//     outputFile << "<keyword>"
//         << tokenizer.getToken()
//         << "</keyword>" << endl;

//     tokenizer.advance();

//     if (tokenizer.tokenType() == TokenType::SYMBOL) {
//         outputFile << "<symbol>"
//             << tokenizer.symbol()
//             << "</symbol>" << endl;
//     } else {
//         // compileExpression();
//     }

//     outputFile << "</returnStatement>" << endl;
// }

// void CompilationEngine::compileExpression()
// {

// }
