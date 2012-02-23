#ifndef TOQUE_H_INCLUDED_20120125
#define TOQUE_H_INCLUDED_20120125

#include "Token.h"
#include <deque>
#include <iosfwd>
#include <boost/optional.hpp>
#include <initializer_list>

namespace quatsch { namespace compiler { namespace phase1 {

using boost::optional;

class Toque /*final*/ { // in the veins of deque, token deque.
public:

        typedef std::deque<Token>::const_iterator const_iterator;

        bool empty () const;
        size_t size() const;

        void emplace (code_iterator const &from,
                      code_iterator const &to,
                      Token::Type type
                     );

        void push (Token const &tok) ;
        void pop  ()                 ;
        void pop  (size_t n)         ;

        Token take () ;

        bool is_seq (std::initializer_list<Token::Type>) const;

        template <typename ...T>
        bool is_seq (T ...types) const
        {
                return is_seq({types...});
        }


        Token           front ()           const ;
        optional<Token> peek  (size_t i=1) const ;

        Token operator[] (size_t i) const ;


        const_iterator begin() const ;
        const_iterator end  () const ;


private:
        std::deque<Token> tokens_;
};

} } }

#endif // TOQUE_H_INCLUDED_20120125
