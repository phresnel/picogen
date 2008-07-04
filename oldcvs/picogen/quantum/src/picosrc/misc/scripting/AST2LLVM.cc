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

AST2LLVM::AST2LLVM() : verbose(true) {
}

AST2LLVM::~AST2LLVM () {
    std::cout << endl;
    while (!values.empty()){
        std::cout << values.top().value << endl;
        values.pop();
    }
    std::cout << "-------------" << endl;
}

void AST2LLVM::visit (const FloatExprAST*) {}
void AST2LLVM::visit (const IntExprAST*) {}
void AST2LLVM::visit (const IdExprAST*) {}
void AST2LLVM::visit (const CallExprAST*) {}
void AST2LLVM::visit (const BinaryExprAST*) {}
void AST2LLVM::visit (const AssignmentExprAST*) {}
void AST2LLVM::visit (const DeclarationAST*) {}
void AST2LLVM::visit (const BlockAST*) {}
void AST2LLVM::visit (const IfBlockAST*) {}
void AST2LLVM::visit (const WhileLoopAST*) {}
void AST2LLVM::visit (const DoWhileLoopAST*) {}
void AST2LLVM::visit (const FunProtoAST*) {}
void AST2LLVM::visit (const FunAST*) {}



void AST2LLVM::end (const FloatExprAST* ast) {
    values.push (ValueDescriptor (llvm::ConstantFP::get(llvm::Type::FloatTy, llvm::APFloat (ast->getValue())), ast));
    if (verbose) values.top().value->dump();
}



void AST2LLVM::end (const IntExprAST* ast) {
    values.push (ValueDescriptor (llvm::ConstantInt::get(llvm::Type::Int32Ty, ast->getValue()), ast));
    if (verbose) values.top().value->dump();
}



void AST2LLVM::end (const IdExprAST*)  {
    ////std::cout << "IdExpr" << "-" << std::endl;
}



void AST2LLVM::end (const CallExprAST*)  {
    ////std::cout << "CallExpr" << "-" << std::endl;
}



void AST2LLVM::end (const BinaryExprAST* ast)   {
    Value *rhs = values.top().value;  values.pop();
    Value *lhs = values.top().value;  values.pop();

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
            std::cerr << "ouch, unsupported operator in AST2LLVM: " << ast->getOp() << endl;
    };
    if (verbose) values.top().value->dump();
}



void AST2LLVM::end (const AssignmentExprAST*)   {
    //std::cout << "AssignmentExpr" << "-" << std::endl;
}



void AST2LLVM::end (const DeclarationAST*)  {
    std::cout << "Declaration" << "-" << std::endl;
    //ConstantFP::get(Type::DoubleTy, APFloat(Val));
}



void AST2LLVM::end (const BlockAST*)  {
    //std::cout << "Block" << "-" << std::endl;
}



void AST2LLVM::end (const IfBlockAST*)  {
    //std::cout << "IfBlock" << "-" << std::endl;
}



void AST2LLVM::end (const WhileLoopAST*)  {
    //std::cout << "WhileLoop" << "-" << std::endl;
}



void AST2LLVM::end (const DoWhileLoopAST*)  {
    //std::cout << "DoWhileLoop" << "-" << std::endl;
}



void AST2LLVM::end (const FunProtoAST*)  {
    //std::cout << "FunProto" << "-" << std::endl;
}



void AST2LLVM::end (const FunAST*)  {
    //std::cout << "Fun" << "-" << std::endl;
}
