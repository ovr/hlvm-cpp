
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

// y own library with ast and this file for all classes
#include "ast.h"
#include "ast/Statement/Function.h"

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
    llvm::Value *argument1 = llvm::cast<llvm::Value>(argumentsIterator++); // Black magic box, with casting

    llvm::BasicBlock *returnBasicBlock = llvm::BasicBlock::Create(llvmContext, "return", functionPointer);
    llvm::ReturnInst::Create(llvmContext, argument1, returnBasicBlock);

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

    std::cout << std::endl << "IR Code" << std::endl;

    std::string codeIR;
    llvm::raw_string_ostream codeIRStream(codeIR);

    codeIRStream << *llvmModule;
    std::cout << codeIRStream.str() << std::endl;

    /**
     * function rand() { // what return?
     *  return; // return nothing is not allowed in LLVM?
     * }
     */

    auto statements = std::vector<HLVM::AST::Node>();
    statements.push_back(HLVM::AST::Statement::Return());

    auto astFunction = HLVM::AST::Statement::Function(
        "rand",
        statements
    );

    return 0;
}
