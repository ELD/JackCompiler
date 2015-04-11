#ifndef _KEYWORDS_H_
#define _KEYWORDS_H_
#undef NULL

enum class KeywordType : unsigned short {
    CLASS = 0,
    METHOD,
    FUNCTION,
    CONSTRUCTOR,
    INT,
    BOOLEAN,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    NULL,
    THIS
};

std::ostream& operator<<(std::ostream&, KeywordType const&);

#endif
