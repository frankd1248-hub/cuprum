#ifndef ferrum_codegen_h
#define ferrum_codegen_h

#include <fstream>
#include <unordered_map>
#include "common.h"
#include "ir.h"

// Tracks each temp's stack slot (offset from rbp, e.g. -8, -16, ...)
class StackFrame {
public:
    // Assigns the next available slot to tempId. Returns the offset.
    int allocate(int tempId) {
        if (offsets.contains(tempId)) {
            return offsets.at(tempId);
        }

        int offset = nextOffset;
        offsets[tempId] = offset;
        nextOffset += 8;
        return offset;
    }

    // Returns the rbp offset for a previously allocated temp.
    int offsetOf(int tempId) const {
        return offsets.at(tempId);
    }

    // Total bytes needed — rounded up to 16-byte alignment.
    int frameSize() const {
        int size = nextOffset - 8; // nextOffset starts at 8
        return (size + 15) & ~15;   // round up to nearest 16
    }

    void reset() {
        offsets.clear();
        nextOffset = 8;
    }

private:
    std::unordered_map<int, int> offsets;
    int nextOffset = 8; // first slot is [rbp-8]
};

class CodeGen {
public:
    CodeGen(IRProgram& program) : program_(program) {}

    // Entry point. Writes the full .s file to outputPath.
    void generate(const std::string& outputPath);

private:
    IRProgram& program_;
    std::ofstream out_;
    StackFrame frame_;

    // File-level preamble: .intel_syntax + _start stub
    void emitPreamble();

    // Lowers one IRFunction.
    void emitFunction(const IRFunction& fn);

    // push rbp / mov rbp, rsp / sub rsp, N
    void emitPrologue(int frameSize);

    // mov rsp, rbp / pop rbp / ret
    void emitEpilogue();

    // Dispatches a single instruction.
    void emitInstruction(const IRInstruction& instr);

    void emitConst (const IRInstruction& instr); // dest = constant
    void emitCmp   (const IRInstruction& instr, const std::string& setcc);
    void emitNot   (const IRInstruction& instr);
    void emitRet   (const IRInstruction& instr); // return src1
    void emitBinop (const IRInstruction& instr); // dest = src1 op src2

    // "QWORD PTR [rbp-N]" for a temp id.
    std::string stackRef(int tempId) const;

    // Resolves an IRValue to either a stack ref or an immediate string.
    std::string resolve(const IRValue& val) const;

    // Emits a line with a leading tab.
    void emit(const std::string& line);

    // Emits a label (no leading tab).
    void emitLabel(const std::string& label);
};

#endif