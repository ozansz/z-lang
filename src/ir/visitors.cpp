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
                                llvm::GlobalVariable::LinkageTypes::InternalLinkage,
                                llvm::ConstantInt::get(this->GetIntegerType(), 0), Z_STACK_INDX_REPR);

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
    return new llvm::LoadInst(var_ptr, "", false, this->currentBlock());
}

antlrcpp::Any ZLLVMIRGenerator::visitSExpr(ZParser::SExprContext *context) {
    this->DebugMsg("Generating code for SExpr");

    std::vector<llvm::Value *> indx_vect;

    indx_vect.push_back(this->globals[Z_STACK_INDX_REPR]);
    return llvm::GetElementPtrInst::CreateInBounds(this->globals[Z_STACK_REPR], indx_vect, "", this->currentBlock());
}

antlrcpp::Any ZLLVMIRGenerator::visitFunction_call_expression(ZParser::Function_call_expressionContext *context) {
    std::string func_id = context->function_identifier()->getText();

    this->DebugMsg("Generating code for function_call_expression (" + func_id + ")");

    if (func_id == "'") {
        return this->visitPutchCall(context);
    } else if (func_id == "^") {
        return this->visitPeekCall(context);
    } else if (func_id == "<<") {
        return this->visitPopushCall(context);
    } else if (func_id == "+") {
        return this->visitArithmeticOp(llvm::Instruction::Add, context->function_call_arg_list()->expression());
    } else if (func_id == "-") {
        return this->visitArithmeticOp(llvm::Instruction::Sub, context->function_call_arg_list()->expression());
    } else if (func_id == "*") {
        return this->visitArithmeticOp(llvm::Instruction::Mul, context->function_call_arg_list()->expression());
    } else if (func_id == "/") {
        return this->visitArithmeticOp(llvm::Instruction::SDiv, context->function_call_arg_list()->expression());
    } else {
        llvm::Function *function = this->module->getFunction(func_id);
        
        if (function == NULL)
            this->AbortWithError("No such function: '" + func_id + "' (undeclared function)");

        std::vector<llvm::Value*> args;
        std::vector<ZParser::ExpressionContext *>::const_iterator it;
        
        for (it = context->function_call_arg_list()->expression().begin(); it != context->function_call_arg_list()->expression().end(); it++) {
            args.push_back(this->visit(*it));
        }

        return llvm::CallInst::Create(function, args, "", this->currentBlock());
    }
}

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