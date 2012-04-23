#ifndef QUATSCH_EXCEPT_H_20120118
#define QUATSCH_EXCEPT_H_20120118

#include <stdexcept>

namespace quatsch { namespace compiler {

        class quatsch_exception : public std::exception {
        public:
                virtual ~quatsch_exception() throw() {}
                virtual const char *what() const throw()  { return "quatsch exception"; }
        };

        class missing_entry_point : public quatsch_exception {
        public:
                virtual ~missing_entry_point() throw() {}
                virtual const char *what() const throw()  { return "missing entry point"; }
        };

        class multiple_entry_points : public quatsch_exception {
        public:
                virtual ~multiple_entry_points() throw() {}
                virtual const char *what() const throw()  { return "multiple entry points"; }
        };

        class tokenization_error : public quatsch_exception {
        public:
                virtual ~tokenization_error() throw() {}
                virtual const char *what() const throw() { return "tokenization error"; }
        };


        class unknown_symbol : public quatsch_exception {
        public:
                virtual ~unknown_symbol() throw() {}
                virtual const char *what() const throw() { return "unknown symbol"; }
        };

        class unknown_function : public quatsch_exception {
        public:
                virtual ~unknown_function() throw() {}
                virtual const char *what() const throw() { return "unknown function"; }
        };

        class unknown_typename : public quatsch_exception {
        public:
                virtual ~unknown_typename() throw() {}
                virtual const char *what() const throw() { return "unknown typename"; }
        };

        class unexpected : public quatsch_exception {
        public:
                virtual ~unexpected() throw() {}
                virtual const char *what() const throw() { return "unexpected"; }
        };

        class unexpected_eof : public unexpected {
        public:
                virtual ~unexpected_eof() throw() {}
                virtual const char *what() const throw() { return "unexpected end of file"; }
        };

        class expression_expected : public unexpected {
        public:
                virtual ~expression_expected() throw() {}
                virtual const char *what() const throw() { return "expected an expression"; }
        };

        class constexpr_expected : public unexpected {
        public:
                virtual ~constexpr_expected() throw() {}
                virtual const char *what() const throw() { return "expected a constant expression"; }
        };
} }

#endif // QUATSCH_EXCEPT_H_20120118

