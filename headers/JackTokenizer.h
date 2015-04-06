#ifndef _JACK_TOKENIZER_H_
#define _JACK_TOKENIZER_H_

#include "includes.h"

class JackTokenizer {
    public:
        JackTokenizer(istream&);
        const vector<string>& getTokens() const { return tokens; }
    private:
        //void stripComments();
        //void Onelineify(string&);

        vector<string> tokens;
};

#endif
