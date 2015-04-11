#ifndef _JACK_TOKENIZER_H_
#define _JACK_TOKENIZER_H_

#include "includes.h"

using TokenDeque = deque<string>;

class JackTokenizer {
    public:
        JackTokenizer(istream&);
        bool hasMoreTokens() { return tokens.size() > 0 ? true : false; }
        string nextToken();
        string peekAhead();
        TokenType tokenType();
        KeywordType keywordType();
        char symbol();
        string identifier();
        int intVal();
        string stringVal();
        const TokenDeque& getTokens() const { return tokens; }
    private:
        void tokenize();
        void stripComments(string&);
        void stripWhitespace(string&);
        void getTokenType(string&);

        istream& file;
        TokenDeque tokens;
        string currentToken;
};

#endif
