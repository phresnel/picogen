#ifndef ERROR_STATE_H_INCLUDED_20120329
#define ERROR_STATE_H_INCLUDED_20120329

#include "code_iterator.h"
#include <list>
#include <utility>

namespace quatsch {

class ErrorState {
public:
        typedef std::pair<code_iterator, code_iterator> code_range;
        typedef std::pair<std::string, code_range> message_code_range_pair;
        typedef std::list<message_code_range_pair>::const_iterator const_iterator;

        bool has_errors() const;
        void post_error (std::string const &msg,
                         code_iterator from, code_iterator to);

        const_iterator begin() const ;
        const_iterator end  () const ;
private:
        std::list<message_code_range_pair> errors_;
};
ErrorState::const_iterator begin (ErrorState const& e) ;
ErrorState::const_iterator end   (ErrorState const& e) ;

}

#endif // ERROR_STATE_H_INCLUDED_20120329
