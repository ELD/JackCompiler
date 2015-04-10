#ifndef _JACK_TOKENIZER_H_
#define _JACK_TOKENIZER_H_

#include "includes.h"

using TokenDeque = deque<pair<string,string>>;

class JackTokenizer {
    public:
        JackTokenizer(string&);
        bool hasMoreTokens() { return tokens.size() > 0 ? true : false; }
        const TokenDeque& getTokens() const { return tokens; }
    private:
        void tokenize();
        void stripComments(string&);
        void stripWhitespace(string&);

        ifstream file;
        TokenDeque tokens;
};

#endif
