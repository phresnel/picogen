#include "Token.h"
#include <ostream>
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


std::ostream& operator<< (std::ostream &os, Token::Type t)
{
        switch (t) {
        case Token::Type::Defun: os << "defun"; break;
        case Token::Type::Identifier: os << "id"; break;
        case Token::Type::Integer:    os << "int"; break;
        case Token::Type::Floating:    os << "float"; break;
        case Token::Type::Colon:    os << "colon"; break;
        case Token::Type::OpenParen:  os << "("; break;
        case Token::Type::CloseParen: os << ")"; break;
        case Token::Type::OpenBracket:  os << "["; break;
        case Token::Type::CloseBracket: os << "]"; break;
        case Token::Type::TemplateArg:  os << "tpl-arg"; break;
        case Token::Type::Operator: os << "O"; break;
        }
        return os;
}
std::ostream& operator<< (std::ostream &os, Token const &tok)
{
        os << "'" << std::string(tok.begin().str_iter(),
                                 tok.end().str_iter()) << "'"
           << ':' << tok.type();
        return os;
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
