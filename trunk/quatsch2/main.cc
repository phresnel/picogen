#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <sstream>

#include <stdexcept>
#include <deque>
#include <stack>
#include <vector>

namespace quatsch2 { 
        using boost::optional;
        using boost::shared_ptr;
}

namespace quatsch2 { namespace frontend { namespace jux {
        // inspired by C++ grammar notation
        template <typename iterator>
        bool is_digit (iterator it) {
                switch (*it) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                        return true;
                };
                return false;
        }

        template <typename iterator>
        bool is_nondigit (iterator it) {
                switch (*it) {
                case '_':
                case 'a':
                case 'A':
                case 'b':
                case 'B':
                case 'c':
                case 'C':
                case 'd':
                case 'D':
                case 'e':
                case 'E':
                case 'f':
                case 'F':
                case 'g':
                case 'G':
                case 'h':
                case 'H':
                case 'i':
                case 'I':
                case 'j':
                case 'J':
                case 'k':
                case 'K':
                case 'l':
                case 'L':
                case 'm':
                case 'M':
                case 'n':
                case 'N':
                case 'o':
                case 'O':
                case 'p':
                case 'P':
                case 'q':
                case 'Q':
                case 'r':
                case 'R':
                case 's':
                case 'S':
                case 't':
                case 'T':
                case 'u':
                case 'U':
                case 'v':
                case 'V':
                case 'w':
                case 'W':
                case 'x':
                case 'X':
                case 'y':
                case 'Y':
                case 'z':
                case 'Z':
                        return true;
                };
                return false;
        }

        template <typename iterator>
        bool is_identifier (iterator curr, iterator end) {
                if (!is_nondigit (*curr))
                        return false;
                for (++curr; curr!=end; ++curr) {
                        if (!is_digit (*curr) && !is_nondigit (*curr))
                                return false;
                }
                return true;
        }

        template <typename T>
        bool is_identifier (T const &str) {
                return is_identifier (str.begin(), str.end());
        }


        
        template <typename iterator>
        bool follows_operator (iterator begin, iterator end) {                
                return tokenize_operator (begin, end).valid();                
        }


        template <typename iterator>
        bool is_whitespace (iterator it) {
                switch (*it) {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                        return true;
                };
                return false;
        }

        template <typename iterator>
        bool is_punctuator (iterator it, iterator end) {
                if (follows_operator (it, end))
                        return true;
                if (is_whitespace (it))
                        return true;
                if (it == end) // eof surely is a punctuator, too
                        return true;
                switch (*it) {
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                        return true;
                };
                return false;
        }


        //----------------------------------------------------------------------
        template <typename iterator>
        iterator eat_whitespace (iterator it, iterator end) {
                while (it != end && is_whitespace (it))
                        ++it;
                return it;
        }

        //----------------------------------------------------------------------
        template <typename iterator>
        class Token {
        public:
                enum Type {
                        integer_literal,
                        float_literal,
                        
                        plus,
                        minus,
                        asterisk,
                        slash,
                        
                        interval_in_in,
                        interval_in_out,
                        interval_out_in,
                        interval_out_out,
                        
                        lparen, rparen, // ( )
                        lsquare, rsquare, // [ ]
                        lbrace, rbrace // { }
                };

                Token (iterator begin, iterator end, Type type)
                                : begin_(begin), end_(end), type_(type) {
                }
                
                Token (iterator it, Type type)
                                : begin_(it), end_(it+1), type_(type) {
                }
                
                Token () : begin_(0), end_(begin_) {}

                bool valid () const {
                        return begin_ != end_;
                }

                operator bool () const {
                        return valid();
                }

                size_t size () const {
                        return distance (begin_, end_);
                }

                iterator begin() const {
                        return begin_;
                }
                iterator end() const {
                        return end_;
                }
                Type type() const {
                        return type_;
                }
        private:

                iterator begin_, end_;
                Type type_;
        };
        
        class Position {
                size_t line_, column_;
        public:
                template <typename iterator>
                Position (iterator current, iterator begin, iterator end) {
                        line_ = 1;
                        column_ = 1;
                        for (iterator it=begin; it!=end && it!=current;  ++it) {
                                if (*it == '\n') {
                                        ++line_;
                                        column_ = 1;
                                } else {
                                        column_++;
                                }
                        }
                }
                
                size_t line () const { return line_; }
                size_t column () const { return column_; }
                
                std::string to_string () const {
                        std::stringstream ss;
                        ss << line_ << ":" << column_;
                        return ss.str();
                }
        };

        //----------------------------------------------------------------------
        // Tokenizer
        //----------------------------------------------------------------------
        template <typename iterator>
        Token<iterator> tokenize_number (iterator it, iterator end) {
                typedef Token<iterator> Tok;

                const iterator begin = it;
                while (is_digit (it))
                        ++it;
                switch (*it) {
                default:
                        return Tok(begin, it, Tok::integer_literal);
                case '.': ; // fallthrough
                        // in future, integer suffixes could be here
                };
                ++it;

                while (is_digit (it))
                        ++it;

                // At this point, it is possible that only "." has been parsed,
                // which would yield a distance of <= 1. Catch that:
                if (distance (begin, it) <= 1)
                        return Tok();

                return Tok(begin, it, Tok::float_literal);
        }
        
        template <typename iterator>
        Token<iterator> tokenize_operator (iterator it, iterator end) {
                typedef Token<iterator> Tok;
                const iterator begin = it;
                const iterator ahead = it+1;
                switch (*it) {
                // single character operators
                case '+': return Tok(it,Tok::plus);
                case '-': return Tok(it,Tok::minus);
                case '*': return Tok(it,Tok::asterisk);
                case '/': return Tok(it,Tok::slash);
                case '(': return Tok(it,Tok::lparen);
                case ')': return Tok(it,Tok::rparen);
                case '{': return Tok(it,Tok::lbrace);
                case '}': return Tok(it,Tok::rbrace);
                        
                // multiple characters
                case '[':
                        switch (*ahead) {
                        case ']': return Tok(begin,1+ahead,Tok::interval_in_in);
                        case '[': return Tok(begin,1+ahead,Tok::interval_in_out);
                        default:  return Tok(begin, Tok::lsquare);
                        };
                        break;
                case ']':
                        switch (*ahead) {
                        case ']': return Tok(begin,1+ahead,Tok::interval_out_in);
                        case '[': return Tok(begin,1+ahead,Tok::interval_out_out);
                        default:  return Tok(begin, Tok::rsquare);
                        };
                        break;
                };                
                return Tok();
        }

        template <typename iterator>
        void tokenize (iterator it, iterator end, std::deque<Token<iterator> > &tokens) {
                typedef Token<iterator> Tok;

                const iterator begin = it;

                while (it != end) {
                        it = eat_whitespace (it, end);
                        if (it == end)
                                break;
                        if (Tok tok = tokenize_number (it, end)) {
                                it = tok.end();
                                tokens.push_back (tok);

                                if (!is_punctuator (it, end)) {
                                        throw std::domain_error(
                                        "tokenization error: expected punctuation at "
                                        + Position (it, begin, end).to_string()
                                        );
                                }
                        } else if (Tok tok = tokenize_operator (it, end)) {
                                it = tok.end();
                                tokens.push_back (tok);                                
                                // An operator is a punctuator itself, so no need
                                // to check for a punctuator here.
                        } else {
                                throw std::domain_error(
                                        "tokenization error: unknown char sequence at "
                                        + Position (it, begin, end).to_string()
                                        );
                        }
                }
        }
        
        
        //----------------------------------------------------------------------
        // Parse
        //----------------------------------------------------------------------
        enum {
                constructors_shout = 0
        };
        template <typename iterator> class Expression {
        public:
                Expression (iterator begin, iterator end)
                : begin_(begin), end_(end)
                {}
                        
                virtual ~Expression() {}

                iterator begin() const {
                        return begin_;
                }
                
                iterator end() const { 
                        return end_;
                }
                
                bool valid () const {
                        return begin_ != end_;
                }

                operator bool () const {
                        return valid();
                }

                size_t size () const {
                        return distance (begin_, end_);
                }
        private:
                iterator begin_, end_;
        };
        
        template <typename iterator>
        class InvalidExpr : public Expression<iterator> {
        public:
                InvalidExpr (iterator begin, iterator end)
                : Expression<iterator> (begin, end)
                {
                        if (constructors_shout) {
                                std::cout << "created invalid-expr" << std::endl;
                        }
                }
                
                virtual ~InvalidExpr() {}
        private:
        };
        
        template <typename iterator>
        class AdditionExpr : public Expression<iterator> {
        public:
                AdditionExpr(iterator begin, iterator end, 
                        std::vector<shared_ptr<Expression<iterator> > > const & parameters)
                : Expression<iterator> (begin, end)
                {
                        if (constructors_shout) {
                                std::cout << "created addition: {";
                                for (iterator it=begin; it!=end; ++it)
                                        std::cout << *it;
                                std::cout << "}" << std::endl;
                        }
                }
                
                virtual ~AdditionExpr() {}
                
                enum {
                        min_param_count = 1,
                        max_param_count = -1
                };
        private:
        };
        
        template <typename iterator>
        class FloatLiteralExpr : public Expression<iterator> {
        public:
                FloatLiteralExpr(iterator begin, iterator end)
                : Expression<iterator> (begin, end)
                {
                        if (constructors_shout) {
                                std::cout << "created float-literal: {";
                                for (iterator it=begin; it!=end; ++it)
                                        std::cout << *it;
                                std::cout << "}" << std::endl;
                        }
                }
                
                virtual ~FloatLiteralExpr() {}
        private:
        };
        
        template <typename iterator>
        class IntLiteralExpr : public Expression<iterator> {
        public:
                IntLiteralExpr(iterator begin, iterator end)
                : Expression<iterator> (begin, end)
                {
                        if (constructors_shout) {
                                std::cout << "created int-literal: {";
                                for (iterator it=begin; it!=end; ++it)
                                        std::cout << *it;
                                std::cout << "}" << std::endl;
                        }
                }
                
                virtual ~IntLiteralExpr() {}
        private:
        };



        template <typename iterator>
        class Compiler {
        private:
                typedef Token<iterator> Tok;
                typedef Expression<iterator> Expr;
                typedef shared_ptr<Expr> ExprPtr;
                typedef shared_ptr<Expr> null;       
                typedef std::deque<Token<iterator> > TokDeque;
                
                class Foo {
                        TokDeque &tokens;
                        std::stack<Tok> rollbackStack;
                public:
                        void commit () {
                                while (!rollbackStack.empty())
                                        rollbackStack.pop();
                        }

                        void rollback () {
                                while (!rollbackStack.empty()) {
                                        tokens.push_front (rollbackStack.top());
                                        rollbackStack.pop();
                                }
                        }

                        Foo (TokDeque &tokens) : tokens(tokens)
                        {}
                                
                        size_t size() const {
                                return tokens.size();
                        }
                        
                        bool empty() const {
                                return tokens.empty();
                        }                                

                        void pop_front() {
                                rollbackStack.push(tokens.front());
                                tokens.pop_front();
                        }
                        
                        Tok front () {
                                return tokens.front();
                        }
                        
                        Tok take_front () {
                                if (tokens.size() == 0) {
                                        throw std::domain_error (
                                        "Unexpected end of file"
                                        );
                                }
                                Tok ret = tokens.front();
                                pop_front();
                                return ret;
                        }
                                
                        ~Foo () {
                                rollback();
                        }
                };
                
        public:
                
                Compiler (
                        iterator prog_begin, iterator prog_end,
                        TokDeque const & tokens
                )
                : prog_begin_(prog_begin), prog_end_(prog_end)
                , tokens_(tokens), tokens(tokens_)
                , cerr(std::cerr)
                {}
                
                void parse () {
                        while (!tokens.empty()) {
                                if (ExprPtr expr = parse_expression()) {
                                        if (dynamic_cast<InvalidExpr<iterator>*> (expr.get())) {
                                                tokens.rollback();
                                                cerr << "Expression beginning at "
                                                        << Position(tokens_[0].begin(), prog_begin_, prog_end_).to_string()
                                                        << " is invalid" << std::endl;
                                                std::cout << "Compiler token dump: ";dbg_dump_tokens (tokens_);
                                                break;
                                        } else {
                                                tokens.commit();
                                        }
                                } else {
                                        tokens.rollback();
                                        std::cout << "Compiler token dump: ";dbg_dump_tokens (tokens_);
                                        throw std::domain_error(
                                        "Parsing error at "
                                        + Position (tokens_[0].begin(), prog_begin_, prog_end_).to_string()
                                        );
                                }
                        }
                }
        private:
                
                template<template <typename> class ExprT_>
                ExprPtr parse_to_end (Tok const & op) {
                        typedef ExprT_<iterator> ExprT;
                        typedef InvalidExpr<iterator> InvExprT;

                        std::vector<ExprPtr> parameters;
                        
                        int param_id = 0;
                        while (ExprPtr param = parse_expression()) {
                                ++param_id;
                                if (dynamic_cast<InvExprT*> (param.get())) {
                                        cerr << "Parameter " << param_id
                                                << " of expression beginning at "
                                                << Position(op.begin(),prog_begin_,prog_end_).to_string()
                                                << " is invalid" << std::endl;
                                        return shared_ptr<InvExprT> (new InvExprT(op.begin(), op.end()));
                                }
                                parameters.push_back (param);
                        }
                        const iterator end = parameters.size()>0?parameters.back()->end():op.end();

                        bool invalid = false;
                        // Post-cond: must have at least <min_param_count> parameters
                        if (ExprT::min_param_count>=0 && parameters.size()<ExprT::min_param_count) {
                                cerr << "Not enough parameters for expression beginning at "
                                     << Position (op.begin(), prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        }
                        // Post-cond: must have at max <max_param_count> parameters
                        if (ExprT::max_param_count>=0 && parameters.size()>ExprT::max_param_count) {
                                cerr << "Too many parameters for expression beginning at "
                                     << Position (op.begin(), prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        }
                        // Post-cond: must be followed by ')', i.e. not by end-of-file or anything else
                        if (tokens.size() == 0) {
                                cerr << "Unexpected end-of-file at "
                                     << Position (end, prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        } else if (tokens.take_front ().type() != Tok::rparen) {
                                cerr << "Expected ')' at "
                                     << Position (end, prog_begin_, prog_end_).to_string()
                                     << ", but found something else."
                                     << '\n';
                                invalid = true;
                        }
                        
                        if (invalid) {
                                ExprPtr ret (new InvExprT (
                                        op.begin(), end 
                                ));                                
                                return ret;
                        } else {
                                ExprPtr ret (new ExprT (
                                        op.begin(), end, 
                                        parameters
                                ));                                
                                return ret;
                        }
                }

                ExprPtr parse_expression() {
                        const Tok alpha = tokens.front();
                        
                        if (alpha.type() == Tok::integer_literal) {                                
                                shared_ptr<Expr> ret (new IntLiteralExpr<iterator>(
                                        alpha.begin(), alpha.end()
                                ));
                                tokens.pop_front();
                                return ret;
                        } else if (alpha.type() == Tok::float_literal) {                                
                                shared_ptr<Expr> ret (new FloatLiteralExpr<iterator>(
                                        alpha.begin(), alpha.end()
                                ));
                                tokens.pop_front();
                                return ret;
                        } else if (alpha.type() == Tok::rparen) {
                                return null();
                        } else if (alpha.type() != Tok::lparen) {
                                ExprPtr ret (new InvalidExpr<iterator> (
                                        alpha.begin(), alpha.end()
                                ));                                
                                return ret;
                        }
                        tokens.pop_front();
                        
                        const Tok op = tokens.take_front();

                        switch (op.type()) {
                        case Tok::integer_literal: 
                        case Tok::float_literal:                        
                        case Tok::lparen: case Tok::rparen: // ( )
                        case Tok::lsquare: case Tok::rsquare: // [ ]
                        case Tok::lbrace: case Tok::rbrace: // { }
                                return null();
                        case Tok::plus: {
                                return parse_to_end<AdditionExpr> (op);
                        }                        
                        case Tok::minus:
                        case Tok::asterisk:
                        case Tok::slash:
                                
                        case Tok::interval_in_in:
                        case Tok::interval_in_out:
                        case Tok::interval_out_in:
                        case Tok::interval_out_out:
                                return null();
                        };
                        
                        return null();
                }                
                
        private:        
                const iterator prog_begin_, prog_end_;
                std::deque<Token<iterator> > tokens_;
                Foo tokens;
                std::ostream &cerr;
        };
        
        
        template <typename iterator>
        void dbg_dump_tokens (std::deque<Token<iterator> > const & tokens) {
                typedef Token<iterator> Tok;
                std::cout << '[';
                for (typename std::deque<Tok>::const_iterator it = tokens.begin();
                     it != tokens.end();
                     ++it
                ) {                
                        switch (it->type()) {
                        case Tok::integer_literal: std::cout << "integer_literal"; break;
                        case Tok::float_literal: std::cout << "float_literal"; break;
                        case Tok::plus: std::cout << "+"; break;
                        case Tok::minus: std::cout << "-"; break;
                        case Tok::asterisk: std::cout << "*"; break;
                        case Tok::slash: std::cout << "/"; break;
                        case Tok::interval_in_in: std::cout << "[]"; break;
                        case Tok::interval_in_out: std::cout << "[["; break;
                        case Tok::interval_out_in: std::cout << "]]"; break;
                        case Tok::interval_out_out: std::cout << "]["; break;
                        case Tok::lparen: std::cout << "("; break;                
                        case Tok::rparen: std::cout << ")"; break;                
                        case Tok::lsquare: std::cout << "["; break;                
                        case Tok::rsquare: std::cout << "]"; break;
                        case Tok::lbrace: std::cout << "{"; break;                
                        case Tok::rbrace: std::cout << "}"; break;                
                        };                        
                        std::cout << ' ';
                }
                std::cout << "]\n";
        }
} } }

#include <iostream>
int main () {
        using namespace quatsch2::frontend::jux;
        typedef Token<std::string::iterator> Tok;
        std::string str = 
"(+ 1.0 (+ (+ 2) 3))\n"
"(+ +)";
               
        std::deque<Tok> tokens;
        tokenize (str.begin(), str.end(), tokens);
        dbg_dump_tokens (tokens);
        
        try {
                //parse (str.begin(), str.end(), tokens);
                Compiler<std::string::iterator> compiler (str.begin(), str.end(), tokens);
                compiler.parse();
        } catch (std::domain_error const &exc) {
                std::cout << exc.what() << '\n';
        }

        std::cout << "\n";
}
