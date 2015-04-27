#ifndef _SYMBOL_TYPES_H_
#define _SYMBOL_TYPES_H_

#include "includes.hpp"

enum class SymbolTypes {
    STATIC = 0,
    FIELD,
    ARG,
    VAR,
    NONE
};

std::ostream& operator<<(std::ostream&, SymbolTypes const&);

#endif
