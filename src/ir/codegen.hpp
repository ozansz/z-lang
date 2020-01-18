#include <stack>
#include <map>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/None.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/iterator.h>
#include <llvm/ADT/iterator_range.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/OperandTraits.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Use.h>
#include <llvm/IR/User.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/AtomicOrdering.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/ErrorHandling.h>

#include "../antlr4-runtime/ZVisitor.h"

#define ZIRGEN_NODEBUG  0
#define ZIRGEN_DEBUG  1

#define Z_STACK_SIZE    128
#define Z_STACK_REPR    "_S"
#define Z_STACK_INDX_REPR   "_S_index"

#define Z_PRINTF_LINKAGE_REPR   "printf"
#define Z_PRINTF_FORMATTER  "%c"
#define Z_PRINTF_FORMATTER_REPR "_printf_char_formatter"

static llvm::LLVMContext GlobCtx;

class SymbolTable {
public:
    llvm::BasicBlock *block;
    std::map<std::string, llvm::Value*> locals;
};

class ZLLVMIRGenerator : public ZVisitor {
    ZParser::ProgramContext* root;
    std::stack<SymbolTable *> symtabs;
    std::map<std::string, llvm::Value *> globals;
    llvm::Module *module;
    int debugLevel;

public:
    ZLLVMIRGenerator(ZParser::ProgramContext* ctx);

    antlrcpp::Any codeGen();
    
    llvm::BasicBlock *currentBlock();
    void pushBlock(llvm::BasicBlock *block);
    llvm::BasicBlock *popBlock();
    std::map<std::string, llvm::Value*>& getLocals();

    llvm::Type *GetIntegerType();
    llvm::Type *GetArrayType(uint64_t elem_count);

    void AbortWithError(std::string);
    void DebugMsg(std::string);
    void SetDebugLevel(int);

    antlrcpp::Any visitPutchCall(ZParser::Function_call_expressionContext *context);
    antlrcpp::Any visitPeekCall(ZParser::Function_call_expressionContext *context);
    antlrcpp::Any visitPopushCall(ZParser::Function_call_expressionContext *context);
    antlrcpp::Any visitArithmeticOp(llvm::Instruction::BinaryOps, std::vector<ZParser::ExpressionContext *> context);

    virtual antlrcpp::Any visitProgram(ZParser::ProgramContext *context);
    virtual antlrcpp::Any visitIDExpr(ZParser::IDExprContext *context);
    virtual antlrcpp::Any visitSExpr(ZParser::SExprContext *context);
    virtual antlrcpp::Any visitFunction_call_expression(ZParser::Function_call_expressionContext *context);
    virtual antlrcpp::Any visitConditional_expression(ZParser::Conditional_expressionContext *context);
    virtual antlrcpp::Any visitConditional_expression_condition(ZParser::Conditional_expression_conditionContext *context);
    virtual antlrcpp::Any visitK_expression(ZParser::K_expressionContext *context);
    virtual antlrcpp::Any visitK_expression_param(ZParser::K_expression_paramContext *context);
    virtual antlrcpp::Any visitZ_expression(ZParser::Z_expressionContext *context);
    virtual antlrcpp::Any visitLambda_decl(ZParser::Lambda_declContext *context);
    virtual antlrcpp::Any visitLambda_param(ZParser::Lambda_paramContext *context);
    virtual antlrcpp::Any visitLambda_body(ZParser::Lambda_bodyContext *context);
    virtual antlrcpp::Any visitConstantSetExpr(ZParser::ConstantSetExprContext *context);
    virtual antlrcpp::Any visitConstantINTExpr(ZParser::ConstantINTExprContext *context);
    virtual antlrcpp::Any visitSet_statement(ZParser::Set_statementContext *context);

private:
    llvm::Function *initializePrintfFunction();
};