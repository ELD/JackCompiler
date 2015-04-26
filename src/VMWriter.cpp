#include "../headers/VMWriter.hpp"

VMWriter::VMWriter(ostream& out) : vmOut(out), whileCounter(0), ifElseCounter(0)
{
}

void VMWriter::writePush(SegmentTypes const& segmentType, int const index)
{
    vmOut << "push " << getSegment(segmentType) << " " << index << endl;
}

void VMWriter::writePop(SegmentTypes const& segmentType, int const index)
{
    vmOut << "pop " << getSegment(segmentType) << " " << index << endl;
}

void VMWriter::writeArithmetic(OperationTypes const& operation)
{
    vmOut << operation << endl;
}

void VMWriter::writeLabel(string const& label)
{
    vmOut << "label " <<  label << endl;
}

void VMWriter::writeGoto(string const& label)
{
    vmOut << "goto " << label << endl;
}

void VMWriter::writeIf(string const& label)
{
    vmOut << "if-goto " << label << endl;
}

void VMWriter::writeCall(string const& fnName, int const numArgs)
{
    vmOut << "call " << fnName << " " << numArgs << endl;
}

void VMWriter::writeFunction(string const& functionName, int const numLocals)
{
    vmOut << "function " << functionName << " " << numLocals << endl;
}

void VMWriter::writeReturn()
{
    vmOut << "return" << endl;
}

void VMWriter::writeStringConstant(string const& stringLiteral)
{
    writePush(SegmentTypes::CONST, static_cast<int>(stringLiteral.size()));
    writeCall("String.new", 1);

    for (char const& c : stringLiteral) {
        writePush(SegmentTypes::CONST, static_cast<int>(c));
        writeCall("String.appendChar", 2);
    }
}

string VMWriter::getSegment(SegmentTypes const& segmentType)
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

    return segment;
}
