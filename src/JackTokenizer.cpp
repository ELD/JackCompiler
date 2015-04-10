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

        boost::char_separator<char> c {" ",".=()[];"};
        boost::tokenizer<boost::char_separator<char>> tizer{line, c};
        for (const auto& t : tizer) {
            // cout << t << endl;
            tokens.emplace_back(make_pair("test", t));
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
