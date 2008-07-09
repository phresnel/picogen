/***************************************************************************
 *            AST2LLVM.h
 *
 *  Jul 04 19:40:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <picogen/misc/scripting/AST2LLVM.h>

using namespace llvm;
using namespace std;



AST2LLVM::AST2LLVM()
: builder(), module (new Module ("me jit")), executionEngine (0), entryPoint(0), verbose (true)
{

}



AST2LLVM::~AST2LLVM () {

    while (!values.empty()){
        values.pop();
    }

    if (0 != executionEngine) {
        delete executionEngine;
        // From the comments in llvm's source code I read
        //    /// create - This is the factory method for creating an execution engine which
        //    /// is appropriate for the current machine.  This takes ownership of the
        //    /// module.
        // --> This means, if we have created an execution-engine, than we must no more
        //     delete module ? or what?
    } else {
        if (0 != module) delete module;
    }
}



void AST2LLVM::compile() {
    module->dump();
    executionEngine = ExecutionEngine::create (module);
    entryPoint = executionEngine->getPointerToFunction (funtab [""]);
    int (*FP)() = (int (*)())entryPoint;
    fprintf(stderr, "Evaluated to %i\n", FP());
}



void AST2LLVM::visit (const FloatExprAST*) {}
void AST2LLVM::visit (const IntExprAST*) {}
void AST2LLVM::visit (const IdExprAST*) {}
void AST2LLVM::visit (const CallExprAST*) {}
void AST2LLVM::visit (const BinaryExprAST*) {}
void AST2LLVM::visit (const AssignmentExprAST*) {}
void AST2LLVM::visit (const DeclarationAST*) {}
void AST2LLVM::visit (const BlockAST*) {}



void AST2LLVM::visit (const IfBlockAST *ast) {
    ast->enableVisit (false, false, false);
}



void AST2LLVM::visit (const WhileLoopAST*) {}
void AST2LLVM::visit (const DoWhileLoopAST*) {}



void AST2LLVM::visit (const FunProtoAST* protoast) {
    // Reminder:
    //   FunctionType * get (const Type *Result, const std::vector< const Type * > &Params, bool isVarArg)

    const Type *resultType = Type::Int32Ty;
    std::vector< const Type * > paramTypes;

    const std::vector<FunProtoAST::Argument> & args = protoast->getArguments();
    for (std::vector<FunProtoAST::Argument>::const_iterator it = args.begin(); it != args.end(); ++it) {
        switch (it->type) {
            case int_type:
                paramTypes.push_back (Type::Int32Ty);
                break;
            case float_type:
                paramTypes.push_back (Type::FloatTy);
                break;
        };
    }

    FunctionType *funtype = FunctionType::get (resultType, paramTypes, false);
    Function *fun = new Function (funtype, GlobalValue::ExternalLinkage, protoast->getName(), module);
    Function::arg_iterator ai;
    unsigned int srcidx = 0;
    for (ai = fun->arg_begin(); ai != fun->arg_end(); ++ai, ++srcidx) {
        ai->setName (args [srcidx].name);
    }

    // hmm
    BasicBlock *BB = new BasicBlock ("entry", fun);
    builder.SetInsertPoint(BB);

    funtab [protoast->getName()] = fun;
}



void AST2LLVM::visit (const FunAST *ast) {
}



void AST2LLVM::end (const FloatExprAST* ast) {
    values.push (ValueDescriptor (llvm::ConstantFP::get(llvm::Type::FloatTy, llvm::APFloat (ast->getValue())), ast));
}



void AST2LLVM::end (const IntExprAST* ast) {
    values.push (ValueDescriptor (llvm::ConstantInt::get(llvm::Type::Int32Ty, ast->getValue()), ast));
}



void AST2LLVM::end (const IdExprAST *ast)  {
    // A) Check if Id has been declared.
    SymbolTable::const_iterator it = symtab.find (ast->getValue());
    if (it == symtab.end()) {
        // We have an undeclared variable.
        /// \todo should this be handled by the frontend?
        std::cout << "!!we have an undeclared var." << std::endl;
        throw;
    }
    // B) Profit.
    AllocaInst *alloca = symtab [ast->getValue()].llvmAlloca;
    values.push (ValueDescriptor (alloca, ast));
    values.top().value = builder.CreateLoad (values.top().value);
    //builder.CreateLoad (valtmp);
    //values.push (ValueDescriptor (new LoadInst (symtab [ast->getValue()].llvmAlloca, ast->getValue().c_str()), ast));
}



void AST2LLVM::end (const CallExprAST*)  {
}



void AST2LLVM::end (const BinaryExprAST* ast)   {
    // Load rhs.
    Value *rhs   = values.top().value;
    if (false && 0 != dynamic_cast<const IdExprAST*> (values.top().ast)) {
        // We got to load an id.
        Value *valtmp = rhs;
        rhs = builder.CreateLoad (valtmp);
    }
    values.pop();

    // Load lhs.
    Value *lhs   = values.top().value;
    if (false && 0 != dynamic_cast<const IdExprAST*> (values.top().ast)) {
        // We got to load an id.
        Value *valtmp = lhs;
        lhs = builder.CreateLoad (valtmp);
    }
    values.pop();

    switch (ast->getOp()) {
        case '+':
            values.push (ValueDescriptor (builder.CreateAdd (lhs, rhs, "addtmp"), ast));
            break;
        case '-':
            values.push (ValueDescriptor (builder.CreateSub (lhs, rhs, "subtmp"), ast));
            break;
        case '*':
            values.push (ValueDescriptor (builder.CreateMul (lhs, rhs, "multmp"), ast));
            break;
        case '/':
            values.push (ValueDescriptor (builder.CreateFDiv (lhs, rhs, "fdivtmp"), ast));
            break;
        default:
            std::cerr << "!!ouch, unsupported operator in AST2LLVM: " << ast->getOp() << endl;
    };
    //if (verbose) values.top().value->dump();
}



void AST2LLVM::end (const AssignmentExprAST *ast) {
    // Load val.
    Value *val   = values.top().value;
    if (false && 0 != dynamic_cast<const IdExprAST*> (values.top().ast)) {
        // We got to load an id.
        Value *valtmp = val;
        val = builder.CreateLoad (valtmp);
    }
    values.pop();

    // This is a bit tricky (not really): in most cases we want to use the value of what an Id points to.
    // But in assignments, even if not obvious at first, the LHS is actually the mem-ptr and not the value
    // and to not struggle our code too much we go through the usual routine 'values.top()/pop()'
    // but then acquire our symtab, which holds the address in llvmAlloca.

    // Next: to what we assign
    values.pop(); // Value not needed, see above comment.
    Value *alloc = symtab [ast->getId()->getValue()].llvmAlloca;


    // Finally create a store.
    values.push (ValueDescriptor (builder.CreateStore (val, alloc), ast));
}



void AST2LLVM::end (const DeclarationAST *ast)  {
    const Type *llvmType = 0;
    switch (ast->getType()) {
        case int_type:
            llvmType = Type::Int32Ty;
            break;
        case float_type:
            llvmType = Type::FloatTy;
            break;
    };

    if (symtab.end() != symtab.find (ast->getName())) {
        std::cout << "!! already declared!" << std::endl;
        throw;
    } else {
        AllocaInst *tmp = builder.CreateAlloca (llvmType, 0, ast->getName().c_str());
        symtab [ast->getName()] = Symbol (ast->getType(), ast->getName(), tmp);
    }
}



void AST2LLVM::end (const BlockAST*)  {
    //std::cout << "Block - " << values.size() << std::endl;
    // What we have now in values is what goes en blok.

    //BasicBlock *BB = builder.Create ("entry",
    /*
    BasicBlock *BB = builder.GetInsertBlock ();
    if (0 != BB) {
        BasicBlock::iterator it;
        for (it = BB->begin(); it != BB->end(); ++it) {
        }
    }*/
}



void AST2LLVM::end (const IfBlockAST *ast)  {
    Value *val_cond = 0;
    Value *val_then = 0;
    Value *val_else = 0;

    // Generate If:
    ast->getIfClause()->accept (*this);
    val_cond = values.top().value;
    values.pop();

    val_cond = builder.CreateICmpNE (val_cond, ConstantInt::get(Type::Int32Ty, 0), "ifcond");

    Function *parentFun = builder.GetInsertBlock()->getParent();
    BasicBlock *thenBB  = new BasicBlock ("then", parentFun);
    BasicBlock *elseBB  = new BasicBlock ("else");
    BasicBlock *mergeBB = new BasicBlock ("ifcont");

    builder.CreateCondBr (val_cond, thenBB, elseBB);

    // Generate Then:
    builder.SetInsertPoint (thenBB);
    const ExprAST *ifBody = ast->getIfBody(); // Makes it easier to dbg when saved to var.
    if (0 != ifBody) {
        const unsigned int old_size = values.size();
        ifBody->accept (*this);
        if (values.size() != old_size) { // Check if any code has been generated.
            val_then = values.top().value;
            values.pop();
        }
    }
    builder.CreateBr (mergeBB);
    thenBB = builder.GetInsertBlock();

    // Generate Else:
    parentFun->getBasicBlockList().push_back (elseBB); // Remember else-target.
    builder.SetInsertPoint (elseBB);

    const ExprAST *elseBody = ast->getElseBody(); // Makes it easier to dbg when saved to var.
    if (0 != elseBody) {
        const unsigned int old_size = values.size();
        elseBody->accept (*this);
        if (values.size() != old_size) { // Check if any code has been generated.
            val_else = values.top().value;
            values.pop();
        }
    }

    builder.CreateBr (mergeBB);
    elseBB = builder.GetInsertBlock();

    // Merge:
    parentFun->getBasicBlockList().push_back (mergeBB);
    builder.SetInsertPoint(mergeBB);

    /*PHINode *phi = builder.CreatePHI(Type::Int32Ty, "iftmp");
    phi->addIncoming (val_then, thenBB);
    phi->addIncoming (val_else, elseBB);
    values.push (ValueDescriptor (phi, ast));
    */
    ast->enableVisit (true, true, true);
}



void AST2LLVM::end (const WhileLoopAST*)  {
    //std::cout << "WhileLoop" << "-" << std::endl;
}



void AST2LLVM::end (const DoWhileLoopAST*)  {
    //std::cout << "DoWhileLoop" << "-" << std::endl;
}



void AST2LLVM::end (const FunProtoAST *ast)  {}



void AST2LLVM::end (const FunAST *ast) {
    Value *retval = values.top().value; values.pop();
    builder.CreateRet (retval);
    verifyFunction (*funtab [ast->getPrototype()->getName()]);
}
