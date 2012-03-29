#include "ErrorState.h"

namespace quatsch {

bool ErrorState::has_errors() const
{
        return !errors_.empty();
}

void ErrorState::post_error (std::string const &msg,
                             code_iterator from,
                             code_iterator to)
{
        errors_.emplace_back (msg, code_range{from, to});
}

ErrorState::const_iterator ErrorState::begin() const
{
        return errors_.begin();
}

ErrorState::const_iterator ErrorState::end() const
{
        return errors_.end();
}

ErrorState::const_iterator begin (ErrorState const& e)
{
        return e.begin();
}

ErrorState::const_iterator end   (ErrorState const& e)
{
        return e.end();
}

}
