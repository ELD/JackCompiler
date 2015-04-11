#ifndef _TOKEN_TYPES_H_
#define _TOKEN_TYPES_H_

enum class TokenType : unsigned short {
    KEYWORD = 0,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
};

std::ostream& operator<<(std::ostream&, TokenType const&);

#endif
