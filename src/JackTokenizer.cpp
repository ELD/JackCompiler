#include "../headers/JackTokenizer.h"

/*********** PUBLIC METHODS ***********/
JackTokenizer::JackTokenizer(istream& input) : file(input), currentToken("")
{
    tokenize();
}

string JackTokenizer::nextToken()
{
    string token = tokens.front();
    tokens.pop_front();
    currentToken = token;
    return token;
}

string JackTokenizer::peekAhead()
{
    return tokens.front();
}

TokenType JackTokenizer::tokenType()
{
    vector<string> keywords{"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};
    string symbols{"{ } ( ) [ ] . , ; + - * / & | < > = ~"};
    if (find(keywords.begin(), keywords.end(), currentToken) != keywords.end()) {
        return TokenType::KEYWORD;
    } else if (symbols.find(currentToken) != string::npos) {
        return TokenType::SYMBOL;
    } else if (currentToken.find("\"") != string::npos) {
        return TokenType::STRING_CONST;
    } else {
        try {
            stoi(currentToken);
            return TokenType::INT_CONST;
        } catch (invalid_argument exc) {

        }
    }

    return TokenType::IDENTIFIER;
}

KeywordType JackTokenizer::keywordType()
{
    if (currentToken == "class") {
        return KeywordType::CLASS;
    } else if (currentToken == "method") {
        return KeywordType::METHOD;
    } else if (currentToken == "function") {
        return KeywordType::FUNCTION;
    } else if (currentToken == "consructor") {
        return KeywordType::CONSTRUCTOR;
    } else if (currentToken == "int") {
        return KeywordType::INT;
    } else if (currentToken == "boolean") {
        return KeywordType::BOOLEAN;
    } else if (currentToken == "char") {
        return KeywordType::CHAR;
    } else if (currentToken == "void") {
        return KeywordType::VOID;
    } else if (currentToken == "var") {
        return KeywordType::VAR;
    } else if (currentToken == "static") {
        return KeywordType::STATIC;
    } else if (currentToken == "field") {
        return KeywordType::FIELD;
    } else if (currentToken == "let") {
        return KeywordType::LET;
    } else if (currentToken == "do") {
        return KeywordType::DO;
    } else if (currentToken == "if") {
        return KeywordType::IF;
    } else if (currentToken == "else") {
        return KeywordType::ELSE;
    } else if (currentToken == "while") {
        return KeywordType::WHILE;
    } else if (currentToken == "return") {
        return KeywordType::RETURN;
    } else if (currentToken == "true") {
        return KeywordType::TRUE;
    } else if (currentToken == "false") {
        return KeywordType::FALSE;
    } else if (currentToken == "null") {
        return KeywordType::NULL;
    }

    return KeywordType::THIS;
}

char JackTokenizer::symbol()
{
    return currentToken[0];
}

string JackTokenizer::identifier()
{
    return currentToken;
}

int JackTokenizer::intVal()
{
    int val;
    try {
        val = stoi(currentToken);
    } catch (invalid_argument exc) {
        val = -1;
    }

    return val;
}

string JackTokenizer::stringVal()
{
    auto firstQuote = currentToken.find_first_of("\"");
    auto lastQuote = currentToken.find_last_of("\"");

    return currentToken.substr(firstQuote + 1, lastQuote - 1);
}

/*********** PRIVATE METHODS ***********/
void JackTokenizer::tokenize()
{

    string line;
    bool multilineComment = false;
    while (getline(file, line)) {
        // If comments exist, strip them
        stripComments(line);
        stripWhitespace(line);

        auto multilineCommentLoc = line.find("/*");
        if (multilineCommentLoc != string::npos || multilineComment) {
            multilineComment = true;
            auto endMultilineCommentLoc = line.find("*/");
            if (endMultilineCommentLoc != string::npos) {
                multilineComment = false;
            }

            continue;
        }

        bool inStringLiteral{false};
        string stringLiteral{""};
        boost::char_separator<char> c{" ","~.=()[];,"};
        boost::tokenizer<boost::char_separator<char>> tizer{line, c};
        for (const auto& t : tizer) {
            auto stringLiteralLoc = t.find("\"");
            if (stringLiteralLoc != string::npos && !inStringLiteral) {
                inStringLiteral = true;
                stringLiteral += t;
                continue;
            } else if (stringLiteralLoc != string::npos && inStringLiteral) {
                inStringLiteral = false;
                stringLiteral += " " + t;
                tokens.emplace_back(stringLiteral);
                continue;
            } else if (inStringLiteral) {
                stringLiteral += " " + t;
                continue;
            }

            tokens.emplace_back(t);
        }
    }
}

void JackTokenizer::stripComments(string& line)
{
    auto commentLoc = line.find("//");
    if (commentLoc != string::npos) {
        line = line.substr(0, commentLoc);
    }
}

void JackTokenizer::stripWhitespace(string& line)
{
    trim_all(line);
}
