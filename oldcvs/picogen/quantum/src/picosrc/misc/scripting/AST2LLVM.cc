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


int AST2LLVM::walkDepth = 0;

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
    std::cerr << "We're ready, but there are still " << values.size() << " entries on the value-stack." << endl;
    module->dump();
    executionEngine = ExecutionEngine::create (module);
    entryPoint = executionEngine->getPointerToFunction (funtab [""]);
    int (*FP)() = (int (*)())entryPoint;
    fprintf(stderr, "Evaluated to %i\n", FP());
}



void AST2LLVM::visit (const FloatExprAST *ast) {
    begin ("void AST2LLVM::visit (const FloatExprAST *ast)");
    values.push (ValueDescriptor (llvm::ConstantFP::get(llvm::Type::FloatTy, llvm::APFloat (ast->getValue())), ast, float_type));
    end ("void AST2LLVM::visit (const FloatExprAST *ast)");
}



void AST2LLVM::visit (const IntExprAST *ast) {
    begin ("void AST2LLVM::visit (const IntExprAST *ast)");
    values.push (ValueDescriptor (llvm::ConstantInt::get(llvm::Type::Int32Ty, ast->getValue()), ast, int_type));
    end ("void AST2LLVM::visit (const IntExprAST *ast)");
}



void AST2LLVM::visit (const BoolExprAST *ast) {
    begin ("void AST2LLVM::visit (const BoolExprAST *ast)");
    values.push (ValueDescriptor (llvm::ConstantInt::get(llvm::Type::Int1Ty, ast->getValue()?1:0), ast, bool_type));
    end ("void AST2LLVM::visit (const BoolExprAST *ast)");
}



void AST2LLVM::visit (const IdExprAST *ast) {
    begin ("void AST2LLVM::visit (const IdExprAST *ast)");
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
    values.push (ValueDescriptor (alloca, ast, it->second.type));
    values.top().value = builder.CreateLoad (values.top().value);
    //builder.CreateLoad (valtmp);
    //values.push (ValueDescriptor (new LoadInst (symtab [ast->getValue()].llvmAlloca, ast->getValue().c_str()), ast));
    end ("void AST2LLVM::visit (const IdExprAST *ast)");
}



void AST2LLVM::visit (const CallExprAST*) {
    begin ("void AST2LLVM::visit (const CallExprAST*)");
    end ("void AST2LLVM::visit (const CallExprAST*)");
}



void AST2LLVM::visit (const BinaryExprAST *ast) {
    begin ("void AST2LLVM::visit (const BinaryExprAST *ast)");

    // Load lhs.
    ast->getLhs()->accept (*this);
    Value *lhs   = values.top().value;
    const Datatype l_type = values.top().type;
    values.pop();

    // Load rhs.
    ast->getRhs()->accept (*this);
    Value *rhs   = values.top().value;
    const Datatype r_type = values.top().type;
    values.pop();

    // Check Type Equality.
    if (l_type != r_type) {
        std::cerr << "!! types not identical in binop (" << l_type << "!=" << r_type << ") !!" << endl;
        throw;
    }

    const Datatype res_type = l_type;

    // Check Type Validity.
    switch (res_type) {
        case float_type:  // fallthrough
        case int_type:    // fallthrough
        case bool_type:   // fallthrough
            // Okay.
            break;
        case void_type:   // fallthrough
        case mixed_type:  // fallthrough
            std::cerr << "!! unsupported type in binop !!" << endl;
            throw;
    };


    switch (ast->getOp()) {
        case add_binop:
            switch (res_type) {
                case float_type:  // fallthrough
                case int_type:    // fallthrough
                    values.push (ValueDescriptor (builder.CreateAdd (lhs, rhs, "addtmp"), ast, res_type));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in binop-add !!" << endl;
                    throw;
            };
            break;
        case sub_binop:
            switch (res_type) {
                case float_type:  // fallthrough
                case int_type:    // fallthrough
                    values.push (ValueDescriptor (builder.CreateSub (lhs, rhs, "subtmp"), ast, res_type));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in binop-sub !!" << endl;
                    throw;
            };
            break;
        case mul_binop:
            switch (res_type) {
                case float_type:  // fallthrough
                case int_type:    // fallthrough
                    values.push (ValueDescriptor (builder.CreateMul (lhs, rhs, "multmp"), ast, res_type));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in binop-mul !!" << endl;
                    throw;
            };
            break;
        case div_binop:
            switch (res_type) {
                case int_type:
                    values.push (ValueDescriptor (builder.CreateSDiv (lhs, rhs, "sdivtmp"), ast, res_type));
                    break;
                case float_type:
                    values.push (ValueDescriptor (builder.CreateFDiv (lhs, rhs, "fdivtmp"), ast, res_type));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in division !!" << endl;
                    throw;
            };
            break;
        case log_and_binop:
        case log_or_binop:
            std::cerr << "!! logical and/or not supported atm !!" << endl;
            break;
        case eq_binop:
            switch (res_type) {
                case bool_type: // fallthrough
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpEQ (lhs, rhs, "slttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpUEQ (lhs, rhs, "flttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in eq !!" << endl;
                    throw;
            };
            break;
        case ne_binop:
            switch (res_type) {
                case bool_type: // fallthrough
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpNE (lhs, rhs, "slttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpUNE (lhs, rhs, "flttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in ne !!" << endl;
                    throw;
            };
            break;
        case lt_binop:
            switch (res_type) {
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpSLT (lhs, rhs, "slttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpULT (lhs, rhs, "flttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in lt !!" << endl;
                    throw;
            };
            break;
        case gt_binop:
            switch (res_type) {
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpSGT (lhs, rhs, "sgttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpUGT (lhs, rhs, "fgttmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in gt !!" << endl;
                    throw;
            };
            break;
        case le_binop:
            switch (res_type) {
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpSLE (lhs, rhs, "sletmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpULE (lhs, rhs, "fletmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in le !!" << endl;
                    throw;
            };
            break;
        case ge_binop:
            switch (res_type) {
                case int_type:
                    values.push (ValueDescriptor (
                        builder.CreateICmpSGE (lhs, rhs, "sgetmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case float_type:
                    values.push (ValueDescriptor (
                        builder.CreateFCmpUGE (lhs, rhs, "fgetmp"),
                        ast,
                        bool_type
                    ));
                    break;
                case bool_type: case void_type: case mixed_type: // fallthrough
                    std::cerr << "!! unsupported type in ge !!" << endl;
                    throw;
            };
            break;
        case nop_binop:
            std::cerr << "!! ouch, unsupported operator in AST2LLVM (caught an nop_binop) !!" << endl;
            throw;
    };
    //if (verbose) values.top().value->dump();
    end ("void AST2LLVM::visit (const BinaryExprAST *ast)");
}



void AST2LLVM::visit (const AssignmentExprAST *ast) {
    begin ("void AST2LLVM::visit (const AssignmentExprAST *ast)");


    // Note: the two accept calls below are exactly there because in the beggining AST2LLVM
    //   did not implement NonRecursingVisitor but rather the recursing one, which resulted
    //   in such bottom-up behaviour. We could as well accept() at the places exactly were needed
    //   but right now I am lazy.
    ast->getId()->accept (*this);
    ast->getRhs()->accept (*this);

    // Load val.
    Value *val   = values.top().value;
    const Datatype r_type = values.top().type;
    values.pop();

    // This is a bit tricky (not really): in most cases we want to use the value of what an Id points to.
    // But in assignments, even if not obvious at first, the LHS is actually the mem-ptr and not the value
    // and to not struggle our code too much we go through the usual routine 'values.top()/pop()'
    // but then acquire our symtab, which holds the address in llvmAlloca.

    // Next: to what we assign
    const Datatype l_type = values.top().type;
    values.pop(); // Value not needed, see above comment.
    Value *alloc = symtab [ast->getId()->getValue()].llvmAlloca;


    // Check Type Equality.
    if (l_type != r_type) {
        std::cerr << "!! types not identical in assignmt (" << l_type << "!=" << r_type << ") !!" << endl;
        throw;
    }

    const Datatype res_type = l_type;

    // Check Type Validity.
    switch (res_type) {
        case float_type:  // fallthrough
        case int_type:    // fallthrough
        case bool_type:   // fallthrough
            // Okay.
            break;
        case void_type: case mixed_type: // fallthrough
            std::cerr << "!! unsupported type in assignmt !!" << endl;
            throw;
    };

    // Finally create a store.
    values.push (ValueDescriptor (builder.CreateStore (val, alloc), ast, res_type));
    end ("void AST2LLVM::visit (const AssignmentExprAST *ast)");
}



void AST2LLVM::visit (const DeclarationAST* ast) {
    begin ("void AST2LLVM::visit (const DeclarationAST* ast)");
    // pro

    // epi
    const Type *llvmType = 0;
    switch (ast->getType()) {
        case int_type:
            llvmType = Type::Int32Ty;
            break;
        case bool_type:
            llvmType = Type::Int1Ty;
            break;
        case float_type:
            llvmType = Type::FloatTy;
            break;
        case void_type: case mixed_type:
            std::cerr << "!! unsupported type in AST2LLVM::visit (const DeclararationAST*) !!" << endl;
            throw;
    };

    if (symtab.end() != symtab.find (ast->getName())) {
        std::cout << "!! already declared!" << std::endl;
        throw;
    } else {
        AllocaInst *tmp = builder.CreateAlloca (llvmType, 0, ast->getName().c_str());
        symtab [ast->getName()] = Symbol (ast->getType(), ast->getName(), tmp);
    }
    end ("void AST2LLVM::visit (const DeclarationAST* ast)");
}



void AST2LLVM::visit (const BlockAST *ast) {
    begin ("void AST2LLVM::visit (const BlockAST*)");
    BlockAST::const_iterator it = ast->begin();
    while (ast->end() != it) {
        if (0 != *it) {
            const int base = values.size(); // after it->accept, the stack should be +1
            (*it)->accept (*this);
            const int diff = values.size()-base;
            if (diff != 0 && diff != 1) {
                std::cerr << "!! Normally, for each item in a BlockAST, there should be added exactly 0 or exactly 1 value on the stack, but here are +"
                    <<  values.size()-base << " !!" << endl;
                throw;
            }
            // If that on the stack, if any, does not belong to a return-instr, then send it to heaven.
            if (0 < diff && 0 == dynamic_cast <const RetAST *> (*it)) {
                values.pop();
            }
            /*const RetAST *ret = dynamic_cast<const RetAST*>(*it);
            if (0 != ret) {
                std::cout << "heh!" << endl;
            }*/
        }
        ++it;
    }
    end ("void AST2LLVM::visit (const BlockAST*)");
}



void AST2LLVM::visit (const IfBlockAST *ast) {
    begin ("void AST2LLVM::visit (const IfBlockAST *ast)");
    Value *val_cond = 0;
    Value *val_then = 0;
    Value *val_else = 0;

    // Generate If:
    ast->getIfClause()->accept (*this);
    val_cond = values.top().value;
    values.pop();

    val_cond = builder.CreateICmpNE (val_cond, ConstantInt::get(Type::Int1Ty, 0), "ifcond");

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
    parentFun->getBasicBlockList().push_back (elseBB);
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
    end ("void AST2LLVM::visit (const IfBlockAST *ast)");
}



void AST2LLVM::visit (const WhileLoopAST *ast) {
    begin ("void AST2LLVM::visit (const WhileLoopAST*)");
    Value *val_cond = 0;
    Value *val_body = 0;

    Function *parentFun = builder.GetInsertBlock()->getParent();
    BasicBlock *condBB  = new BasicBlock ("whilecond", parentFun);
    BasicBlock *bodyBB  = new BasicBlock ("whilebody");
    BasicBlock *mergeBB = new BasicBlock ("whilemerge");

    builder.CreateBr (condBB);

    // Generate Condition:
    builder.SetInsertPoint (condBB);
    ast->getCondition()->accept (*this);
    val_cond = values.top().value;
    values.pop();
    val_cond = builder.CreateICmpNE (val_cond, ConstantInt::get(Type::Int1Ty, 0), "whilecond");
    builder.CreateCondBr (val_cond, bodyBB, mergeBB);

    // Generate Body:
    parentFun->getBasicBlockList().push_back (bodyBB);
    builder.SetInsertPoint (bodyBB);

    const ExprAST *body = ast->getBody(); // Makes it easier to dbg when saved to var.
    if (0 != body) {
        const unsigned int old_size = values.size();
        body->accept (*this);
        if (values.size() != old_size) { // Check if any code has been generated.
            val_body = values.top().value;
            values.pop();
        }
    }
    builder.CreateBr (condBB);

    // Merge:
    parentFun->getBasicBlockList().push_back (mergeBB);
    builder.SetInsertPoint(mergeBB);

    end ("void AST2LLVM::visit (const WhileLoopAST*)");
}



void AST2LLVM::visit (const DoWhileLoopAST *ast) {
    begin ("void AST2LLVM::visit (const DoWhileLoopAST*)");

    Value *val_cond = 0;
    Value *val_body = 0;

    Function *parentFun = builder.GetInsertBlock()->getParent();
    BasicBlock *bodyBB  = new BasicBlock ("whilebody", parentFun);
    BasicBlock *condBB  = new BasicBlock ("whilecond");
    BasicBlock *mergeBB = new BasicBlock ("whilemerge");

    builder.CreateBr (bodyBB);

    // Generate Body:
    builder.SetInsertPoint (bodyBB);

    const ExprAST *body = ast->getBody(); // Makes it easier to dbg when saved to var.
    if (0 != body) {
        const unsigned int old_size = values.size();
        body->accept (*this);
        if (values.size() != old_size) { // Check if any code has been generated.
            val_body = values.top().value;
            values.pop();
        }
    }
    builder.CreateBr (condBB);

    // Generate Condition:
    parentFun->getBasicBlockList().push_back (condBB);
    builder.SetInsertPoint (condBB);
    ast->getCondition()->accept (*this);
    val_cond = values.top().value;
    values.pop();
    val_cond = builder.CreateICmpNE (val_cond, ConstantInt::get(Type::Int1Ty, 0), "whilecond");
    builder.CreateCondBr (val_cond, bodyBB, mergeBB);


    // Merge:
    parentFun->getBasicBlockList().push_back (mergeBB);
    builder.SetInsertPoint(mergeBB);

    end ("void AST2LLVM::visit (const DoWhileLoopAST*)");
}



void AST2LLVM::visit (const FunProtoAST* proto) {
    begin ("void AST2LLVM::visit (const FunProtoAST* protoast)");
    // Reminder:
    //   FunctionType * get (const Type *Result, const std::vector< const Type * > &Params, bool isVarArg)

    const Type *resultType = 0;//Type::Int32Ty;
    switch (proto->getType()) {
        case int_type:
            resultType = Type::Int32Ty;
            break;
        case bool_type:
            resultType = Type::Int1Ty;
            break;
        case float_type:
            resultType = Type::FloatTy;
            break;
        case void_type:
            resultType = Type::VoidTy;
            break;
        case mixed_type:
            std::cerr << "!! unsupported return-type in AST2LLVM::visit (const FunProtoAST*) (e.g. mixed_type)!!" << endl;
            throw;
    };

    std::vector<const Type *> paramTypes;

    const std::vector<FunProtoAST::Argument> & args = proto->getArguments();
    for (std::vector<FunProtoAST::Argument>::const_iterator it = args.begin(); it != args.end(); ++it) {
        switch (it->type) {
            case int_type:
                paramTypes.push_back (Type::Int32Ty);
                break;
            case bool_type:
                paramTypes.push_back (Type::Int1Ty);
                break;
            case float_type:
                paramTypes.push_back (Type::FloatTy);
                break;
            case void_type: case mixed_type:
                std::cerr << "!! unsupported type in AST2LLVM::visit (const FunProtoAST*) !!" << endl;
                throw;
        };
    }

    /// \todo How do we handle lvl[2[a|b]|3]-overloading?
    if (0 == funtab [proto->getName()]) {
        FunctionType *funtype = FunctionType::get (resultType, paramTypes, false);
        Function *fun = new Function (funtype, GlobalValue::ExternalLinkage, proto->getName(), module);
        Function::arg_iterator ai;
        unsigned int srcidx = 0;
        for (ai = fun->arg_begin(); ai != fun->arg_end(); ++ai, ++srcidx) {
            ai->setName (args [srcidx].name);
        }

        funtab [proto->getName()] = fun;
    }
    end ("void AST2LLVM::visit (const FunProtoAST* protoast)");
}



void AST2LLVM::visit (const FunAST *ast) {
    begin ("void AST2LLVM::visit (const FunAST *ast)");

    if (0 == ast->getPrototype()) {
        std::cerr << "!! Prototype cannot be 0 when parsing function !!" << endl;
        throw;
    }
    if (0 == ast->getBody()) {
        std::cerr << "!! Function-Body cannot be 0 when parsing it !!" << endl;
        throw;
    }

    const bool verb = false;

    if (verb) std::cerr << "1) building proto of " << ast->getPrototype()->getName() << endl;


    // Get function, if yet created.
    Function *fun = funtab [ast->getPrototype()->getName()];

    if (0 == fun) {
        ast->getPrototype()->accept (*this);
        fun = funtab [ast->getPrototype()->getName()];
        if (0 == fun) {
            std::cerr << "!! something went wrong while parsing function prototype !!" << endl;
            throw;
        }
    }

    if (verb) std::cerr << "2) building corpse of " << ast->getPrototype()->getName() << endl;

    // Start Inserting into Function.
    BasicBlock *previousBB = builder.GetInsertBlock();

    //Function *parentFun = builder.GetInsertBlock()->getParent();

    BasicBlock *BB = new BasicBlock ("entry", fun);
    builder.SetInsertPoint(BB);
    ast->getBody()->accept (*this);

    // Build Terminator.
    if (ast->getPrototype()->getType() == void_type) {
        builder.CreateRetVoid ();
    } else if (values.size() > 0) {
        Value *retval = values.top().value; values.pop();
        builder.CreateRet (retval);
    } else {
        if (verb) std::cerr << "!! missing something on the value-stack while finalizing function '" <<
            ast->getPrototype()->getName() << "' !!" << endl;
        throw;
    }

    // Return to inserting to outer function, if nested.
    if (0 != previousBB) {
        //BasicBlock *continueBB = new BasicBlock ("cont", previousBB->getParent());
        builder.SetInsertPoint (previousBB);
    }

    if (verb) std::cerr << "3) verifying " << ast->getPrototype()->getName() << endl;
    //verifyFunction (*funtab [ast->getPrototype()->getName()]);
    if (verb) std::cerr << "4) looks good for " << ast->getPrototype()->getName() << endl;

    end ("void AST2LLVM::visit (const FunAST *ast)");
}

void AST2LLVM::visit (const RetAST *ast) {
    begin ("void AST2LLVM::visit (const RetAST *ast)");
    if (0 != ast->getRet()) {
        ast->getRet()->accept (*this);
    }
    end ("void AST2LLVM::visit (const RetAST *ast)");
}
