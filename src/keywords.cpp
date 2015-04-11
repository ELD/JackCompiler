#include "../headers/includes.h"

ostream& operator<<(ostream& os, KeywordType const& keywords)
{
    switch (keywords) {
        case KeywordType::CLASS:
            os << "KeywordType::CLASS";
            break;

        case KeywordType::METHOD:
            os << "KeywordType::METHOD";
            break;

        case KeywordType::FUNCTION:
            os << "KeywordType::FUNCTION";
            break;

        case KeywordType::CONSTRUCTOR:
            os << "KeywordType::CONSTRUCTOR";
            break;

        case KeywordType::INT:
            os << "KeywordType::INT";
            break;

        case KeywordType::BOOLEAN:
            os << "KeywordType::BOOLEAN";
            break;

        case KeywordType::CHAR:
            os << "KeywordType::CHAR";
            break;

        case KeywordType::VOID:
            os << "KeywordType::VOID";
            break;

        case KeywordType::VAR:
            os << "KeywordType::VAR";
            break;

        case KeywordType::STATIC:
            os << "KeywordType::STATIC";
            break;

        case KeywordType::FIELD:
            os << "KeywordType::FIELD";
            break;

        case KeywordType::LET:
            os << "KeywordType::LET";
            break;

        case KeywordType::DO:
            os << "KeywordType::DO";
            break;

        case KeywordType::IF:
            os << "KeywordType::IF";
            break;

        case KeywordType::ELSE:
            os << "KeywordType::ELSE";
            break;

        case KeywordType::WHILE:
            os << "KeywordType::WHILE";
            break;

        case KeywordType::RETURN:
            os << "KeywordType::RETURN";
            break;

        case KeywordType::TRUE:
            os << "KeywordType::TRUE";
            break;

        case KeywordType::FALSE:
            os << "KeywordType::FALSE";
            break;

        case KeywordType::NULL:
            os << "KeywordType::NULL";
            break;

        case KeywordType::THIS:
            os << "KeywordType::THIS";
            break;
    }
    return os;
}
