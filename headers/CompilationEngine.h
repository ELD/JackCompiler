#ifndef _COMPILATION_ENGINE_H_
#define _COMPILATION_ENGINE_H_

#include "includes.h"
#include "JackTokenizer.h"

class CompilationEngine {
public:
    CompilationEngine(istream&, ostream&);
private:
    ostream& outputFile;
    JackTokenizer tokenizer;
};

#endif
