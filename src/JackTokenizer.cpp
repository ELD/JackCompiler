#include "../headers/JackTokenizer.hpp"

VecStr const JackTokenizer::keywords{"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};

string const JackTokenizer::symbols{"{ } ( ) [ ] . , ; + - * / & | < > = ~"};

string const JackTokenizer::operators{"+ - * / | & < > ="};

string const JackTokenizer::unaryOps{"- ~"};

/*********** PUBLIC METHODS ***********/
JackTokenizer::JackTokenizer(istream& input) : file(input), currentToken("")
{
    tokenize();
}

void JackTokenizer::advance()
{
    string token = tokens.front();
    currentToken = token;
    tokens.pop_front();
}

string JackTokenizer::getToken()
{
    return currentToken;
}

string JackTokenizer::peek()
{
    return tokens.front();
}

TokenType JackTokenizer::peekTokenType()
{
    return getTokenType(tokens.front());
}

KeywordType JackTokenizer::peekKeywordType()
{
    return getKeywordType(tokens.front());
}

TokenType JackTokenizer::tokenType()
{
    return getTokenType(currentToken);
}

KeywordType JackTokenizer::keywordType()
{
    return getKeywordType(currentToken);
}

string JackTokenizer::symbol()
{
    auto symbol = currentToken.substr(0, 1);
    if (symbol == ">") {
        symbol = "&gt;";
    } else if (symbol == "<") {
        symbol = "&lt;";
    } else if (symbol == "&") {
        symbol = "&amp;";
    }

    return symbol;
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

bool JackTokenizer::isOperator(string const& token)
{
    if (operators.find(token) != string::npos) {
        return true;
    }

    return false;
}

bool JackTokenizer::isUnaryOperator(string const& token)
{
    if (unaryOps.find(token) != string::npos) {
        return true;
    }

    return false;
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
        boost::char_separator<char> c{" ","~.=()[];,|"};
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

TokenType JackTokenizer::getTokenType(string const& token)
{
    if (find(keywords.begin(), keywords.end(), token) != keywords.end()) {
        return TokenType::KEYWORD;
    } else if (symbols.find(token) != string::npos) {
        return TokenType::SYMBOL;
    } else if (token.find("\"") != string::npos) {
        return TokenType::STRING_CONST;
    } else {
        try {
            stoi(token);
            return TokenType::INT_CONST;
        } catch (invalid_argument exc) {

        }
    }

    return TokenType::IDENTIFIER;
}

KeywordType JackTokenizer::getKeywordType(string const& token)
{
    if (token == "class") {
        return KeywordType::CLASS;
    } else if (token == "method") {
        return KeywordType::METHOD;
    } else if (token == "function") {
        return KeywordType::FUNCTION;
    } else if (token == "constructor") {
        return KeywordType::CONSTRUCTOR;
    } else if (token == "int") {
        return KeywordType::INT;
    } else if (token == "boolean") {
        return KeywordType::BOOLEAN;
    } else if (token == "char") {
        return KeywordType::CHAR;
    } else if (token == "void") {
        return KeywordType::VOID;
    } else if (token == "var") {
        return KeywordType::VAR;
    } else if (token == "static") {
        return KeywordType::STATIC;
    } else if (token == "field") {
        return KeywordType::FIELD;
    } else if (token == "let") {
        return KeywordType::LET;
    } else if (token == "do") {
        return KeywordType::DO;
    } else if (token == "if") {
        return KeywordType::IF;
    } else if (token == "else") {
        return KeywordType::ELSE;
    } else if (token == "while") {
        return KeywordType::WHILE;
    } else if (token == "return") {
        return KeywordType::RETURN;
    } else if (token == "true") {
        return KeywordType::TRUE;
    } else if (token == "false") {
        return KeywordType::FALSE;
    } else if (token == "null") {
        return KeywordType::NULL;
    }

    return KeywordType::THIS;
}
