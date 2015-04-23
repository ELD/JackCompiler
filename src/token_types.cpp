#include "../headers/includes.hpp"

ostream& operator<<(ostream& os, TokenType const& tokenType)
{
    switch (tokenType) {
        case TokenType::KEYWORD:
            os << "keyword";
            break;
        case TokenType::SYMBOL:
            os << "symbol";
            break;
        case TokenType::IDENTIFIER:
            os << "identifier";
            break;
        case TokenType::INT_CONST:
            os << "integerConstant";
            break;
        case TokenType::STRING_CONST:
            os << "stringConstant";
            break;
        default:
            os << "unknown";
            break;
    }

    return os;
}
