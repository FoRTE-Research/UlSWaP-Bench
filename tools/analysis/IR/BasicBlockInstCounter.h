#ifndef BASIC_BLOCK_INST_COUNTER_H
#define BASIC_BLOCK_INST_COUNTER_H

#include <stdint.h>

#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

typedef struct
{
    llvm::Function* func;
    uint32_t basicBlockCount;
    uint32_t instructionCount;
} ResultBBICounter;

struct BasicBlockInstCounter : public llvm::AnalysisInfoMixin<BasicBlockInstCounter>
{
    using Result = ResultBBICounter;
    Result run(llvm::Function &F,
               llvm::FunctionAnalysisManager &);

    static bool isRequired() { return true; }

private:
    // A special type used by analysis passes to provide an address that
    // identifies that particular analysis pass type.
    static llvm::AnalysisKey Key;
    friend struct llvm::AnalysisInfoMixin<BasicBlockInstCounter>;
};

class BasicBlockInstCounterPrinter : public llvm::PassInfoMixin<BasicBlockInstCounterPrinter>
{
public:
    explicit BasicBlockInstCounterPrinter(llvm::raw_ostream &OutS) : OS(OutS) {}
    llvm::PreservedAnalyses run(llvm::Function &Func,
                                llvm::FunctionAnalysisManager &FAM);
    static bool isRequired() { return true; }

private:
    llvm::raw_ostream &OS;
};

#endif  // BASIC_BLOCK_INST_COUNTER_H
