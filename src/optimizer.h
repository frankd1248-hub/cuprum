#ifndef cuprum_optimizer_h
#define cuprum_optimizer_h

#include <vector>
#include "ir.h"

class Optimizer {
public:
    Optimizer(IRProgram& program) : program_(program) {}
    void optimize();

private:
    IRProgram& program_;

    void optimizeFunction(IRFunction& fn);

    void constantFolding(std::vector<IRInstruction>& body);
    void deadCodeElimination(std::vector<IRInstruction>& body);
    void copyPropagation(std::vector<IRInstruction>& body);
};

#endif