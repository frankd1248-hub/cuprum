#ifndef ferrum_symbol_table_h
#define ferrum_symbol_table_h

#include "common.h"
#include "token.h"
#include "ast.h"

struct Symbol {
    std::string name;
    Type        type;
    bool        isFunction  = false;
    bool        isConst = false;
    std::vector<Type> paramTypes; // empty if not a function
    Token       declToken;        // for "previously declared at..." messages
};

class SymbolTable {
public:
    SymbolTable() : scopes({}) { }

    void pushScope() {
        scopes.push_back({});
    }

    void popScope() {
        scopes.pop_back();
    }

    bool define(const Symbol&);     // false if already defined in current scope
    Symbol* lookup(const std::string& name); // walks stack outward, nullptr if not found
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
};

#endif