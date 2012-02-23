#include <iostream>
#include <deque>
#include <boost/optional.hpp>

#include "phase1/tokenize.h"
#include "phase2/parse.h"
#include "phase3/resolve_and_verify.h"
#include "phase5/C99/to_C99.h"

int main () {
        using namespace quatsch::compiler;
        const std::string code =
        "\n"
        "(let pi:float 3.14159)\n"
        "(let two:float 3.14159)\n"
        "(let two2:float 3.14159)\n"
        "(defun main:float (x:int y) (main 2i 3))\n" // TODO: shall we? would have re-nameable program input
        "(defun foo (x:int) (bar 0.5))\n"
        "(defun bar (x) (foo 1i))\n"
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


