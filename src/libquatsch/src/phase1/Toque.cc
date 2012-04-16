#include "Toque.h"
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase1 {


bool Toque::empty() const
{
        return tokens_.empty();
}


size_t Toque::size() const
{
        return tokens_.size();
}


void Toque::emplace (code_iterator const &from,
                     code_iterator const &to,
                     Token::Type type)
{
        tokens_.emplace_back (from, to, type);
}


void Toque::push (Token const &tok)
{
        tokens_.push_back (tok);
}


void Toque::pop ()
{
        if (empty())
                throw std::runtime_error ("Toque::pop() called on empty toque");
        tokens_.pop_front();
}

void Toque::pop (size_t n)
{
        while (n) {
                --n;
                pop();
        }
}

Token Toque::take()
{
        Token ret = front();
        pop();
        return ret;
}

Token Toque::front() const
{
        if (empty())
                throw std::runtime_error ("Toque::front() called on empty toque");
        return tokens_.front();
}


optional<Token> Toque::peek (size_t i) const
{
        //if (i<0) throw std::runtime_error ("Toque::peek(i) with i<0 called");
        if (i>=tokens_.size()) return optional<Token>();
        return tokens_[i];
}


Token Toque::operator[] (size_t i) const
{
        return tokens_[i];
}


Toque::const_iterator Toque::begin() const
{
        return tokens_.begin();
}


Toque::const_iterator Toque::end() const
{
        return tokens_.end();
}

bool Toque::is_seq (std::initializer_list<Token::Type> toks) const
{
        if (toks.size() > size())
                return false;
        auto lit = begin();
        auto rit = toks.begin(), rend = toks.end();
        for (; rit != rend; ++lit, ++rit) {
                if (*lit != *rit) {
                        return false;
                }
        }
        return true;
}

} } }
