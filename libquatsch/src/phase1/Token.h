#ifndef TOKEN_H_INCLUDED_20120125
#define TOKEN_H_INCLUDED_20120125

#include "code_iterator.h"
#include <iosfwd>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

namespace quatsch { namespace compiler { namespace phase1 {

        using boost::optional;

        class Token {
        public:
                enum Type {
                        Defun, Let,
                        Identifier,
                        Integer, Floating, Colon,
                        OpenParen, CloseParen,
                        OpenBracket, CloseBracket,
                        //OpenCurly, CloseCurly,
                        //TemplateArg,
                        TemplateArg_Integer,
                        TemplateArg_Floating,
                        TemplateArg_String,
                        Operator
                };

                Token (code_iterator const &from,
                       code_iterator const &to,
                       Type type);

                code_iterator begin() const ;
                code_iterator end  () const ;

                Type type() const ;

                template <typename T> T value() const
                {
                        auto from_iter = from_.str_iter();
                        auto to_iter   = to_.str_iter();
                        if (type() == Type::Integer) --to_iter; // minus suffix
                        return boost::lexical_cast<T> (std::string(from_iter,
                                                                   to_iter));
                }

        private:
                code_iterator from_, to_;
                Type type_;
        };

        bool operator== (Token const&tok, Token::Type t);
        bool operator!= (Token const&tok, Token::Type t);

} } }

#endif // TOKEN_H_INCLUDED_20120125
