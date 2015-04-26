#include "../headers/symbol_types.hpp"

ostream& operator<<(ostream& os, SymbolTypes const& symbolType)
{
    switch (symbolType) {
        case SymbolTypes::STATIC:
            os << "static";
            break;
        case SymbolTypes::FIELD:
            os << "field";
            break;
        case SymbolTypes::ARG:
            os << "arg";
            break;
        case SymbolTypes::VAR:
            os << "var";
            break;
        case SymbolTypes::NONE:
            os << "none";
            break;
    }

    return os;
}
