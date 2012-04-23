#ifndef TOKENIZE_H_INCLUDED_20120125
#define TOKENIZE_H_INCLUDED_20120125

#include "Toque.h"
#include "ErrorState.h"

namespace quatsch { namespace compiler { namespace phase1 {
        Toque tokenize (code_iterator it, code_iterator end, ErrorState &);
} } }

#endif // TOKENIZE_H_INCLUDED_20120125
