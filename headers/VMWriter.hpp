#ifndef _VM_WRITER_H_
#define _VM_WRITER_H_

#include "includes.hpp"

class VMWriter {
public:
    VMWriter(ostream&);
    void writePush(SegmentTypes const&, int const);
    void writePop(SegmentTypes const&, int const);
    void writeArithmetic(OperationTypes const&);
    void writeCall(string const&, int const);
    void writeFunction(string const&, int const);
    void writeReturn();
private:
    string getSegment(SegmentTypes const&);
    ostream& vmOut;
};

#endif
