#ifndef _COMPILATION_ENGINE_H_
#define _COMPILATION_ENGINE_H_

#include "includes.hpp"
#include "JackTokenizer.hpp"
#include "SymbolTable.hpp"
#include "VMWriter.hpp"

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
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();

    SegmentTypes memorySegmentFromSymbolTypes(SymbolTypes const&);

    // ostream& outputFile;
    JackTokenizer tokenizer;
    SymbolTable symbolTable;
    VMWriter writer;
    string className, currentSubroutineName;
    int argCount;
};

#endif
