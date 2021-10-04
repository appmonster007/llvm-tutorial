#include "MyNewPass.h"

using namespace llvm; 

PreservedAnalyses MyNewPass::run(
    Function &F,
    FunctionAnalysisManager &FAM
) {
// Pass code here
    errs() << "NEW PASS:\t I saw a function called " << F.getName() << "\n";
    return PreservedAnalyses::all();
}


/*
Register vectorized start EP Call back
(Effective when using `clang` toolkit for pluging in the pass)
*/
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK

// PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//     return {
//         LLVM_PLUGIN_API_VERSION, "MyNewPass", "v0.1",
//         [](PassBuilder &PB) {
//             PB.registerVectorizerStartEPCallback(
//                 [](
//                     FunctionPassManager &FPM,
//                     PassBuilder::OptimizationLevel O
//                 ) {
//                     FPM.addPass(MyNewPass());
//                   }
//             );
//         }
//     };
// }

/*
Register pipeline parsing Callback
(Effective when using `opt` toolkit for pluging in the pass)
*/
PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MyNewPass", "v0.1",
        [](PassBuilder &PB) {
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

char MyNewPass::ID = 0;