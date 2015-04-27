#ifndef _VM_WRITER_H_
#define _VM_WRITER_H_

#include "includes.hpp"

class VMWriter {
public:
    VMWriter(ostream&);
    void writePush(SegmentTypes const&, int const);
    void writePop(SegmentTypes const&, int const);
    void writeArithmetic(OperationTypes const&);
    void writeLabel(string const&);
    void writeGoto(string const&);
    void writeIf(string const&);
    void writeCall(string const&, int const);
    void writeFunction(string const&, int const);
    void writeReturn();
    void writeStringConstant(string const&);
    int getWhileCounter() { return whileCounter; }
    int getIfElseCounter() { return ifElseCounter; }
    void incrementWhileCounter() { ++whileCounter; }
    void incrementIfElseCounter() { ++ifElseCounter; }
private:
    string getSegment(SegmentTypes const&);
    ostream& vmOut;
    int whileCounter;
    int ifElseCounter;
};

#endif
