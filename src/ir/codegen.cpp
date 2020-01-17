#include "codegen.hpp"

ZLLVMIRGenerator::ZLLVMIRGenerator(ZParser::ProgramContext* ctx) {
    this->module = new llvm::Module::Module("main", GlobCtx);
    this->root = ctx;
}

antlrcpp::Any ZLLVMIRGenerator::codeGen() {
    return this->visitProgram(this->root);
}

llvm::BasicBlock* ZLLVMIRGenerator::currentBlock() {
    return this->blocks.top();
}

void ZLLVMIRGenerator::pushBlock(llvm::BasicBlock *block) {
    this->blocks.push(block);
}

llvm::BasicBlock* ZLLVMIRGenerator::popBlock() {
    llvm::BasicBlock *top = this->blocks.top();
    this->blocks.pop();
    return top;
}

std::map<std::string, symtab_t> ZLLVMIRGenerator::getFunctionLocals() {
    return this->func_locals;
}