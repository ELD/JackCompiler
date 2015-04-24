#include "../headers/includes.hpp"

ostream& operator<<(ostream& os, KeywordType const& keywords)
{
    switch (keywords) {
        case KeywordType::CLASS:
            os << "class";
            break;

        case KeywordType::METHOD:
            os << "method";
            break;

        case KeywordType::FUNCTION:
            os << "function";
            break;

        case KeywordType::CONSTRUCTOR:
            os << "constructor";
            break;

        case KeywordType::INT:
            os << "int";
            break;

        case KeywordType::BOOLEAN:
            os << "boolean";
            break;

        case KeywordType::CHAR:
            os << "char";
            break;

        case KeywordType::VOID:
            os << "void";
            break;

        case KeywordType::VAR:
            os << "var";
            break;

        case KeywordType::STATIC:
            os << "static";
            break;

        case KeywordType::FIELD:
            os << "field";
            break;

        case KeywordType::LET:
            os << "let";
            break;

        case KeywordType::DO:
            os << "do";
            break;

        case KeywordType::IF:
            os << "if";
            break;

        case KeywordType::ELSE:
            os << "else";
            break;

        case KeywordType::WHILE:
            os << "while";
            break;

        case KeywordType::RETURN:
            os << "return";
            break;

        case KeywordType::TRUE:
            os << "true";
            break;

        case KeywordType::FALSE:
            os << "false";
            break;

        case KeywordType::NULL:
            os << "null";
            break;

        case KeywordType::THIS:
            os << "this";
            break;
    }
    return os;
}
