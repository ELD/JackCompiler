#ifndef _COMPILATION_ENGINE_H_
#define _COMPILATION_ENGINE_H_

#include "includes.h"
#include "JackTokenizer.h"

class CompilationEngine {
public:
    CompilationEngine(istream&, ostream&);
    void compileClass();
private:
    ostream& outputFile;
    JackTokenizer tokenizer;
};

#endif
