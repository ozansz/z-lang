#include "codegen.hpp"

ZLLVMIRGenerator::ZLLVMIRGenerator(ZParser::ProgramContext* ctx) {
    this->module = new llvm::Module::Module("main", GlobCtx);
    this->root = ctx;
    this->debugLevel = ZIRGEN_NODEBUG;
}

antlrcpp::Any ZLLVMIRGenerator::codeGen() {
    return this->visitProgram(this->root);
}

llvm::BasicBlock* ZLLVMIRGenerator::currentBlock() {
    return this->symtabs.top()->block;
}

void ZLLVMIRGenerator::pushBlock(llvm::BasicBlock *block) {
    this->symtabs.push(new SymbolTable());
    this->symtabs.top()->block = block;
}

llvm::BasicBlock* ZLLVMIRGenerator::popBlock() {
    llvm::BasicBlock *top = this->symtabs.top()->block;
    this->symtabs.pop();
    return top;
}

std::map<std::string, llvm::Value*>& ZLLVMIRGenerator::getLocals() {
    return this->symtabs.top()->locals;
}

llvm::Type* ZLLVMIRGenerator::GetIntegerType() {
    return llvm::Type::getInt64Ty(GlobCtx);
}

llvm::Type* ZLLVMIRGenerator::GetArrayType(uint64_t elem_count) {
    return llvm::ArrayType::get(this->GetIntegerType(), elem_count);
}

void ZLLVMIRGenerator::AbortWithError(std::string errmsg) {
    std::cout << std::endl << "[!] ERR: " << errmsg << std::endl;
    abort();
}

void ZLLVMIRGenerator::DebugMsg(std::string msg) {
    if (this->debugLevel > 0)
        std::cout << "[D] " << msg << std::endl;
}

void ZLLVMIRGenerator::SetDebugLevel(int level) {
    this->debugLevel = level;
}