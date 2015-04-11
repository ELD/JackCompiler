#ifndef _TOKEN_TYPES_H_
#define _TOKEN_TYPES_H_

enum class TokenType : unsigned short {
    KEYWORD = 0,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
};

inline std::ostream& operator<<(std::ostream& os, TokenType const& tokenType)
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

#endif
