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
#include <vector>

// the following code is clearly inspired by the kaleidoscope tutorial on llvm.org

typedef enum {
    int_type,
    float_type
} Datatype;



// Forward declare all Node-Types.
class ExprAST;
class FloatExprAST;
class IntExprAST;
class IdExprAST;
class CallExprAST;
class BinaryExprAST;
class AssignmentExprAST;
class DeclarationAST;
class BlockAST;
class IfBlockAST;
class WhileLoopAST;
class DoWhileLoopAST;
class FunProtoAST;
class FunAST;


// Visitor.
// Update: okay, this is cool for pretty printing or ast-dumping or so. But not the right for LLVM-IR. Below is better template.
class ASTVisitor {
    public:
        virtual ~ASTVisitor() {};

        virtual void visit (const FloatExprAST*) = 0;
        virtual void visit (const IntExprAST*) = 0;
        virtual void visit (const IdExprAST*) = 0;
        virtual void visit (const CallExprAST*) = 0;
        virtual void visit (const BinaryExprAST*) = 0;
        virtual void visit (const AssignmentExprAST*) = 0;
        virtual void visit (const DeclarationAST*) = 0;
        virtual void visit (const BlockAST*) = 0;
        virtual void visit (const IfBlockAST*) = 0;
        virtual void visit (const WhileLoopAST*) = 0;
        virtual void visit (const DoWhileLoopAST*) = 0;
        virtual void visit (const FunProtoAST*) = 0;
        virtual void visit (const FunAST*) = 0;

        virtual void end (const FloatExprAST*) = 0;
        virtual void end (const IntExprAST*) = 0;
        virtual void end (const IdExprAST*) = 0;
        virtual void end (const CallExprAST*) = 0;
        virtual void end (const BinaryExprAST*) = 0;
        virtual void end (const AssignmentExprAST*) = 0;
        virtual void end (const DeclarationAST*) = 0;
        virtual void end (const BlockAST*) = 0;
        virtual void end (const IfBlockAST*) = 0;
        virtual void end (const WhileLoopAST*) = 0;
        virtual void end (const DoWhileLoopAST*) = 0;
        virtual void end (const FunProtoAST*) = 0;
        virtual void end (const FunAST*) = 0;
};

class ASTNonRecursingVisitor { // In contrast to Visitor, where a container Node also calls it's childrens' accept
    public:
        virtual ~ASTNonRecursingVisitor() {}
        virtual void visit (const FloatExprAST*) = 0;
        virtual void visit (const IntExprAST*) = 0;
        virtual void visit (const IdExprAST*) = 0;
        virtual void visit (const CallExprAST*) = 0;
        virtual void visit (const BinaryExprAST*) = 0;
        virtual void visit (const AssignmentExprAST*) = 0;
        virtual void visit (const DeclarationAST*) = 0;
        virtual void visit (const BlockAST*) = 0;
        virtual void visit (const IfBlockAST*) = 0;
        virtual void visit (const WhileLoopAST*) = 0;
        virtual void visit (const DoWhileLoopAST*) = 0;
        virtual void visit (const FunProtoAST*) = 0;
        virtual void visit (const FunAST*) = 0;
};


// Implementation.
class ExprAST {
    public:
        /// \todo store type (?)
        virtual ~ExprAST() {}
        virtual void print (int indent = 0) const = 0;/*{
            AST_PRINT_DO_INDENT (indent);
            std::cout << "expr" << std::endl;
        }*/
        virtual void accept (ASTVisitor &visitor) const = 0;
        virtual void accept (ASTNonRecursingVisitor &visitor) const = 0;
};

class FloatExprAST : public ExprAST {
    protected:
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
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class IntExprAST : public ExprAST {
    protected:
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
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class IdExprAST : public ExprAST {
    protected:
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
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class CallExprAST : public ExprAST {
        const std::string callee;
        const std::vector<ExprAST*> args;
    public:
        explicit CallExprAST (const std::string &callee, const std::vector<ExprAST*> &args)
                : callee (callee), args (args) {}
        virtual ~CallExprAST() {
            std::vector<ExprAST*>::const_iterator it = args.begin();
            while (it != args.end()) {
                if (*it) delete *it;
                ++it;
            }
        }
        const std::string &getCallee() const {
            return callee;
        }
        const std::vector<ExprAST*> &getArgs() const {
            return args;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            std::vector<ExprAST*>::const_iterator it = args.begin();
            while (it != args.end()) {
                if(0 != (*it)) (*it)->accept (visitor);
                ++it;
            }
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class BinaryExprAST : public ExprAST {
        const char op;
        const ExprAST *lhs;
        const ExprAST *rhs;
        /*BinaryExprAST(const BinaryExprAST&) : op('\0'), lhs(0), rhs(0) {}
        const BinaryExprAST & operator = (const BinaryExprAST&) { return *this; }*/
    public:
        explicit BinaryExprAST (char op, const ExprAST *lhs, const ExprAST *rhs) : op (op), lhs (lhs), rhs (rhs) {}
        virtual ~BinaryExprAST() {
            if (lhs) delete lhs;
            if (rhs) delete rhs;
        }
        char getOp () const {
            return op;
        }
        const ExprAST *getLhs () const {
            return lhs;
        }
        const ExprAST *getRhs () const {
            return rhs;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "binaryOpExpr(" << op << "){" << std::endl;
            if (lhs != 0)
                lhs->print (indent + 1);
            if (rhs != 0)
                rhs->print (indent + 1);
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (lhs) lhs->accept (visitor);
            if (rhs) rhs->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class AssignmentExprAST : public ExprAST {
        const IdExprAST *id;
        const ExprAST *rhs;
    public:
        explicit AssignmentExprAST (const IdExprAST *id, const ExprAST *rhs) : id (id), rhs (rhs) {}
        virtual ~AssignmentExprAST() {
            if (id) delete id;
            if (rhs) delete rhs;
        }
        const IdExprAST *getId () const {
            return id;
        }
        const ExprAST *getRhs () const {
            return rhs;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "assignmentExpr(=){" << std::endl;
            if (id != 0)
                id->print (indent + 1);
            if (rhs != 0)
                rhs->print (indent + 1);
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (id)  id->accept (visitor);
            if (rhs) rhs->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class DeclarationAST : public ExprAST {
        const Datatype type;
        const std::string name;
    public:
        explicit DeclarationAST (Datatype type, const std::string &name) : type(type), name(name) {}
        virtual ~DeclarationAST() {}
        const Datatype getType () const {
            return type;
        }
        const std::string getName () const {
            return name;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "declare " << name << ":" << type << std::endl;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class BlockAST : public ExprAST {
        std::vector<const ExprAST *> list; // first i had an hierarchical (even when linear) block. but that was ugly to debug.
    public:
        explicit BlockAST() : list() {}
        virtual ~BlockAST() {
            std::vector<const ExprAST *>::iterator it = list.begin();
            while (it != list.end()) {
                if (0 != *it) delete *it;
                ++it;
            }
        }
        const std::vector<const ExprAST *> &getList () const {
            return list;
        }
        virtual void addTail (const ExprAST *ast) {
            list.push_back (ast);
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "block{" << std::endl;
            for (std::vector<const ExprAST*>::const_iterator it=list.begin(); it!=list.end(); ++it) {
                if ( (*it) != 0)
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
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            std::vector<const ExprAST *>::const_iterator it = list.begin();
            while (it != list.end()) {
                if(0 != (*it))
                    (*it)->accept (visitor);
                ++it;
            }
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class IfBlockAST : public ExprAST {
        const ExprAST *if_clause;
        const ExprAST *if_body;
        const ExprAST *else_body;
        bool visit_cond, visit_then, visit_else;
    public:
        explicit IfBlockAST (const ExprAST *if_clause, const ExprAST *if_body, const ExprAST *else_body)
                : if_clause (if_clause), if_body (if_body), else_body (else_body)
                , visit_cond (true), visit_then (true), visit_else (true) {}
        virtual ~IfBlockAST() {
            if (0 != if_clause) delete if_clause;
            if (0 != if_body) delete if_body;
            if (0 != else_body) delete else_body;
        }
        const ExprAST* getIfClause () const {
            return if_clause;
        }
        const ExprAST* getIfBody () const {
            return if_body;
        }
        const ExprAST* getElseBody () const {
            return else_body;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "if-block{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "if:" << std::endl;
            if (if_clause != 0)
                if_clause->print (indent+2);
            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "then:" << std::endl;
            if (if_body != 0)
                if_body->print (indent+2);
            if (else_body != 0) {
                AST_PRINT_DO_INDENT (indent+1);
                std::cout << "else:" << std::endl;
                else_body->print (indent+2);
            }
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
        /// \todo Damn, that hack below. Got get rid of it.
        void enableVisit (bool visit_cond_, bool visit_then_, bool visit_else_) const {
            const_cast<bool&>(visit_cond) = visit_cond_;
            const_cast<bool&>(visit_then) = visit_then_;
            const_cast<bool&>(visit_else) = visit_else_;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (visit_cond && 0!=if_clause) if_clause->accept (visitor);
            if (visit_then && 0!=if_body) if_body->accept (visitor);
            if (visit_else && 0!=else_body) else_body->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

// we want to avoid pure jump-ast's, so we provide two hardcoded loops (head-controlled and foot-controlled)
class WhileLoopAST : public ExprAST {
        const ExprAST *condition;
        const ExprAST *body;
    public:
        explicit WhileLoopAST (const ExprAST *condition, const ExprAST *body)
                : condition (condition), body (body) {}
        virtual ~WhileLoopAST() {
            if (condition) delete condition;
            if (body) delete body;
        }
        const ExprAST *getCondition () const {
            return condition;
        }
        const ExprAST *getBody () const {
            return body;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "while-loop{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "condition:" << std::endl;
            if (0 != condition)
                condition->print (indent+2);
            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "body:" << std::endl;
            if (0 != body)
                body->print (indent+2);
            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (condition) condition->accept (visitor);
            if (body) body->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};

class DoWhileLoopAST : public ExprAST {
        const ExprAST *condition;
        const ExprAST *body;
    public:
        explicit DoWhileLoopAST (const ExprAST *condition, const ExprAST *body)
                : condition (condition), body (body) {}
        virtual ~DoWhileLoopAST() {
            if (condition) delete condition;
            if (body) delete body;
        }
        const ExprAST *getCondition () const {
            return condition;
        }
        const ExprAST *getBody () const {
            return body;
        }
        virtual void print (int indent) const {
            AST_PRINT_DO_INDENT (indent);
            std::cout << "while-loop{" << std::endl;

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "body:" << std::endl;
            if (body != 0)
                body->print (indent+2);

            AST_PRINT_DO_INDENT (indent+1);
            std::cout << "condition:" << std::endl;
            if (condition != 0)
                condition->print (indent+2);

            AST_PRINT_DO_INDENT (indent);
            std::cout << "}" << std::endl;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (condition) condition->accept (visitor);
            if (body) body->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
};


class FunProtoAST : public ExprAST {
    public:
        struct Argument {
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

        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
        const std::string & getName() const {
            return name;
        }
        const std::vector<Argument> & getArguments() const {
            return args;
        }
        virtual void print (int indent) const {
            using namespace std;
            AST_PRINT_DO_INDENT (indent);
            cout << "fun-proto '" << name << "' (";
            std::vector<Argument>::const_iterator it;
            for (it = args.begin() ; it != args.end() ; ++it) {
                cout << it->name << ":";
                switch (it->type) {
                    case int_type:   cout << "int";   break;
                    case float_type: cout << "float"; break;
                }
                cout << "; ";
            }
            cout << ")" << endl;
        }
};

class FunAST : public ExprAST {
        const FunProtoAST *prototype;
        const ExprAST *body;
    public:
        explicit FunAST (const FunProtoAST *prototype, const ExprAST *body)
                : prototype (prototype), body (body) {}
        virtual ~FunAST() {
            if (prototype) delete prototype;
            if (body) delete body;
        }
        virtual void accept (ASTVisitor &visitor) const {
            visitor.visit (this);
            if (prototype) prototype->accept (visitor);
            if (body) body->accept (visitor);
            visitor.end(this);
        }
        virtual void accept (ASTNonRecursingVisitor &visitor) const {
            visitor.visit (this);
        }
        const FunProtoAST * getPrototype() const {
            return prototype;
        }
        const ExprAST * getBody() const {
            return body;
        }
        virtual void print (int indent) const {
            if (0 != prototype) prototype->print(indent);
            if (0 != body) body->print(indent);
        }
};





#endif // AST_H_INCLUDED
