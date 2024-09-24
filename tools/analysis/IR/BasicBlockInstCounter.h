
#ifndef BASIC_BLOCK_COUNTER_H
#define BASIC_BLOCK_COUNTER_H

#include <stdint.h>

#include "llvm/ADT/MapVector.h"
#include "llvm/IR/AbstractCallSite.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace llvm;

typedef struct
{
    Function* func;
    uint32_t totalInsts;
    uint32_t totalBBs;
} BBICountResult;


//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
// using BBICountResult = llvm::MapVector<const llvm::Function*, tuple<uint32_t, uint32_t>>;

struct BasicBlockInstCounter : public llvm::AnalysisInfoMixin<BasicBlockInstCounter>
{
    using Result = BBICountResult;
    Result run(llvm::Function &F, llvm::FunctionAnalysisManager &);
    static bool isRequired() { return true; }

private:
    // A special type used by analysis passes to provide an address that
    // identifies that particular analysis pass type.
    static llvm::AnalysisKey Key;
    friend struct llvm::AnalysisInfoMixin<BasicBlockInstCounter>;
};

//------------------------------------------------------------------------------
// New PM interface for the printer pass
//------------------------------------------------------------------------------
class BasicBlockInstCounterPrinter
    : public llvm::PassInfoMixin<BasicBlockInstCounterPrinter>
{
public:
    explicit BasicBlockInstCounterPrinter(llvm::raw_ostream &OutS) : OS(OutS) {}
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &);
    // Part of the official API:
    //  https://llvm.org/docs/WritingAnLLVMNewPMPass.html#required-passes
    static bool isRequired() { return true; }

private:
    llvm::raw_ostream &OS;
};

#endif // BASIC_BLOCK_COUNTER_H
