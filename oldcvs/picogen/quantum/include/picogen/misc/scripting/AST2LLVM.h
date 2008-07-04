/***************************************************************************
 *            AST2LLVM.h
 *
 *  Jul 03 20:14:00 2008
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

#ifndef AST2LLVM_H
#define AST2LLVM_H

#include <stack>

#include <llvm/DerivedTypes.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Constants.h>
#include <llvm/Support/LLVMBuilder.h>

#include <picogen/misc/scripting/AST.h>

class AST2LLVM : public ASTVisitor {
        struct ValueDescriptor {
            llvm::Value *value;
            const ExprAST *ast;
            explicit ValueDescriptor (llvm::Value * val, const ExprAST *ast) : value(val), ast(ast) {}
        };
        std::stack<ValueDescriptor> values;
        llvm::LLVMBuilder builder;
        bool verbose;
    public:

        AST2LLVM();
        virtual ~AST2LLVM ();

        virtual void visit (const FloatExprAST*);
        virtual void visit (const IntExprAST*);
        virtual void visit (const IdExprAST*);
        virtual void visit (const CallExprAST*);
        virtual void visit (const BinaryExprAST*);
        virtual void visit (const AssignmentExprAST*);
        virtual void visit (const DeclarationAST*);
        virtual void visit (const BlockAST*);
        virtual void visit (const IfBlockAST*);
        virtual void visit (const WhileLoopAST*);
        virtual void visit (const DoWhileLoopAST*);
        virtual void visit (const FunProtoAST*);
        virtual void visit (const FunAST*);

        virtual void end (const FloatExprAST* ast);
        virtual void end (const IntExprAST* ast);
        virtual void end (const IdExprAST*);
        virtual void end (const CallExprAST*);
        virtual void end (const BinaryExprAST* ast);
        virtual void end (const AssignmentExprAST*);
        virtual void end (const DeclarationAST*);
        virtual void end (const BlockAST*);
        virtual void end (const IfBlockAST*);
        virtual void end (const WhileLoopAST*);
        virtual void end (const DoWhileLoopAST*);
        virtual void end (const FunProtoAST*);
        virtual void end (const FunAST*);
};



class LLVMCodegen {
    public:
        virtual llvm::Value *codegen () = 0;
};

class LLVMFloatExprAST : public FloatExprAST, public LLVMCodegen {
    public:
        virtual llvm::Value *codegen () {
            return llvm::ConstantFP::get(llvm::Type::FloatTy, llvm::APFloat (value));
        }
};

class LLVMIntExprAST : public IntExprAST, public LLVMCodegen {
    public:
        virtual llvm::Value *codegen () {
            return llvm::ConstantInt::get(llvm::Type::Int32Ty, value);
        }
};


#endif // AST2LLVM_H
