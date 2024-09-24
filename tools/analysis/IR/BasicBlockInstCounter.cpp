#include "BasicBlockInstCounter.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

static void printCountResult(raw_ostream &OutS, const BBICountResult &CountMap);

//------------------------------------------------------------------------------
// BasicBlockInstCounter Implementation
//------------------------------------------------------------------------------
PreservedAnalyses BasicBlockInstCounterPrinter::run(Function &Func, FunctionAnalysisManager &FAM)
{
    auto &Count = FAM.getResult<BasicBlockInstCounter>(Func);

    printCountResult(OS, Count);
    return PreservedAnalyses::all();
}

BasicBlockInstCounter::Result BasicBlockInstCounter::run(Function &Func, FunctionAnalysisManager &FAM)
{
    BBICountResult Count;
    uint32_t TotalInsts = 0;
    uint32_t TotalBBs = 0;

    for (auto &BB : Func)
    {
        TotalBBs++;
        for (auto &I : BB)
        {
            TotalInsts++;
        }
    }

    Count = {&Func, TotalInsts, TotalBBs};
    return Count;
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getBasicBlockInstCounterPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "BasicBlockInstCounter", LLVM_VERSION_STRING, [](PassBuilder &PB)
        {
            // #1 REGISTRATION FOR "opt -passes=print<opcode-counter>"
            // Register BasicBlockInstCounterPrinter so that it can be used when
            // specifying pass pipelines with `-passes=`.
            PB.registerPipelineParsingCallback([&](StringRef Name, FunctionPassManager &FPM,
                                                   ArrayRef<PassBuilder::PipelineElement>)
                                               {
                    if (Name == "print-basic-block-inst-counter")
                    {
                        FPM.addPass(BasicBlockInstCounterPrinter(llvm::errs()));
                        return true;
                    }
                    return false; });
            // #2 REGISTRATION FOR "-O{1|2|3|s}"
            // Register BasicBlockInstCounterPrinter as a step of an existing pipeline.
            // The insertion point is specified by using the
            // 'registerVectorizerStartEPCallback' callback. To be more precise,
            // using this callback means that BasicBlockInstCounterPrinter will be called
            // whenever the vectoriser is used (i.e. when using '-O{1|2|3|s}'.
            PB.registerVectorizerStartEPCallback(
                [](llvm::FunctionPassManager &PM,
                   llvm::OptimizationLevel Level)
                {
                    PM.addPass(BasicBlockInstCounterPrinter(llvm::errs()));
                });
            // #3 REGISTRATION FOR "FAM.getResult<BasicBlockInstCounter>(Func)"
            // Register BasicBlockInstCounter as an analysis pass. This is required so that
            // BasicBlockInstCounterPrinter (or any other pass) can request the results
            // of BasicBlockInstCounter.
            PB.registerAnalysisRegistrationCallback(
                [](FunctionAnalysisManager &FAM)
                {
                    FAM.registerPass([&]
                                     { return BasicBlockInstCounter(); });
                });
        }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return getBasicBlockInstCounterPluginInfo();
}

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------
static void printCountResult(raw_ostream &OutS, const BBICountResult &CountMap)
{
    OutS << "Function: " << CountMap.func->getName() << "\n";
    OutS << "  Total Instructions: " << CountMap.totalInsts << "\n";
    OutS << "  Total Basic Blocks: " << CountMap.totalBBs << "\n";
    OutS << "\n";
}