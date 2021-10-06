#include "MyNewPass.h"

using namespace llvm; 

char MyNewPass::ID = 0;

PreservedAnalyses MyNewPass::run(
    Function &F,
    FunctionAnalysisManager &FAM
) {
// Pass code here
    errs() << "NEW PASS:\t I saw a function called " << F.getName() << "\n";
    return PreservedAnalyses::all();
}

/* Register pass for callback */
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MyNewPass", "v0.1",
        [](PassBuilder &PB) {
            /*
            Register vectorized start EP Call back
            (Effective when using `clang` toolkit for pluging in the pass)
            
            Register a callback for a default optimizer pipeline extension point
            This extension point allows adding optimization passes before the
            vectorizer and other highly target specific optimization passes are
            executed. 
            */
            PB.registerVectorizerStartEPCallback(
                [](
                    FunctionPassManager &FPM,
                    PassBuilder::OptimizationLevel O
                ) {
                    FPM.addPass(MyNewPass());
                  }
            );

            /*
            Register pipeline parsing Callback
            (Effective when using `opt` toolkit for pluging in the pass)
            
            Register pipeline parsing callbacks with this pass builder instance.
            Using these callbacks, callers can parse both a single pass name, 
            as well as entire sub-pipelines, and populate the PassManager instance accordingly.
            */
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "MyNewPass") {
                        FPM.addPass(MyNewPass());
                        return true;
                    }
                  return false;
                }
            );
        }
    };
}
