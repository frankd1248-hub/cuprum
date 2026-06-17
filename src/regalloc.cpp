#include "regalloc.h"

std::vector<LiveInterval> RegAlloc::computeIntervals(const IRFunction& fn) {
    std::unordered_map<int, LiveInterval> intervals;

    auto touch = [&](const IRValue& val, int idx, bool isDef) {
        if (val.kind != IRValue::Kind::Temp) return;
        int id = val.id;
        if (!intervals.count(id))
            intervals[id] = { id, idx, idx };
        if (isDef)
            intervals[id].start = std::min(intervals[id].start, idx);
        else
            intervals[id].end = std::max(intervals[id].end, idx);
    };

    for (int i = 0; i < (int)fn.body.size(); i++) {
        const IRInstruction& instr = fn.body[i];
        touch(instr.dest, i, true);
        touch(instr.src1, i, false);
        touch(instr.src2, i, false);
        for (const IRValue& arg : instr.args)
            touch(arg, i, false);
    }

    std::vector<LiveInterval> result;
    for (auto& [id, iv] : intervals)
        result.push_back(iv);
    std::sort(result.begin(), result.end(),
        [](const LiveInterval& a, const LiveInterval& b) {
            return a.start < b.start;
        });

    return result;
}

std::unordered_map<int, std::string> RegAlloc::linearScan(std::vector<LiveInterval>& intervals, const IRFunction& fn) {

    std::unordered_map<int, std::string> regMap;
    std::vector<std::string> pool = {
        "r10", "r11", "r12", "r13", "r14", "r15"
    }; // Scratch registers

    static const std::vector<std::string> callerSaved = { "r10", "r11" };
    static const std::vector<std::string> calleeSaved = { "r12", "r13", "r14", "r15" };

    auto spansCall = [&](const LiveInterval& iv) {
        for (int i = iv.start; i <= iv.end; i++)
            if (fn.body[i].op == IROp::Call) return true;
        return false;
    };
    
    std::vector<std::string> free = pool;

    struct ActiveEntry { int end; int tempId; std::string reg; };
    std::vector<ActiveEntry> active;

    for (LiveInterval& iv : intervals) {
        active.erase(
            std::remove_if(active.begin(), active.end(),
                [&](const ActiveEntry& e) {
                    if (e.end < iv.start) {
                        free.push_back(e.reg);
                        return true;
                    }
                    return false;
                }),
            active.end());

        if (free.empty()) {
            
            auto it = std::max_element(active.begin(), active.end(),
                [](const ActiveEntry& a, const ActiveEntry& b) {
                    return a.end < b.end;
                });

            if (it != active.end() && it->end > iv.end) {
                regMap[iv.tempId] = it->reg;
                regMap.erase(it->tempId); // spill it
                active.erase(it);
                active.push_back({ iv.end, iv.tempId, regMap[iv.tempId] });
            }
            
        } else {
            bool callSpanning = spansCall(iv);

            std::string reg;
            bool found = false;

            const auto& preferred = callSpanning ? calleeSaved : callerSaved;
            const auto& fallback  = callSpanning ? callerSaved : calleeSaved;

            for (const auto& pool : { preferred, fallback }) {
                for (const std::string& r : pool) {
                    auto it = std::find(free.begin(), free.end(), r);
                    if (it != free.end()) {
                        reg = r;
                        free.erase(it);
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }

            if (!found) {
                continue;
            }

            regMap[iv.tempId] = reg;
            active.push_back({ iv.end, iv.tempId, reg });
            std::sort(active.begin(), active.end(),
                [](const ActiveEntry& a, const ActiveEntry& b) {
                    return a.end < b.end;
                });
        }
    }

    return regMap;
}