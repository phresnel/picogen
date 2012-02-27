#include <iostream>
#include <deque>
#include <boost/optional.hpp>

#include "phase1/tokenize.h"
#include "phase2/parse.h"
#include "phase3/resolve_and_verify.h"
#include "phase5/C99/to_C99.h"


#include <initializer_list>
#include <vector>
#include <stdexcept>
namespace quatsch { namespace extern_template {
        class StaticArgument {
        public:
                enum Type {
                        String,
                        Integer,
                        Float
                };

                StaticArgument (std::string const &name,
                                Type type)
                        : name_(name), type_(type)
                {}

                std::string name() const { return name_; }
                Type        type() const { return type_; }

        private:
                std::string name_;
                Type type_;
        };
        class Template {
        public:
                virtual ~Template() ;

                bool static_argument_exists (std::string const &name) const;
                StaticArgument static_argument (std::string const &name) const;
        protected:
                Template (std::initializer_list<StaticArgument>) ;

                /* boils down to this with delegating constructors
                template <typename ...Args>
                Template (Args ...args) : Template ({args...}) {}
                */

        private:
                std::vector<StaticArgument> static_args_;
        };

        Template::Template (std::initializer_list<StaticArgument>)
        {}

        Template::~Template()
        {}

        bool Template::static_argument_exists (std::string const &name) const
        {
                for (auto const &s : static_args_)
                     if (s.name() == name) return true;
                return false;
        }

        StaticArgument Template::static_argument (std::string const &name) const
        {
                for (auto const &s : static_args_)
                        if (s.name() == name) return s;
                throw std::runtime_error ("quatsch::extern_template::Template::"
                                          "static_argument(\"" + name + "\") "
                                          "called, but '" + name + "' does not "
                                          "exist.");
        }


        class Test : public Template {
        public:
                Test() : Template({StaticArgument("foo", StaticArgument::String),
                                   StaticArgument("bar", StaticArgument::Float) })
                {}
        };
} }

int main () {
        quatsch::extern_template::Test tpl;
        tpl.static_argument ("foo");
        using namespace quatsch::compiler;
        const std::string code =
        "\n"
        "(let pi:float 3.14159)\n"
        "(let two:float 3.14159)\n"
        "(let two:float 3.14159)\n"
        "(defun main:int (x:int y) (main 2i 3))\n" // TODO: shall we? would have re-nameable program input
        "(defun foo (x:int) (bar 0.5))\n"
        "(defun bar (x) (foo 1i))\n"
        "(defun bar_ (x) (foo_ 1i))\n"
        "(foo (main 2i 1337))"
        ;
        std::cout << "quatsch code:\n------------\n" << code
                  << "\n------------\n";

        std::cout << "-- phase 1 -----" << std::endl;
        phase1::Toque toks = phase1::tokenize (code.begin(), code.end());

        std::cout << "-- phase 2 -----" << std::endl;
        phase2::ProgramPtr P2 = phase2::parse(toks);
        if (!P2) {
                std::cerr << "parse error" << std::endl;
                return 0;
        }

        std::cout << "-- phase 3 -----" << std::endl;
        phase3::ProgramPtr P3 = phase3::resolve_and_verify (*P2);
        if (!P3) {
                std::cerr << "verficiation/resolution error" << std::endl;
                return 0;
        }
        std::string c99 = phase5::to_C99 (*P3);
        std::cout << "C99 code:\n------------\n" << c99
                  << "\n------------\n";
}


