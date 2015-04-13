#ifndef _COMPILATION_ENGINE_H_
#define _COMPILATION_ENGINE_H_

#include "includes.h"
#include "JackTokenizer.h"

class CompilationEngine {
public:
    CompilationEngine(istream&, ostream&);
private:
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    ostream& outputFile;
    JackTokenizer tokenizer;
    string className;
};

#endif
