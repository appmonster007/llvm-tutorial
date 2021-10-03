#ifndef LLVM_TRANSFORMS_MY_NEW_PASS_H
#define LLVM_TRANSFORMS_MY_NEW_PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {

class MyNewPass : public PassInfoMixin<MyNewPass> {
public:
    static char ID;

    PreservedAnalyses run(
        Function &F,
        FunctionAnalysisManager &FAM
    );
    static bool isRequired() { return true; }
};

}

#endif 
