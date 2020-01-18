#include "codegen.hpp"

llvm::Function* ZLLVMIRGenerator::initializePrintfFunction() {
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(GlobCtx));

    llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(GlobCtx),
                                        printf_arg_types, true);
    llvm::Function *func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage,
                                        llvm::Twine("printf"), this->module);
    
    func->setCallingConv(llvm::CallingConv::C);

    return func;
}

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

    // Initialize and link external libc function @printf
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(GlobCtx, Z_PRINTF_FORMATTER);
    llvm::GlobalVariable *format_string = new llvm::GlobalVariable(*this->module, llvm::ArrayType::get(this->GetIntegerType(), 3), true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");

    this->globals[Z_PRINTF_FORMATTER_REPR] = format_string;
    this->globals[Z_PRINTF_LINKAGE_REPR] = this->initializePrintfFunction();

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

antlrcpp::Any ZLLVMIRGenerator::visitPutchCall(ZParser::Function_call_expressionContext *context) {
        llvm::Constant *zero = llvm::Constant::getNullValue(this->GetIntegerType());
        std::vector<llvm::Value *> indices;
        
        indices.push_back(zero);
        indices.push_back(zero);

        llvm::Value *format_str_ref = llvm::GetElementPtrInst::CreateInBounds(
                                        this->globals[Z_PRINTF_FORMATTER_REPR],
                                        indices, "", this->currentBlock());

        std::vector<llvm::Value *> printf_args;
        printf_args.push_back(format_str_ref);
        printf_args.push_back(this->visit(context->function_call_arg_list()->expression()[0]));

        return llvm::CallInst::Create(this->globals[Z_PRINTF_LINKAGE_REPR], printf_args, "", this->currentBlock());
}

//antlrcpp::Any ZLLVMIRGenerator::visitPeekCall(ZParser::Function_call_expressionContext *context) {
//
//}
//
//antlrcpp::Any ZLLVMIRGenerator::visitPopushCall(ZParser::Function_call_expressionContext *context) {
//
//}
//
//antlrcpp::Any ZLLVMIRGenerator::visitArithmeticOp(llvm::Instruction::BinaryOps, std::vector<ZParser::ExpressionContext *> context) {
//
//}

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