#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
	struct MyLegacyPass : public FunctionPass {
    	static char ID;
    	MyLegacyPass() : FunctionPass(ID) {}

	virtual bool runOnFunction(Function &F) {
    	errs() << "LEGACY PASS:\t I saw a function called " << F.getName() << "\n";
		return false;
    }
  };
}

char MyLegacyPass::ID = 0;

// Below function named X, Y can be anything

static RegisterPass<MyLegacyPass> X("MyLegacyPass", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static RegisterStandardPasses Y(
  PassManagerBuilder::EP_EarlyAsPossible,
  [](const PassManagerBuilder &Builder,
  		legacy::PassManagerBase &PM
	) { 
		PM.add(new MyLegacyPass());
	}
);
