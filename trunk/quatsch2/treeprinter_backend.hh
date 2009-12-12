//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#ifndef TREEPRINTER_BACKEND_HH_INCLUDED_20091208
#define TREEPRINTER_BACKEND_HH_INCLUDED_20091208
namespace quatsch2 { namespace backend {
        class TreePrinter : public middleend::ExpressionVisitor {
                int indent_;
                std::string indent() {
                        std::string ret;
                        for (int i=0; i<indent_; ++i) {
                                ret += "    ";
                        }
                        return ret;
                }
        public:
                TreePrinter () : indent_(0) {}

                virtual void visit (middleend::ExpressionListExpr &e) {
                        using std::cout; using std::endl;
                        cout << indent() << e.expression_count() << ": {\n";
                        ++indent_;
                        for (size_t i = 0; i<e.expression_count(); ++i)
                                e[i]->accept (*this);
                        --indent_;
                        std::cout << indent() << "}\n";
                }
                virtual void visit (middleend::InvalidExpr &e) {
                }
                virtual void visit (middleend::AdditionExpr &e) {
                        std::cout << indent() << "+ {\n";
                        ++indent_;
                        for (size_t i = 0; i<e.parameter_count(); ++i)
                                e[i]->accept (*this);
                        --indent_;
                        std::cout << indent() << "}\n";
                }
                virtual void visit (middleend::SubtractionExpr &e) {
                        std::cout << indent() << "- {\n";
                        ++indent_;
                        for (size_t i = 0; i<e.parameter_count(); ++i)
                                e[i]->accept (*this);
                        --indent_;
                        std::cout << indent() << "}\n";
                }
                virtual void visit (middleend::MultiplicationExpr &e) {
                        std::cout << indent() << "* {\n";
                        ++indent_;
                        for (size_t i = 0; i<e.parameter_count(); ++i)
                                e[i]->accept (*this);
                        --indent_;
                        std::cout << indent() << "}\n";
                }
                virtual void visit (middleend::DivisionExpr &e) {
                        std::cout << indent() << "/ {\n";
                        ++indent_;
                        for (size_t i = 0; i<e.parameter_count(); ++i)
                                e[i]->accept (*this);
                        --indent_;
                        std::cout << indent() << "}\n";
                }
                virtual void visit (middleend::FloatLiteralExpr &e) {
                        std::cout << indent() << "<float>\n";
                }
                virtual void visit (middleend::IntLiteralExpr &e) {
                        std::cout << indent() << "<int>\n";
                }
                virtual void visit (middleend::IdExpr &e) {
                        std::cout << indent() << "<id:";
                        std::wcout << e.name() << ">\n";
                }
        };
} }
#endif // TREEPRINTER_BACKEND_HH_INCLUDED_20091208
