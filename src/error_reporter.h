#ifndef ferrum_error_reporter_h
#define ferrum_error_reporter_h

#include "common.h"
#include "token.h"

#include <print>

class ErrorReporter {
public:

    ErrorReporter(std::ostream& out) : out(out) { }

    void report(int line, int col, const std::string& msg) {
        std::print(out, "At line: {:03d}, col {:02d}: {}\n", line, col, msg);
    }

    void report(Token where, const std::string& msg) {
        report(where.line, where.col, msg);
    }

private:
    std::ostream& out;
};

#endif