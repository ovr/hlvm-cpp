
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Printable.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>

#include <llvm/ADT/APInt.h>

// Main Execute Class
#include <llvm/ExecutionEngine/ExecutionEngine.h>
// Interpreter code
#include <llvm/ExecutionEngine/Interpreter.h>
// Arguments building when call
#include <llvm/ExecutionEngine/GenericValue.h>

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

llvm::Function *createSimpleStrLenFunction(llvm::Module *llvmModule, llvm::LLVMContext &llvmContext) {
    llvm::Function *functionPointer = llvm::cast<llvm::Function>(
        llvmModule->getOrInsertFunction(
            "str_len",
            llvm::Type::getInt32Ty(llvmContext), // 1st Argument
            llvm::Type::getInt32Ty(llvmContext), // Return,
            nullptr
        )
    );


    llvm::Function::arg_iterator argumentsIterator = functionPointer->arg_begin();
    argumentsIterator->setName("Argument1");


    auto *argumentValuePointer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), 1);

    llvm::BasicBlock *returnBasicBlock = llvm::BasicBlock::Create(llvmContext, "return", functionPointer);
    llvm::ReturnInst::Create(llvmContext, argumentValuePointer, returnBasicBlock);

    return functionPointer;
}

int main() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::LLVMContext llvmContext;

    std::unique_ptr<llvm::Module> llvmOwner(new llvm::Module("test", llvmContext));
    llvm::Module *llvmModule = llvmOwner.get();

    llvm::Function *strLenFunctionPointer = createSimpleStrLenFunction(llvmModule, llvmContext);

    std::string errStr;
    llvm::ExecutionEngine *EE = llvm::EngineBuilder(std::move(llvmOwner))
            .setErrorStr(&errStr)
            .create();

    if (!EE) {
        std::cout << "Cannot create ExecutionEngine " << errStr << std::endl;
        return 1;
    }

    std::string type_str;
    llvm::raw_string_ostream verifyError(type_str);

    if (llvm::verifyModule(*llvmModule, &verifyError)) {
        std::cout << "Cannot verify LLVM Module" << std::endl;
        std::cout << verifyError.str() << std::endl;
        return 1;
    }

    std::cout << "Lets call str_len" << std::endl;

    std::vector<llvm::GenericValue> Args(1);
    Args[0].IntVal = llvm::APInt(32, 100500);
    llvm::GenericValue result = EE->runFunction(strLenFunctionPointer, Args);

    std::cout << "Result " << result.IntVal.toString(10, true) << std::endl;

    llvmModule->dump();

    return 0;
}
