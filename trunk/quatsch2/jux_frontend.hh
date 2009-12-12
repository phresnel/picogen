//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#ifndef JUX_FRONTEND_HH_INCLUDED_20091208
#define JUX_FRONTEND_HH_INCLUDED_20091208
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
        iterator eat_whitespace (iterator it, iterator end,
                size_t *offset = 0, size_t *line = 0, size_t *column = 0) {
                while (it != end && is_whitespace (it)) {
                        ++it;
                }
                return it;
        }

        //----------------------------------------------------------------------
        class Token {
        public:
                enum Type {
                        integer_literal,
                        float_literal,
                        identifier,

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

                Token (std::wstring::iterator begin, std::wstring::iterator end, Type type)
                : valid_(begin!=end), begin_(begin), end_(end), type_(type) {
                }

                Token (std::wstring::iterator it, Type type)
                : valid_(true), begin_(it), end_(it+1), type_(type) {
                }

                Token () : valid_(false) {}

                bool valid () const {
                        return valid_;
                }

                operator bool () const {
                        return valid_;
                }

                size_t size () const {
                        return distance (begin_, end_);
                }

                std::wstring::iterator begin() const {
                        return begin_;
                }
                std::wstring::iterator end() const {
                        return end_;
                }
                Type type() const {
                        return type_;
                }
        private:
                bool valid_;
                std::wstring::iterator begin_, end_;
                Type type_;
        };


        //----------------------------------------------------------------------
        // Tokenizer
        //----------------------------------------------------------------------
        Token tokenize_number (std::wstring::iterator it, std::wstring::iterator end) {
                typedef Token Tok;

                const std::wstring::iterator begin = it;
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

        Token tokenize_id (std::wstring::iterator it, std::wstring::iterator end) {
                typedef Token Tok;

                const std::wstring::iterator begin = it;
                if (!is_nondigit (it))
                        return Tok();
                ++it;
                while (is_digit (it) || is_nondigit (it))
                        ++it;
                return Tok(begin, it, Tok::identifier);
        }

        Token tokenize_operator (std::wstring::iterator it, std::wstring::iterator end) {
                const std::wstring::iterator begin = it;
                const std::wstring::iterator ahead = it+1;
                switch (*it) {
                // single character operators
                case '+': return Token(it,Token::plus);
                case '-': return Token(it,Token::minus);
                case '*': return Token(it,Token::asterisk);
                case '/': return Token(it,Token::slash);
                case '(': return Token(it,Token::lparen);
                case ')': return Token(it,Token::rparen);
                case '{': return Token(it,Token::lbrace);
                case '}': return Token(it,Token::rbrace);

                // multiple characters
                case '[':
                        switch (*ahead) {
                        case ']': return Token(begin,1+ahead,Token::interval_in_in);
                        case '[': return Token(begin,1+ahead,Token::interval_in_out);
                        default:  return Token(begin, Token::lsquare);
                        };
                        break;
                case ']':
                        switch (*ahead) {
                        case ']': return Token(begin,1+ahead,Token::interval_out_in);
                        case '[': return Token(begin,1+ahead,Token::interval_out_out);
                        default:  return Token(begin, Token::rsquare);
                        };
                        break;
                };
                return Token();
        }

        class Tokenizer {
        public:
                Tokenizer () {}

                void tokenize (std::wstring::iterator prog_begin, std::wstring::iterator prog_end, std::deque<Token> &tokens) {
                        std::wstring::iterator it = prog_begin;

                        typedef Token Tok;

                        const std::wstring::iterator begin = it;

                        while (it != prog_end) {
                                it = eat_whitespace (it, prog_end);
                                if (it == prog_end)
                                        break;
                                if (Tok tok = tokenize_number (it, prog_end)) {
                                        it = tok.end();
                                        tokens.push_back (tok);

                                        if (!is_punctuator (it, prog_end)) {
                                                throw std::domain_error(
                                                "tokenization error: expected punctuation at "
                                                + Position(it, prog_begin,prog_end).to_string()
                                                );
                                        }
                                } else if (Tok tok = tokenize_operator (it, prog_end)) {
                                        it = tok.end();
                                        tokens.push_back (tok);
                                        // An operator is a punctuator itself, so no need
                                        // to check for a punctuator here.
                                } else if (Tok tok = tokenize_id (it, prog_end)) {
                                        it = tok.end();
                                        tokens.push_back (tok);
                                } else {
                                        throw std::domain_error(
                                                "tokenization error: unknown char sequence at "
                                                + Position(it, prog_begin, prog_end).to_string()
                                                );
                                }
                        }
                }
        };

        //----------------------------------------------------------------------
        // Parse
        //----------------------------------------------------------------------
        class Compiler {
        private:
                typedef Token Tok;
                typedef middleend::Expression Expr;
                typedef shared_ptr<Expr> ExprPtr;
                typedef shared_ptr<Expr> null;
                typedef std::deque<Token> TokDeque;

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
                        std::wstring::iterator prog_begin,
                        std::wstring::iterator prog_end,
                        TokDeque const & tokens
                )
                : prog_begin_(prog_begin), prog_end_(prog_end)
                , tokens_(tokens), tokens(tokens_)
                , cerr(std::cerr)
                {}

                ExprPtr parse () {
                        using namespace middleend;
                        shared_ptr<ExpressionListExpr> ret (
                         new ExpressionListExpr(Position(prog_begin_, prog_begin_, prog_end_))
                        );
                        while (!tokens.empty()) {
                                if (ExprPtr expr = parse_expression()) {
                                        if (dynamic_cast<middleend::InvalidExpr*> (expr.get())) {
                                                tokens.rollback();
                                                cerr << "Expression beginning at "
                                                        << Position(tokens_[0].begin(), prog_begin_, prog_end_).to_string()
                                                        << " is invalid" << std::endl;
                                                return null();
                                        } else {
                                                tokens.commit();
                                                ret->add (expr);
                                        }
                                } else {
                                        tokens.rollback();
                                        throw std::domain_error(
                                        "Parsing error at "
                                        + Position(tokens_[0].begin(), prog_begin_, prog_end_).to_string()
                                        );
                                }
                        }
                        return ret;
                }
        private:

                template<typename ExprT_>
                ExprPtr parse_to_end (Tok const & op) {
                        typedef ExprT_ ExprT;
                        typedef middleend::InvalidExpr InvExprT;

                        std::vector<ExprPtr> parameters;

                        int param_id = 0;
                        while (ExprPtr param = parse_expression()) {
                                ++param_id;
                                if (dynamic_cast<InvExprT*> (param.get())) {
                                        cerr << "Parameter " << param_id
                                                << " of expression beginning at "
                                                << Position(op.begin(),prog_begin_, prog_end_).to_string()
                                                << " is invalid" << std::endl;
                                        return shared_ptr<InvExprT> (new InvExprT(
                                                Position(op.begin(), prog_begin_, prog_end_))
                                        );
                                }
                                parameters.push_back (param);
                        }
                        // TODO: fix so that it points after last parameter, if any
                        const std::wstring::iterator end = parameters.size() > 0
                                          ? op.begin()+parameters.back()->position().offset()
                                          : op.end();

                        bool invalid = false;
                        // Post-cond: must have at least <min_param_count> parameters
                        if (ExprT::min_param_count>=0 && parameters.size()<ExprT::min_param_count) {
                                cerr << "Not enough parameters for expression beginning at "
                                     << Position(op.begin(), prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        }
                        // Post-cond: must have at max <max_param_count> parameters
                        if (ExprT::max_param_count>=0 && (int)parameters.size()>ExprT::max_param_count) {
                                cerr << "Too many parameters for expression beginning at "
                                     << Position(op.begin(), prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        }
                        // Post-cond: must be followed by ')', i.e. not by end-of-file or anything else
                        if (tokens.size() == 0) {
                                cerr << "Unexpected end-of-file after "
                                     << Position(prog_end_, prog_begin_, prog_end_).to_string()
                                     << '\n';
                                invalid = true;
                        } else if (tokens.take_front ().type() != Tok::rparen) {
                                cerr << "Expected ')' after "
                                     << Position(end, prog_begin_, prog_end_).to_string()
                                     << ", but found something else."
                                     << '\n';
                                invalid = true;
                        }

                        if (invalid) {
                                ExprPtr ret (new InvExprT (
                                        Position(op.begin(), prog_begin_, prog_end_)
                                ));
                                return ret;
                        } else {
                                ExprPtr ret (new ExprT (
                                        Position(op.begin(), prog_begin_, prog_end_),
                                        parameters
                                ));
                                return ret;
                        }
                }

                ExprPtr parse_expression() {
                        using namespace middleend;
                        if (tokens.size() == 0)
                                return null();

                        const Tok alpha = tokens.front();

                        if (alpha.type() == Tok::integer_literal) {
                                shared_ptr<Expr> ret (new middleend::IntLiteralExpr(
                                        Position(alpha.begin(), prog_begin_, prog_end_)
                                ));
                                tokens.pop_front();
                                return ret;
                        } else if (alpha.type() == Tok::float_literal) {
                                shared_ptr<Expr> ret (new middleend::FloatLiteralExpr(
                                        Position(alpha.begin(), prog_begin_, prog_end_)
                                ));
                                tokens.pop_front();
                                return ret;
                        } else if (alpha.type() == Tok::identifier) {
                                shared_ptr<Expr> ret (new middleend::IdExpr(
                                        Position(alpha.begin(), prog_begin_, prog_end_),
                                        std::wstring(alpha.begin(), alpha.end())
                                ));
                                tokens.pop_front();
                                return ret;
                        } else if (alpha.type() == Tok::rparen) {
                                return null();
                        } else if (alpha.type() != Tok::lparen) {
                                ExprPtr ret (new middleend::InvalidExpr (
                                        Position(alpha.begin(), prog_begin_, prog_end_)
                                ));
                                return ret;
                        }
                        tokens.pop_front();

                        const Tok op = tokens.take_front();

                        switch (op.type()) {
                        case Tok::integer_literal:
                        case Tok::float_literal:
                        case Tok::identifier:
                        case Tok::lparen: case Tok::rparen: // ( )
                        case Tok::lsquare: case Tok::rsquare: // [ ]
                        case Tok::lbrace: case Tok::rbrace: // { }
                                return null();
                        case Tok::plus: return parse_to_end<AdditionExpr> (op);
                        case Tok::minus: return parse_to_end<SubtractionExpr> (op);
                        case Tok::asterisk: return parse_to_end<MultiplicationExpr> (op);
                        case Tok::slash: return parse_to_end<DivisionExpr> (op);

                        case Tok::interval_in_in:
                        case Tok::interval_in_out:
                        case Tok::interval_out_in:
                        case Tok::interval_out_out:
                                return null();
                        };

                        return null();
                }

        private:
                const std::wstring::iterator prog_begin_, prog_end_;
                std::deque<Token> tokens_;
                Foo tokens;
                std::ostream &cerr;
        };


        void dbg_dump_tokens (std::deque<Token> const & tokens) {
                typedef Token Tok;
                std::cout << '[';
                for (std::deque<Tok>::const_iterator it = tokens.begin();
                     it != tokens.end();
                     ++it
                ) {
                        switch (it->type()) {
                        case Tok::integer_literal: std::cout << "integer_literal"; break;
                        case Tok::float_literal: std::cout << "float_literal"; break;
                        case Tok::identifier: std::cout << "id"; break;
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
#endif // JUX_FRONTEND_HH_INCLUDED_20091208
