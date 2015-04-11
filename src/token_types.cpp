#include "../headers/includes.h"

ostream& operator<<(ostream& os, TokenType const& tokenType)
{
    switch (tokenType) {
        case TokenType::KEYWORD:
            os << "TokenType::KEYWORD";
            break;
        case TokenType::SYMBOL:
            os << "TokenType::SYMBOL";
            break;
        case TokenType::IDENTIFIER:
            os << "TokenType::IDENTIFIER";
            break;
        case TokenType::INT_CONST:
            os << "TokenType::INT_CONST";
            break;
        case TokenType::STRING_CONST:
            os << "TokenType::STRING_CONST";
            break;
        default:
            os << "TokenType::UNKNOWN";
            break;
    }

    return os;
}
