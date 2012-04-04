#include "Token.h"
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase1 {

Token::Token (code_iterator const &from,
              code_iterator const &to,
              Type type) :
        from_(from), to_(to), type_(type)
{
        if (from == to)
                throw std::runtime_error("Token with zero length created.");
}


code_iterator Token::begin() const
{
        return from_;
}


code_iterator Token::end() const
{
        return to_;
}


Token::Type Token::type() const
{
        return type_;
}

bool operator== (Token const& tok, Token::Type t)
{
        return tok.type() == t;
}

bool operator!= (Token const& tok, Token::Type t)
{
        return !(tok == t);
}

} } }
