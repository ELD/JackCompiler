#include "../headers/includes.hpp"

std::ostream& operator<<(std::ostream& os, OperationTypes const& operation)
{
    switch (operation) {
        case OperationTypes::ADD:
            os << "add";
            break;
        case OperationTypes::SUB:
            os << "sub";
            break;
        case OperationTypes::NEG:
            os << "neg";
            break;
        case OperationTypes::EQ:
            os << "eq";
            break;
        case OperationTypes::GT:
            os << "gt";
            break;
        case OperationTypes::LT:
            os << "lt";
            break;
        case OperationTypes::AND:
            os << "and";
            break;
        case OperationTypes::OR:
            os << "or";
            break;
        case OperationTypes::NOT:
            os << "not";
            break;
    }

    return os;
}
