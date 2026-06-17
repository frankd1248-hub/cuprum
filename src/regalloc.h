#ifndef cuprum_regalloc_h
#define cuprum_regalloc_h

#include "ir.h"
#include <unordered_map>
#include <string>

struct LiveInterval {
    int tempId;
    int start;   // index of first definition
    int end;     // index of last use
};

class RegAlloc {
public:
    RegAlloc() { }

    // Returns: map from temp id → register name for allocated temps.
    std::unordered_map<int, std::string> allocate(const IRFunction& fn) {
        std::vector<LiveInterval> intervals = computeIntervals(fn);
        return linearScan(intervals, fn);
    }

private:
    std::vector<LiveInterval> computeIntervals(const IRFunction& fn);

    std::unordered_map<int, std::string> linearScan(std::vector<LiveInterval>& intervals, const IRFunction& fn);

    // Available integer registers (excluding rax, arg regs near calls)
    std::vector<std::string> freeRegs;

    // Active intervals — sorted by end point, for expire-old-intervals
    std::vector<LiveInterval*> active;

    // Mapping register name to currently assigned interval
    std::unordered_map<std::string, LiveInterval*> regToInterval;
};

#endif