#ifndef _JACK_TOKENIZER_H_
#define _JACK_TOKENIZER_H_

#include "includes.hpp"

using VecStr = vector<string>;

using TokenDeque = deque<string>;

class JackTokenizer {
    public:
        JackTokenizer(istream&);
        bool hasMoreTokens() { return tokens.size() > 0 ? true : false; }
        void advance();
        string getToken();
        string peek();
        TokenType peekTokenType();
        KeywordType peekKeywordType();
        TokenType tokenType();
        KeywordType keywordType();
        string symbol();
        string identifier();
        int intVal();
        string stringVal();
        const TokenDeque& getTokens() const { return tokens; }
    private:
        void tokenize();
        void stripComments(string&);
        void stripWhitespace(string&);
        TokenType getTokenType(string const&);
        KeywordType getKeywordType(string const&);

        istream& file;
        TokenDeque tokens;
        string currentToken;
        static VecStr const keywords;
        static string const symbols;
};

#endif
