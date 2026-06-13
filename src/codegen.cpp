#include "codegen.h"

#include <cstring>

void CodeGen::emit(const std::string& line) {
    out_ << "\t" << line << "\n";
}

void CodeGen::emitLabel(const std::string& label) {
    out_ << label << ":\n";
}

std::string CodeGen::stackRef(int tempId) const {
    return "QWORD PTR [rbp-" + std::to_string(frame_.offsetOf(tempId)) + "]";
}

std::string CodeGen::fStackRef(int tempId) const {
    return "DWORD PTR [rbp-" + std::to_string(frame_.offsetOf(tempId)) + "]";
}

std::string CodeGen::resolve(const IRValue& val) const {
    if (val.kind == IRValue::Kind::Temp)
        return stackRef(val.id);
    else if (val.kind == IRValue::Kind::IntConst) 
        return std::to_string(val.ival);
    else if (val.kind == IRValue::Kind::FloatConst) 
        return std::to_string(val.fval);
    return "?";
}

std::string CodeGen::floatLabel(float fval) {
    if (!floatLabels.count(fval)) {
        std::string label = ".LCf" + std::to_string(floatLabels.size());
        floatLabels[fval] = label;
    }

    return floatLabels[fval];
}

void CodeGen::emitFloatPool() {
    out_ << ".section .rodata\n";
    for (auto& [fval, label] : floatLabels) {
        out_ << label << ":\n";
        
        uint32_t bits;
        memcpy(&bits, &fval, 4);
        out_ << "\t.long " << bits << "\n";
    }
    out_ << ".text\n";
}

void CodeGen::generate(const std::string& outputPath) {
    out_.open(outputPath);
    emitPreamble();
    for (const IRFunction& fn : program_.functions)
        emitFunction(fn);
    emitFloatPool();
    out_.close();
}

void CodeGen::emitPreamble() {
    out_ << ".intel_syntax noprefix\n\n";

    out_ << ".global _start\n";
    emitLabel("_start");
    emit("call\tmain");
    emit("mov\trdi, rax");   // exit code = return value of main
    emit("mov\trax, 60");    // syscall number: exit
    emit("syscall");
    out_ << "\n";
}


void CodeGen::emitFunction(const IRFunction& fn) {
    frame_.reset();
    currentFunc = &fn;

    // Dry-run: allocate every temp
    for (const IRInstruction& instr : fn.body) {
        if (instr.dest.kind == IRValue::Kind::Temp)
            frame_.allocate(instr.dest.id);
    }

    out_ << ".global " << fn.name << "\n";
    emitLabel(fn.name);
    emitPrologue(frame_.frameSize());

    for (const IRInstruction& instr : fn.body)
        emitInstruction(instr);
}

void CodeGen::emitPrologue(int frameSize) {
    emit("push\trbp");
    emit("mov\trbp, rsp");
    if (frameSize > 0)
        emit("sub\trsp, " + std::to_string(frameSize));
}

void CodeGen::emitEpilogue() {
    emit("mov\trsp, rbp");
    emit("pop\trbp");
    emit("ret");
}

void CodeGen::emitInstruction(const IRInstruction& instr) {
    switch (instr.op) {
        case IROp::Const:  emitConst(instr);  break;
        case IROp::FConst: emitFConst(instr); break;
        case IROp::Ret:    emitRet(instr);    break;
        case IROp::Add:
        case IROp::Sub:
        case IROp::Mul:
        case IROp::Div:    emitBinop(instr);  break;
        case IROp::FAdd:
        case IROp::FSub:
        case IROp::FMul:
        case IROp::FDiv:   emitFBinop(instr); break;
        case IROp::Label:  emitLabel(instr.label); break;
        case IROp::Eq:     emitCmp(instr, "sete");  break;
        case IROp::Neq:    emitCmp(instr, "setne"); break;
        case IROp::Less:   emitCmp(instr, "setl");  break;
        case IROp::Leq:    emitCmp(instr, "setle"); break;
        case IROp::Gret:   emitCmp(instr, "setg");  break;
        case IROp::Geq:    emitCmp(instr, "setge"); break;
        case IROp::Not:    emitNot(instr); break;
        case IROp::Jump:   emit("jmp\t" + instr.label); break;
        case IROp::JumpIf:
            emit("cmp\t" + resolve(instr.src1) + ", 0");
            emit("jne\t" + instr.label);
            break;
        case IROp::Call:
            emit("call\t" + instr.label);
            if (instr.dest.kind == IRValue::Kind::Temp)
                emit("mov\t" + stackRef(instr.dest.id) + ", rax");
            break;
        case IROp::Mov:
            emit("mov\trax, " + resolve(instr.src1));
            emit("mov\t" + stackRef(instr.dest.id) + ", rax");
            break;
        case IROp::ToBool:
            emit("mov\trax, " + resolve(instr.src1));
            emit("cmp\trax, 0");
            emit("setne\tal");
            emit("movzx\trax, al");
            emit("mov\t" + stackRef(instr.dest.id) + ", rax");
            break;
        case IROp::ToFloat: emitToFloat(instr); break;
        case IROp::ToInt:   emitToInt(instr); break;
    }
}

void CodeGen::emitConst(const IRInstruction& instr) {
    if (instr.src1.kind == IRValue::Kind::IntConst) {
        if (instr.src1.ival== 0) {
            emit("xor\teax, eax");
        } else {
            emit("mov\trax, " + std::to_string(instr.src1.ival));
        }

        emit("mov\t" + stackRef(instr.dest.id) + ", rax");
    }
}

void CodeGen::emitFConst(const IRInstruction& instr) {
    float fval = instr.src1.fval;
    std::string lbl = floatLabel(fval);
    emit("movss\txmm0, DWORD PTR [" + lbl + " + rip]");
    emit("movq\trax, xmm0");
    emit("mov\t" + stackRef(instr.dest.id) + ", rax");
}

void CodeGen::emitCmp(const IRInstruction& instr, const std::string& setcc) {
    emit("mov\trax, " + resolve(instr.src1));
    emit("mov\trbx, " + resolve(instr.src2));
    emit("cmp\trax, rbx");
    emit(setcc + "\tal");
    emit("movzx\trax, al");
    emit("mov\t" + stackRef(instr.dest.id) + ", rax");
}

void CodeGen::emitNot(const IRInstruction& instr) {
    // !x == (x == 0)
    emit("mov\trax, " + resolve(instr.src1));
    emit("cmp\trax, 0");
    emit("sete\tal");
    emit("movzx\trax, al");
    emit("mov\t" + stackRef(instr.dest.id) + ", rax");
}

void CodeGen::emitRet(const IRInstruction& instr) {
    if (currentFunc->returnType == Type::Float32t) {
        emit("mov\trax, " + stackRef(instr.src1.id));
        emit("movq\txmm0, rax");
    } else {
        emit("mov\trax, " + stackRef(instr.src1.id));
    }
    emitEpilogue();
}

void CodeGen::emitBinop(const IRInstruction& instr) {
    if (instr.src1.kind == IRValue::Kind::Temp)
        emit("mov\trax, " + stackRef(instr.src1.id));
    else if (instr.src1.kind == IRValue::Kind::IntConst)
        emit("mov\trax, " + std::to_string(instr.src1.ival));

    if (instr.src2.kind == IRValue::Kind::Temp)
        emit("mov\trbx, " + stackRef(instr.src2.id));
    else if (instr.src2.kind == IRValue::Kind::IntConst)
        emit("mov\trbx, " + std::to_string(instr.src2.ival));

    switch (instr.op) {
        case IROp::Add: emit("add\trax, rbx"); break;
        case IROp::Sub: emit("sub\trax, rbx"); break;
        case IROp::Mul: emit("imul\trax, rbx"); break;
        case IROp::Div:
            emit("cqo");
            emit("idiv\trbx");
            // quotient is in rax, remainder in rdx
            break;
        default: break;
    }

    emit("mov\t" + stackRef(instr.dest.id) + ", rax");
}

void CodeGen::emitFBinop(const IRInstruction& instr) {
    emit("mov\trax, " + stackRef(instr.src1.id));
    emit("movq\txmm0, rax");
    emit("mov\trax, " + stackRef(instr.src2.id));
    emit("movq\txmm1, rax");

    switch (instr.op) {
        case IROp::FAdd: emit("addss\txmm0, xmm1"); break;
        case IROp::FSub: emit("subss\txmm0, xmm1"); break;
        case IROp::FMul: emit("mulss\txmm0, xmm1"); break;
        case IROp::FDiv: emit("divss\txmm0, xmm1"); break;
        default: break;
    }
    
    emit("movss\t" + fStackRef(instr.dest.id) + ", xmm0");
}

void CodeGen::emitToFloat(const IRInstruction& instr) {
    // i32 → f32
    emit("mov\teax, DWORD PTR " + stackRef(instr.src1.id));
    emit("cvtsi2ss\txmm0, eax");
    emit("movss\t" + fStackRef(instr.dest.id) + ", xmm0");
}

void CodeGen::emitToInt(const IRInstruction& instr) {
    emit("mov\trax, " + stackRef(instr.src1.id));
    emit("movq\txmm0, rax");
    emit("cvttss2si\teax, xmm0");
    emit("mov\t" + stackRef(instr.dest.id) + ", rax");
}