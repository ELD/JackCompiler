#include "../headers/JackTokenizer.h"

JackTokenizer::JackTokenizer(string& input) : file(input)
{
    tokenize();
}

void JackTokenizer::tokenize()
{

    string line;
    bool multilineComment = false;
    while (getline(file, line)) {
        // If comments exist, strip them
        stripComments(line);
        stripWhitespace(line);

        auto multilineCommentLoc = line.find("/*");
        if (multilineCommentLoc != string::npos) {
            multilineComment = true;
            auto endMultilineCommentLoc = line.find("*/");
            if (endMultilineCommentLoc != string::npos) {
                multilineComment = false;
            }

            continue;
        }

        bool inStringLiteral{false};
        string stringLiteral{""};
        boost::char_separator<char> c{" ",".=()[];,"};
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
