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
        enum StaticType {
                String,
                Integer,
                Float
        };

        class StaticArgument {
        public:
                StaticArgument (std::string const &name,
                                StaticType type)
                        : name_(name), type_(type)
                {}

                std::string name() const { return name_; }
                StaticType  type() const { return type_; }

        private:
                std::string name_;
                StaticType type_;
        };

        class StaticParameter {
        public:
                static StaticParameter String  (std::string const &name, std::string const& val);
                static StaticParameter Integer (std::string const &name, int val);
                static StaticParameter Float   (std::string const &name, float val);

                std::string name() const;
                StaticType  type() const;

                std::string string  () const;
                int         integer () const;
                float       floating() const;
        private:
                StaticParameter (std::string const &name,  StaticType type);
        private:
                std::string name_;
                StaticType type_;
                std::string string_;
                int integer_;
                float float_;
        };

        StaticParameter::StaticParameter (std::string const &name,  StaticType type)
                : name_(name), type_(type)
        {
        }

        StaticParameter StaticParameter::String (std::string const &name,
                                                 std::string const &val)
        {
                StaticParameter ret {name, StaticType::String};
                ret.string_ = val;
                return ret;
        }
        StaticParameter StaticParameter::Integer (std::string const &name,
                                                  int val)
        {
                StaticParameter ret {name, StaticType::Integer};
                ret.integer_ = val;
                return ret;
        }
        StaticParameter StaticParameter::Float (std::string const &name,
                                                float val)
        {
                StaticParameter ret {name, StaticType::Float};
                ret.float_ = val;
                return ret;
        }
        std::string StaticParameter::name() const
        {
                return name_;
        }
        StaticType StaticParameter::type() const
        {
                return type_;
        }

        std::string StaticParameter::string() const
        {
                if (type_ != StaticType::String)
                        throw std::runtime_error ("StaticParameter::string() "
                                                  "called for non-string");
                return string_;
        }
        int StaticParameter::integer() const
        {
                if (type_ != StaticType::Integer)
                        throw std::runtime_error ("StaticParameter::integer() "
                                                  "called for non-integer");
                return integer_;
        }
        float StaticParameter::floating() const
        {
                if (type_ != StaticType::Float)
                        throw std::runtime_error ("StaticParameter::floating() "
                                                  "called for non-float");
                return float_;
        }

        class Template {
        public:
                virtual ~Template() ;

                bool static_argument_exists (std::string const &name) const;
                StaticArgument static_argument (std::string const &name) const;

                void incarnate (std::list<StaticParameter> parameters) const
                {
                        for (auto p : parameters) {
                                if (!static_argument_exists (p.name()))
                                        throw std::runtime_error( "parameter '"
                                         + p.name() + "' given to incarnate(), "
                                         "but it doesn't exist");
                                StaticArgument sa = static_argument(p.name());
                                if (sa.type() != p.type())
                                        throw std::runtime_error("parameter '" +
                                         p.name() + "' passed with incompatible"
                                         " type.");
                        }
                }

                template <typename ...Args>
                void incarnate (Args... args) const
                {
                        return incarnate (std::list<StaticParameter>{args...});
                }

        protected:
                Template (std::initializer_list<StaticArgument>) ;

                /* boils down to this with delegating constructors
                template <typename ...Args>
                Template (Args ...args) : Template ({args...}) {}
                */

        private:
                std::vector<StaticArgument> static_args_;
        };

        Template::Template (std::initializer_list<StaticArgument> args)
                : static_args_(args)
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
                Test() : Template({StaticArgument("foo", StaticType::String),
                                   StaticArgument("bar", StaticType::Float) })
                {}
        };
} }

int main () {
        using namespace quatsch::extern_template;
        Test tpl;
        tpl.incarnate (StaticParameter::String("foo", "meh!"),
                       StaticParameter::String("bar", "oops"));
        return 0;

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


