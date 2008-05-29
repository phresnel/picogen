/***************************************************************************
 *            AST.h
 *
 *  May 10 10:01:00 2008
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

/// \todo create namespaces for this file
#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED


#define AST_PRINT_DO_INDENT(indent) for( int i=0; i<indent; ++i ) std::cout << "  ";

#include <iostream>

// the following code is clearly inspired by the kaleidoscope tutorial on llvm.org

typedef enum Datatype {
    int_type,
    float_type
};

class ExprAST;
class FloatExprAST;
class IntExprAST;
class IdExprAST;
class CallExprAST;
class BinaryExprAST;
class BlockAST;
class IfBlockAST;
class WhileLoopAST;
class DoWhileLoopAST;
class FunProtoAST;
class FunAST;

class ExprAST {
    public:
        /// \todo store type (?)
        virtual ~ExprAST() {}
        virtual void print (int indent = 0) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "expr" << std::endl;
        }
};

class FloatExprAST : public ExprAST {
        const float value;
    public:
        explicit FloatExprAST (float value) : value (value) {}
        virtual ~FloatExprAST() {}
        float getValue() const {
            return value;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "floatExpr(" << value << ")" << std::endl;
        }
};

class IntExprAST : public ExprAST {
        const int value;
    public:
        explicit IntExprAST (int value) : value (value) {}
        virtual ~IntExprAST() {}
        int getValue() const {
            return value;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "intExpr(" << value << ")" << std::endl;
        }
};

class IdExprAST : public ExprAST {
        const std::string value;
    public:
        explicit IdExprAST (const std::string &value) : value (value) {}
        virtual ~IdExprAST() {}
        const std::string &getValue() const {
            return value;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "idExpr(" << value << ")" << std::endl;
        }
};

class CallExprAST : public ExprAST {
        const std::string callee;
        const std::vector<ExprAST*> args;
    public:
        explicit CallExprAST (const std::string &callee, const std::vector<ExprAST*> &args)
                : callee (callee), args (args) {}
        virtual ~CallExprAST() {}
        const std::string &getCallee() const {
            return callee;
        }
        const std::vector<ExprAST*> &getArgs() const {
            return args;
        }
};

class BinaryExprAST : public ExprAST {
        const char op;
        const ExprAST *lhs;
        const ExprAST *rhs;
    public:
        explicit BinaryExprAST (char op, const ExprAST *lhs, const ExprAST *rhs) : op (op), lhs (lhs), rhs (rhs) {}
        virtual ~BinaryExprAST() {}
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "binaryOpExpr(" << op << "){" << std::endl;
            if (lhs != NULL)
                lhs->print (indent + 1);
            if (rhs != NULL)
                rhs->print (indent + 1);
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
};

class BlockAST : public ExprAST {
        std::vector<const ExprAST *> list; // first i had an hierarchical (even when linear) block. but that was ugly to debug.
    public:
        explicit BlockAST() : list() {}
        virtual ~BlockAST() {}
        virtual void addTail (const ExprAST *ast) {
            list.push_back (ast);
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "block{" << std::endl;
            for (std::vector<const ExprAST*>::const_iterator it=list.begin(); it!=list.end(); ++it) {
                if ( (*it) != NULL)
                    (*it)->print (indent+1);
            }
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
            /*AST_PRINT_DO_INDENT(indent);
            std::cout << "block{" << std::endl;
            a->print( indent + 1 );
            b->print( indent + 1 );
            AST_PRINT_DO_INDENT(indent);
            std::cout << "}" << std::endl;*/
        }
};

class IfBlockAST : public ExprAST {
        const ExprAST *if_clause;
        const ExprAST *if_body;
        const ExprAST *else_body;
    public:
        explicit IfBlockAST (const ExprAST *if_clause, const ExprAST *if_body, const ExprAST *else_body)
                : if_clause (if_clause), if_body (if_body), else_body (else_body) {}
        virtual ~IfBlockAST() {}
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "if-block{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "if:" << std::endl;
            if (if_clause != NULL)
                if_clause->print (indent+2);
            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "then:" << std::endl;
            if (if_body != NULL)
                if_body->print (indent+2);
            if (else_body != NULL) {
                AST_PRINT_DO_INDENT (indent+1);
                std::cout << "else:" << std::endl;
                else_body->print (indent+2);
            }
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
};

// we want to avoid pure jump-ast's, so we provide two hardcoded loops (head-controlled and foot-controlled)
class WhileLoopAST : public ExprAST {
        const ExprAST *condition;
        const ExprAST *body;
    public:
        explicit WhileLoopAST (const ExprAST *condition, const ExprAST *body)
                : condition (condition), body (body) {}
        virtual ~WhileLoopAST() {}
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "while-loop{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "condition:" << std::endl;
            if (NULL != condition)
                condition->print (indent+2);
            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "body:" << std::endl;
            if (NULL != body)
                body->print (indent+2);
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
};

class DoWhileLoopAST : public ExprAST {
        const ExprAST *condition;
        const ExprAST *body;
    public:
        explicit DoWhileLoopAST (const ExprAST *condition, const ExprAST *body)
                : condition (condition), body (body) {}
        virtual ~DoWhileLoopAST() {}
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "while-loop{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "body:" << std::endl;
            if (body != NULL)
                body->print (indent+2);

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "condition:" << std::endl;
            if (condition != NULL)
                condition->print (indent+2);

            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
};


class FunProtoAST : public ExprAST {
    public:
        typedef struct Argument {
            const Datatype type;
            const std::string name;
            Argument (Datatype type, const std::string &name) : type (type), name (name) {}
        };
    private:
        const std::string name;
        const std::vector<Argument> args;
    public:
        explicit FunProtoAST (const std::string &name, const std::vector<Argument> &args)
                : name (name), args (args) {}
        virtual ~FunProtoAST() {}
};

class FunAST : public ExprAST {
        const FunProtoAST *prototype;
        const ExprAST *body;
    public:
        explicit FunAST (const FunProtoAST *prototype, const ExprAST *body)
                : prototype (prototype), body (body) {}
        virtual ~FunAST() {}
};





#endif // AST_H_INCLUDED
