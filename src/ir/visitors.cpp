#include "codegen.hpp"

antlrcpp::Any ZLLVMIRGenerator::visitProgram(ZParser::ProgramContext *context) {
    this->DebugMsg("Generating code for Program");

    llvm::FunctionType *ftype = llvm::FunctionType::get(this->GetIntegerType(), false);
    llvm::Function *start_func = llvm::Function::Create(ftype, llvm::GlobalValue::InternalLinkage, "_start", this->module);
    llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(GlobCtx, "entry", start_func, 0);
    
    this->pushBlock(entry_block);

    // Initialize S and S index as global variables
    llvm::GlobalVariable *S = new llvm::GlobalVariable(*this->module, this->GetArrayType(Z_STACK_SIZE), false,
                                llvm::GlobalVariable::LinkageTypes::InternalLinkage, nullptr, Z_STACK_REPR);
    llvm::Value *S_index = new llvm::GlobalVariable(*this->module, this->GetIntegerType(), false,
                                llvm::GlobalVariable::LinkageTypes::InternalLinkage, nullptr, Z_STACK_INDX_REPR);

    this->globals[Z_STACK_REPR] = S;
    this->globals[Z_STACK_INDX_REPR] = S_index;

    llvm::Value *exprval = this->visit(context->expression());
    llvm::ReturnInst::Create(GlobCtx, exprval, this->currentBlock());
    
    this->popBlock();
}

antlrcpp::Any ZLLVMIRGenerator::visitIDExpr(ZParser::IDExprContext *context) {
    this->DebugMsg("Generating code for IDExpr");

    std::string var_id = context->ID()->getText();

    if (this->getLocals().find(var_id) == this->getLocals().end())
        this->AbortWithError("No such ID in locals: '" + var_id + "' (undeclared variable)");

    llvm::Value *var_ptr = this->getLocals()[var_id];
    return llvm::LoadInst(var_ptr, "", false, this->currentBlock());
}

antlrcpp::Any ZLLVMIRGenerator::visitSExpr(ZParser::SExprContext *context) {

}

//virtual antlrcpp::Any visitFunction_call_expression(ZParser::Function_call_expressionContext *context);
//virtual antlrcpp::Any visitConditional_expression(ZParser::Conditional_expressionContext *context);
//virtual antlrcpp::Any visitConditional_expression_condition(ZParser::Conditional_expression_conditionContext *context);
//virtual antlrcpp::Any visitK_expression(ZParser::K_expressionContext *context);
//virtual antlrcpp::Any visitK_expression_param(ZParser::K_expression_paramContext *context);
//virtual antlrcpp::Any visitZ_expression(ZParser::Z_expressionContext *context);
//virtual antlrcpp::Any visitLambda_decl(ZParser::Lambda_declContext *context);
//virtual antlrcpp::Any visitLambda_param(ZParser::Lambda_paramContext *context);
//virtual antlrcpp::Any visitLambda_body(ZParser::Lambda_bodyContext *context);
//virtual antlrcpp::Any visitConstantSetExpr(ZParser::ConstantSetExprContext *context);
//virtual antlrcpp::Any visitConstantINTExpr(ZParser::ConstantINTExprContext *context);
//virtual antlrcpp::Any visitSet_statement(ZParser::Set_statementContext *context);