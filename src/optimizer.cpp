#include "optimizer.h"

#include <unordered_set>

void Optimizer::optimize() {
    for (IRFunction& fn : program_.functions)
        optimizeFunction(fn);
}

void Optimizer::optimizeFunction(IRFunction& fn) {
    constantFolding(fn.body);
    deadCodeElimination(fn.body);

    copyPropagation(fn.body);
    deadCodeElimination(fn.body);
}

static bool isSideEffect(IROp op) {
    switch (op) {
        case IROp::Ret:
        case IROp::Jump:
        case IROp::JumpIf:
        case IROp::Label:
        case IROp::Call:
        case IROp::Store:
            return true;
        default:
            return false;
    }
}

void Optimizer::constantFolding(std::vector<IRInstruction>& body) {
}

void Optimizer::deadCodeElimination(std::vector<IRInstruction>& body) {
    std::unordered_set<int> used;

    for (const IRInstruction& instr : body) {
        if (instr.src1.kind == IRValue::Kind::Temp) used.insert(instr.src1.id);
        if (instr.src2.kind == IRValue::Kind::Temp) used.insert(instr.src2.id);
    }

    std::vector<IRInstruction> result;
    for (const IRInstruction& instr : body) {
        bool hasDest = instr.dest.kind == IRValue::Kind::Temp;
        bool destUsed = hasDest && used.count(instr.dest.id);

        if (!hasDest || destUsed || isSideEffect(instr.op))
            result.push_back(instr);
    }

    body = std::move(result);
}

void Optimizer::copyPropagation(std::vector<IRInstruction>& body) {
    std::unordered_map<int, IRValue> aliases;

    std::unordered_map<int, int> defCount;
    for (const IRInstruction& instr : body) {
        if (instr.dest.kind == IRValue::Kind::Temp)
            defCount[instr.dest.id]++;
    }

    for (const IRInstruction& instr : body) {
        if (instr.op == IROp::Mov &&
            instr.dest.kind == IRValue::Kind::Temp &&
            defCount[instr.dest.id] == 1) {
            aliases[instr.dest.id] = instr.src1;
        }
    }

    auto resolve = [&](IRValue val) -> IRValue {
        // chase the alias chain: t2 -> t1 -> t0
        int limit = 32; // prevent infinite loops from cycles
        while (val.kind == IRValue::Kind::Temp &&
               aliases.count(val.id) && limit-- > 0) {
            val = aliases[val.id];
        }
        return val;
    };

    for (IRInstruction& instr : body) {
        instr.src1 = resolve(instr.src1);
        instr.src2 = resolve(instr.src2);
    }
}