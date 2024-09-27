#include "BasicBlockInstCounter.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

static void printBasicBlockInstCounterResult(llvm::raw_ostream &, const ResultBBICounter &OC);

llvm::AnalysisKey BasicBlockInstCounter::Key;

BasicBlockInstCounter::Result BasicBlockInstCounter::run(llvm::Function &Func, llvm::FunctionAnalysisManager &)
{
    ResultBBICounter count = {&Func, 0, 0};
    for (auto &BB : Func)
    {
        count.basicBlockCount++;
        for (auto &Inst : BB)
        {
            count.instructionCount++;
        }
    }
    return count;
}

PreservedAnalyses BasicBlockInstCounterPrinter::run(Function &Func, FunctionAnalysisManager &FAM)
{
    auto &BBICount = FAM.getResult<BasicBlockInstCounter>(Func);

    printBasicBlockInstCounterResult(OS, BBICount);
    return PreservedAnalyses::all();
}

llvm::PassPluginLibraryInfo getBasicBlockInstCounterPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "BasicBlockInstCounter", LLVM_VERSION_STRING,
        [](PassBuilder &PB)
        {
            // #1 REGISTRATION FOR "opt -passes=print-basic-block-inst-counter"
            // Register BasicBlockInstCounterPrinter so that it can be used when
            // specifying pass pipelines with `-passes=`.
            PB.registerPipelineParsingCallback(
                [&](StringRef Name, FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>)
                {
                    if (Name == "print-basic-block-inst-counter")
                    {
                        FPM.addPass(BasicBlockInstCounterPrinter(llvm::errs()));
                        return true;
                    }
                    return false;
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

static void printBasicBlockInstCounterResult(raw_ostream &OutS,
                                             const ResultBBICounter &BBICount)
{
    OutS << BBICount.func->getName() << "," << BBICount.basicBlockCount << ","
         << BBICount.instructionCount << "\n";
}
