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
    void compileVarDec();
    void compileStatements();
    void compileDo();
    // void compileLet();
    // void compileWhile();
    // void compileReturn();
    // void compileIf();
    // void compileExpression();
    // void compileTerm();
    // void compileExpressionList();

    ostream& outputFile;
    JackTokenizer tokenizer;
    string className;
};

#endif
