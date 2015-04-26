#include "../headers/SymbolTable.hpp"

SymbolTable::SymbolTable() : classVarCounter(0), localVarCounter(0), argVarCounter(0)
{
}

void SymbolTable::startSubroutine()
{
    if (localVars.size() > 0) {
        localVars.clear();
    }

    localVarCounter = 0;
    argVarCounter = 0;
}

void SymbolTable::define(string const& name, string const& type, SymbolTypes const& kind)
{
    if (kind == SymbolTypes::STATIC || kind == SymbolTypes::FIELD) {
        classVars.emplace(name, make_tuple(type, kind, classVarCounter));
        ++classVarCounter;
    } else {
        if (kind == SymbolTypes::ARG) {
            localVars.emplace(name, make_tuple(type, kind, argVarCounter));
            ++argVarCounter;
        } else {
            localVars.emplace(name, make_tuple(type, kind, localVarCounter));
            ++localVarCounter;
        }
    }
}

int SymbolTable::varCount(SymbolTypes const& kind)
{
    int counter = 0;

    if (kind == SymbolTypes::FIELD || kind == SymbolTypes::STATIC) {
        for (auto const& kv : classVars) {
            if (get<1>(kv.second) == kind) {
                ++counter;
            }
        }
    } else {
        for (auto const& kv : localVars) {
            if (get<1>(kv.second) == kind) {
                ++counter;
            }
        }
    }

    return counter;
}

SymbolTypes SymbolTable::kindOf(string const& name)
{
    auto element = localVars.find(name);

    if (element != localVars.end()) {
        return get<1>(element->second);
    }

    element = classVars.find(name);

    if (element != classVars.end()) {
        return get<1>(element->second);
    }

    return SymbolTypes::NONE;
}

string SymbolTable::typeOf(string const& name)
{
    auto element = localVars.find(name);

    if (element != localVars.end()) {
       return get<0>(element->second);
    }

    element = classVars.find(name);

    if (element != classVars.end()) {
        return get<0>(element->second);
    }

    return "";
}

int SymbolTable::indexOf(string const& name)
{
    auto element = localVars.find(name);

    if (element != localVars.end()) {
       return get<2>(element->second);
    }

    element = classVars.find(name);

    if (element != classVars.end()) {
        return get<2>(element->second);
    }

    return -1;
}

void SymbolTable::dumpClassVars()
{
    for (auto const& kv : classVars) {
        cout << "Name: " << kv.first;
        cout << "\tType: " << get<0>(kv.second)
            << "\tType: " << get<1>(kv.second)
            << "\tIndex: " << get<2>(kv.second) << endl;
    }
}

void SymbolTable::dumpLocalVars()
{
    cout << "Dumping local vars. Size: " << localVars.size() << endl;
    for (auto const& kv : localVars) {
        cout << "Name: " << kv.first;
        cout << "\tType: " << get<0>(kv.second)
            << "\tType: " << get<1>(kv.second)
            << "\tIndex: " << get<2>(kv.second) << endl;
    }
}
