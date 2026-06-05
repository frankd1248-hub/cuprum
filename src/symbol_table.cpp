#include "symbol_table.h"

bool SymbolTable::define(const Symbol& symbol) {
    if (scopes.back().contains(symbol.name)) {
        return false;
    }

    scopes.back().insert({symbol.name, symbol});
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes.at(i).contains(name)) {
            return &scopes.at(i).at(name);
        }
    }

    return nullptr;
}