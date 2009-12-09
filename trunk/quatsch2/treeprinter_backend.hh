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
