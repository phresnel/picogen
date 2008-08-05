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
#include <map>

#include <llvm/DerivedTypes.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Constants.h>
#include <llvm/Support/LLVMBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <picogen/misc/scripting/AST.h>

class AST2LLVM : public ASTNonRecursingVisitor /*ASTVisitor*/ {
        struct ValueDescriptor {
            llvm::Value *value;
            const ExprAST *ast;
            const Datatype type;
            const bool isIdentifier;
            explicit ValueDescriptor (llvm::Value * val, const ExprAST *ast, Datatype type, bool isIdentifier = false)
            : value(val), ast(ast), type(type), isIdentifier(isIdentifier) {}
        };
        std::stack<ValueDescriptor> values;

        struct Symbol;
        typedef std::map<std::string, Symbol> SymbolTable;
        struct Symbol {
            public:
                const Datatype type;
                const std::string id;
                llvm::AllocaInst * const llvmAlloca;
            protected:
                friend class std::map<std::string, Symbol>;
                Symbol () : type(int_type), id("<unknown>"), llvmAlloca(0) {} // Needed for std::map<>.
            public:
                Symbol (Datatype type, const std::string &id, llvm::AllocaInst *llvmAlloca)
                : type(type), id(id), llvmAlloca(llvmAlloca) {}

                Symbol (const Symbol &sym) : type(sym.type), id(sym.id), llvmAlloca(sym.llvmAlloca) {}
                const Symbol & operator = (const Symbol &sym) {
                    const_cast<Datatype&>(type) = sym.type;
                    const_cast<std::string&>(id) = sym.id;
                    const_cast<const llvm::AllocaInst*&>(llvmAlloca) = sym.llvmAlloca;
                    return *this;
                }
        };
        SymbolTable symtab;

        // For incarnation Numero Uno, we will only support Function-Overloading as in C++,
        // hence we will take the lvl1-name of the function as the key.
        typedef std::map<std::string, llvm::Function*> FunctionTable;
        FunctionTable funtab;

        llvm::LLVMBuilder builder;
        llvm::Module *module;
        llvm::ExecutionEngine *executionEngine;
        void *entryPoint;
        bool verbose;

        static int walkDepth;
        static inline void begin (const char *id) {
            for (int i=0; i<walkDepth; ++i) std::cout << "  ";
            walkDepth++;
            std::cout << id << " { " << std::endl;
        }
        static inline void end (const char *id) {
            walkDepth--;
            for (int i=0; i<walkDepth; ++i) std::cout << "  ";
            std::cout << "}" << std::endl;
        }

    public:

        AST2LLVM();
        virtual ~AST2LLVM ();

        void compile();

        virtual void visit (const FloatExprAST*);
        virtual void visit (const IntExprAST*);
        virtual void visit (const BoolExprAST*);
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
        virtual void visit (const RetAST*);
};


#endif // AST2LLVM_H
