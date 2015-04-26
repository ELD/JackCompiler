#include "../headers/VMWriter.hpp"

VMWriter::VMWriter(ostream& out) : vmOut(out)
{
}

void VMWriter::writePush(SegmentTypes const& segmentType, int const index)
{
    string segment;
    switch (segmentType) {
        case SegmentTypes::CONST:
            segment = "constant";
            break;
        case SegmentTypes::ARG:
            segment = "argument";
            break;
        case SegmentTypes::LOCAL:
            segment = "local";
            break;
        case SegmentTypes::STATIC:
            segment = "static";
            break;
        case SegmentTypes::THIS:
            segment = "this";
            break;
        case SegmentTypes::THAT:
            segment = "that";
            break;
        case SegmentTypes::POINTER:
            segment = "pointer";
            break;
        case SegmentTypes::TEMP:
            segment = "temp";
            break;
    }

    vmOut << "push " << segment << " " << index << endl;
}

void VMWriter::writeFunction(string const& functionName, int const numLocals)
{
    vmOut << "function " << functionName << " " << numLocals << endl;
}

void VMWriter::writeReturn()
{
    vmOut << "return" << endl;
}
