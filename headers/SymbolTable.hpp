#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include "includes.hpp"

using SymbolMap = map<string, tuple<string, SymbolTypes, int>>;

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    void startSubroutine();
    void define(string const&, string const&, SymbolTypes const&);
    int varCount(SymbolTypes const&);
    SymbolTypes kindOf(string const&);
    string typeOf(string const&);
    int indexOf(string const&);
private:
    void dumpClassVars();
    void dumpLocalVars();

    SymbolMap classVars;
    SymbolMap localVars;

    int classVarCounter;
    int localVarCounter;
};

#endif
