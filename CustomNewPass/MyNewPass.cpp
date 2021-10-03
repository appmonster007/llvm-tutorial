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