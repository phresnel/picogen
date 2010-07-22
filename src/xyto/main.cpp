#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <boost/optional.hpp>
#include <algorithm>

using boost::optional;

template <typename T> struct remove_pointer {
        typedef T type;
};
template <typename T> struct remove_pointer<T*> {
        typedef T type;
};

class CodeIterator {
        const char *curr;
        unsigned int row_, column_;
        void inc() {
                if (*curr == '\n') {
                        ++row_;
                        column_ = 0;
                } else {
                        ++column_;
                }
                ++curr;
        }
public:
        CodeIterator()
        : curr()
        , row_(0)
        , column_(0) {}

        CodeIterator(char const * val)
        : curr(val)
        , row_(0)
        , column_(0)
        {}

        CodeIterator(CodeIterator const &rhs)
        : curr(rhs.curr)
        , row_(rhs.row_)
        , column_(rhs.column_)
        {}

        bool operator!= (CodeIterator const &rhs) {
                return row_ != rhs.row_
                    || column_ != rhs.column_
                    ;
        }
        CodeIterator operator= (CodeIterator const &rhs) {
                curr = rhs.curr;
                row_ = rhs.row_;
                column_ = rhs.column_;
                return *this;
        }
        CodeIterator& operator ++ () {
                inc();
                return *this;
        }
        CodeIterator operator ++ (int) {
                CodeIterator ret = *this;
                inc();
                return ret;
        }
        char peek (unsigned int p) {
                CodeIterator ret = *this;
                while (p!=0) {
                        ++ret;
                        --p;
                }
                return *ret;
        }
        bool can_peek (unsigned int peek) {
                CodeIterator ret = *this;
                peek--; // <-- This is so that \0 can not be peeked
                while ((peek!=0) && (*ret!='\0')) {
                        ++ret;
                        --peek;
                }
                return peek == 0;
        }
        CodeIterator next() const {
                return ++CodeIterator(*this);
        }
        char operator *() const {
                return *curr;
        }

        unsigned int row() const { return row_; }
        unsigned int column() const { return column_; }
};

bool is_num(char c) {
        switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
                return true;
        };
        return false;
}

bool is_lower_alpha (char c) {
        switch (c) {
        case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
                return true;
        };
        return false;
}
bool is_upper_alpha (char c) {
        switch (c) {
        case 'A': case 'B': case 'C': case 'D':
        case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
                return true;
        };
        return false;
}
bool is_alpha(char c) {
        return is_lower_alpha(c) || is_upper_alpha(c);
}
bool is_alnum(char c) {
        return is_alpha(c) || is_num(c);
}
bool is_white (char c) {
        switch (c) {
        case ' ': case '\n': case '\t': case '\r':
                return true;
        };
        return false;
}




class Token {
public:
        enum Type {
                LessThan, GreaterThan, // <>
                TransformTo, // -->
                Colon, Semicolon,
                Identifier
        };

        Token (Type type_,
               const CodeIterator &from,
               const CodeIterator &to
        )
        : type_(type_)
        , from_(from)
        , to_(to)
        , value_("")
        {
                for (CodeIterator curr=from_; curr!=to_; ++curr) {
                        value_ += *curr;
                }
        }

        Token (Token const &rhs)
        : type_(rhs.type_)
        , from_(rhs.from_)
        , to_(rhs.to_ )
        , value_(rhs.value_)
        {}

        Token& operator= (Token const &rhs) {
                type_ = rhs.type_;
                from_ = rhs.from_;
                to_ = rhs.to_;
                value_ = rhs.value_;
                return *this;
        }

        Type type() const { return type_; }
        std::string value() const { return value_; }
        CodeIterator from() const { return from_; }
        CodeIterator to() const { return to_; }
private:
        Token();
        Type type_;
        CodeIterator from_, to_;
        std::string value_;
};


typedef std::vector<Token> TokenVector;
typedef TokenVector::const_iterator TokenIterator;

TokenVector tokenize(const char *code) {
        TokenVector tokens;
        typedef CodeIterator CI;
        for (CI it=code; *it!='\0'; ++it) {
                const char c = *it;
                if (is_alpha(c)) {
                        const CI begin = it;
                        CI prev = it;
                        for (; *it!='\0' && is_alnum(*it); prev=it, ++it) {
                        }
                        tokens.push_back (Token(Token::Identifier, begin, it));
                        it = prev;
                } else if (c == '<') {
                        tokens.push_back (Token(Token::LessThan, it, it.next()));
                } else if (c == '>') {
                        tokens.push_back (Token(Token::GreaterThan, it, it.next()));
                } else if (c == '-'
                        && it.can_peek(1) && it.peek(1)=='-'
                        && it.can_peek(2) && it.peek(2)=='>'
                ) {
                        const CI begin = it;
                        ++it; ++it;
                        tokens.push_back (Token(Token::TransformTo, begin, it.next()));
                } else if (c == ':') {
                        tokens.push_back (Token(Token::Colon, it, it.next()));
                } else if (c == ';') {
                        tokens.push_back (Token(Token::Semicolon, it, it.next()));
                } else if (!is_white(c)) {
                        std::cerr << "tokenization error in "
                             << "line " << it.row() << ", column " << it.column()
                             << ": '" << *it << "'" << std::endl;
                        return TokenVector();
                }
        }
        return tokens;
}


class Symbol {
public:
        std::string name() const {
                return name_;
        }
        void setName(std::string const &name) {
                name_ = name;
        }
private:
        std::string name_;
};
inline bool operator == (Symbol const &lhs, Symbol const &rhs) {
        return lhs.name() == rhs.name();
}
inline bool operator != (Symbol const &lhs, Symbol const &rhs) {
        return lhs.name() != rhs.name();
}
inline std::ostream& operator<< (std::ostream& o, Symbol const& rhs) {
        o << rhs.name();
        return o;
}


class Pattern {
public:
        Symbol const& operator[] (unsigned int i) const {
                return symbols[i];
        }
        Symbol & operator[] (unsigned int i) {
                return symbols[i];
        }
        unsigned int size() const {
                return symbols.size();
        }
        void push_back(Symbol const &sym) {
                symbols.push_back(sym);
        }
        bool empty() const {
                return size() == 0;
        }
private:
        std::vector<Symbol> symbols;
};
inline bool operator == (Pattern const &lhs, Pattern const &rhs) {
        if (lhs.size() != rhs.size())
                return false;
        for (unsigned int i=0; i<lhs.size(); ++i) {
                if (lhs[i] != rhs[i])
                        return false;
        }
        return true;
}
inline std::ostream& operator<< (std::ostream& o, Pattern const& rhs) {
        if (rhs.size()) {
                o << rhs[0];
        }
        for (unsigned int i=1; i<rhs.size(); ++i) {
                o << " " << rhs[i];
        }
        return o;
}


class ProductionHeader {
public:
        std::string name() const {
                return name_;
        }
        void setName(std::string const &name) {
                name_ = name;
        }

        Pattern leftContext() const {
                return leftContext_;
        }
        void setLeftContext(Pattern const & p) {
                leftContext_ = p;
        }

        Pattern rightContext() const {
                return rightContext_;
        }
        void setRightContext(Pattern const & p) {
                rightContext_ = p;
        }

        Pattern pattern() const {
                return pattern_;
        }
        void setPattern(Pattern const & p) {
                pattern_ = p;
        }

private:
        std::string name_;
        Pattern leftContext_, rightContext_;
        Pattern pattern_;
};
inline std::ostream& operator<< (std::ostream& o, ProductionHeader const& rhs) {
        o << rhs.name() << ": ";

        if (!rhs.leftContext().empty())
                o << rhs.leftContext() << " < ";

        o << rhs.pattern();

        if (!rhs.rightContext().empty())
                o << " > " << rhs.rightContext();
        return o;
}

class ProductionBody {
public:
        Pattern pattern() const {
                return pattern_;
        }
        void setPattern(Pattern const & p) {
                pattern_ = p;
        }
private:
        Pattern pattern_;
};
inline std::ostream& operator<< (std::ostream& o, ProductionBody const& rhs) {
        o << rhs.pattern();
        return o;
}

class Production {
public:
        ProductionHeader header() const {
                return header_;
        }
        void setHeader(ProductionHeader const &h) {
                header_ = h;
        }

        ProductionBody body() const {
                return body_;
        }
        void setBody(ProductionBody const &h) {
                body_ = h;
        }

private:
        ProductionHeader header_;
        ProductionBody body_;
};
inline std::ostream& operator<< (std::ostream& o, Production const& rhs) {
        o << rhs.header() << " --> " << rhs.body() << ";";
        return o;
}
inline bool hasPrecedenceOver (Production const &lhs, Production const &rhs) {
        const int numContextsLhs =
                !lhs.header().leftContext().empty()
                + !lhs.header().rightContext().empty();
        const int numContextsRhs =
                !rhs.header().leftContext().empty()
                + !rhs.header().rightContext().empty();

        // Always prefer contexts over no context, as well as
        // two contexts over one context.
        if (numContextsLhs > numContextsRhs)
                return true;
        else if (numContextsLhs < numContextsRhs)
                return false;

        // Always prefer larger main-pattern.
        const int patternSizeLhs = lhs.header().pattern().size();
        const int patternSizeRhs = rhs.header().pattern().size();
        if (patternSizeLhs > patternSizeRhs)
                return true;
        else if (patternSizeLhs < patternSizeRhs)
                return false;

        // Prefer larger context.
        const int contextSizeLhs = lhs.header().leftContext().size()
                                 + lhs.header().rightContext().size();
        const int contextSizeRhs = rhs.header().leftContext().size()
                                 + rhs.header().rightContext().size();
        if (contextSizeLhs > contextSizeRhs)
                return true;
        else if (contextSizeLhs < contextSizeRhs)
                return false;

        return false;
}
inline bool ambiguous (Production const &lhs, Production const &rhs) {
        return lhs.header().pattern() == rhs.header().pattern()
            && !hasPrecedenceOver(lhs, rhs)
            && !hasPrecedenceOver(rhs, lhs)
            ;
}





optional<Symbol> parse_symbol (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        if (it == end || it->type() != Token::Identifier)
                return optional<Symbol>();
        Symbol sym;
        sym.setName (it->value());
        behind = ++it;
        return sym;
}

Pattern parse_pattern (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        Pattern ret;
        while (optional<Symbol> sym = parse_symbol(it, end, behind)) {
                it = behind;
                ret.push_back(*sym);
        }
        behind = it;
        return ret;
}



bool parse_match_patterns (
        TokenIterator it, TokenIterator end,
        TokenIterator &behind,
        Pattern &leftContext, Pattern &mainPattern, Pattern &rightContext
) {
        const TokenIterator beforePatterns = it;
        TokenIterator behindPattern;
        const Pattern pat = parse_pattern(it, end, behindPattern);
        TokenIterator prev = it;
        it = behindPattern;

        if (it != end && it->type() == Token::LessThan) {
                leftContext = pat;
                ++it;
                mainPattern = parse_pattern(it, end, behindPattern);
                prev = it; it = behindPattern;

                if (it != end && it->type() == Token::GreaterThan) {
                        ++it;
                        rightContext = parse_pattern(it, end, behindPattern);
                        prev = it; it = behindPattern;
                }

        } else if (it != end && it->type() == Token::GreaterThan) {
                mainPattern = pat;
                ++it;
                rightContext = parse_pattern(it, end, behindPattern);
                prev = it; it = behindPattern;
        } else if (!pat.empty()) {
                mainPattern = pat;
        }

        if (mainPattern.empty()) {
                TokenIterator i = beforePatterns - 1;
                std::cerr <<
                   "error: no pattern found in line "
                   << i->from().next().row() << ", column "
                   << i->from().next().column() << std::endl;
                return optional<Production>();
        }
        behind = it;

        return true;
}

optional<ProductionHeader> parse_production_header (
        TokenIterator it, TokenIterator end, TokenIterator &behind
) {
        TokenIterator prev = it;

        if (Token::Identifier != it->type()) {
                std::cerr << "error: expected identifier at line "
                        << prev->from().row() << ", column "
                        << prev->from().column() << std::endl;
                return optional<ProductionHeader>();
        }
        const std::string name = it->value();
        prev = it;
        ++it;
        // Checkpoint: We have an identifier.

        if (it==end || it->type() != Token::Colon) {
                std::cerr << "error: expected ':' at line "
                   << prev->to().next().row() << ", column "
                   << prev->to().next().column() << std::endl;
                return optional<ProductionHeader>();
        }
        prev = it;
        ++it;
        // Checkpoint: The identifier is followed by a colon.

        TokenIterator behindMatchPatterns;
        Pattern leftContext, mainPattern, rightContext;

        if (!parse_match_patterns(it, end, behindMatchPatterns,
                                  leftContext, mainPattern, rightContext)
        ) {
                return optional<ProductionHeader>();
        }
        it = behindMatchPatterns;

        if (it == end || it->type() != Token::TransformTo) {
                TokenIterator i = it - 1;
                std::cerr <<
                   "error: expected body (starting with '-->') for "
                   " production in line "
                   << i->from().next().row() << ", column "
                   << i->from().next().column() << ", but found "
                   << "'" << i->value() << "'" << std::endl;
                return optional<ProductionHeader>();
        }
        // Checkpoint: We just passed "-->"

        ++it;
        behind = it;

        ProductionHeader ret;
        ret.setName (name);
        ret.setLeftContext(leftContext);
        ret.setRightContext(rightContext);
        ret.setPattern(mainPattern);
        return ret;
}

optional<ProductionBody> parse_production_body (
        // foo --> bar  x:
        TokenIterator it, TokenIterator end, TokenIterator &behind
) {
        TokenIterator behindPattern;
        const Pattern pat = parse_pattern(it, end, behindPattern);
        it = behindPattern;
        if (it == end || it->type() != Token::Semicolon) {
                TokenIterator prev = it - 1;
                std::cerr << "error: expected ';' after production in line "
                        << prev->to().row() << ", column "
                        << prev->to().column() << std::endl;
                return optional<ProductionBody>();
        }
        ++it;
        behind = it;

        ProductionBody ret;
        ret.setPattern (pat);
        return ret;
}

optional<Production> parse_production (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        if (it==end) {
                std::cerr << "found nothing" << std::endl;
                return optional<Production>();
        }


        optional<ProductionHeader> header;
        optional<ProductionBody> body;

        // Header
        {
                TokenIterator behindHeader;
                header = parse_production_header(it, end, behindHeader);
                if (!header) {
                        return optional<Production>();
                }
                it = behindHeader;

                if (it == end) {
                        TokenIterator i = it - 1;
                        std::cerr <<
                           "error: expected body behind '-->' in line "
                           << i->to().next().row() << ", column "
                           << i->to().next().column() << std::endl;
                        return optional<Production>();
                }
        }
        // Body
        {
                TokenIterator behindBody;
                body = parse_production_body(it, end, behindBody);
                if (!body) {
                        return optional<Production>();
                }
                it = behindBody;
        }


        behind = it;

        Production ret;
        ret.setHeader (*header);
        ret.setBody (*body);
        return ret;
}

void compile (const char *code) {
        const TokenVector tokens = tokenize (code);

        std::vector<Production> prods;

        std::set<std::string> names;
        TokenIterator it = tokens.begin();
        const TokenIterator &end = tokens.end();
        //char c = 'a';
        while (it != end) {
                TokenIterator behind;
                if (optional<Production> op = parse_production (it, end, behind)) {
                        if (names.find(op->header().name())
                          != names.end()
                        ) {
                                std::cerr << "error: multiple productions are "
                                 << "named '" << op->header().name() << "'.\n";
                                return;
                        }
                        names.insert(op->header().name());
                        prods.push_back(*op);
                } else {
                        return;
                }
                it = behind;
        }

        std::stable_sort (prods.begin(), prods.end(), hasPrecedenceOver);
        /*for (int i=0; i<prods.size(); ++i) {
                std::cout << i << " -- " << prods[i] << '\n';
        }*/

        // check if there are ambiguous rules
        for (unsigned int i=0; i<prods.size()-1; ++i) {
                if (ambiguous (prods[i], prods[i+1])) {
                        std::cerr << "warning: productions '"
                          << prods[i].header().name()
                          << "' and '"
                          << prods[i+1].header().name()
                          << "' might be ambiguous (neither is more "
                          << "specialized); in such case, '"
                          << prods[i].header().name()
                          << "' will be preferred (because it was "
                          << "declared first).\n";
                        ;
                }
        }


        const TokenVector axiom = tokenize("a b a");
        std::cout << "--------------\n";
        TokenIterator behind;
        Pattern pat = parse_pattern(axiom.begin(), axiom.end(), behind);
        std::cout << "axiom: " << pat << '\n';

        for (int step=0; step<6; ++step) {
                optional<Pattern> apply(Production const &, Pattern const &);

                bool any = false;
                optional<Pattern> next;
                for (unsigned int i=0; i<prods.size(); ++i) {
                        next = apply (prods[i], pat);
                        if (next) {
                                any = true;
                                break;
                        }
                }

                if (any) {
                        pat = *next;
                        std::cout << "step " << step << ": " << pat << '\n';
                } else {
                        std::cout << "no match in step " << step << '\n';
                        break;
                }
        }
}


unsigned int matchLength (Pattern const &pattern,
                 Pattern const &axiom,
                 int axiomIndex
) {
        if (axiomIndex<0)
                return 0;
        for (unsigned int i=0; i<pattern.size(); ++i) {
                if (i+axiomIndex >= axiom.size()) return 0;
                if (pattern[i] != axiom[i+axiomIndex])
                        return 0;
        }
        return pattern.size();
}

unsigned int matchLength (Production const &production,
                 Pattern const &axiom,
                 int axiomIndex
) {
        const ProductionHeader &header = production.header();
        const int mainLen = matchLength (header.pattern(), axiom, axiomIndex);
        if (0 == mainLen)
                return 0;

        if (!header.leftContext().empty()) {
                Pattern const & ct = header.leftContext();
                if (!matchLength (ct, axiom, axiomIndex-ct.size()))
                        return 0;
        }
        if (!header.rightContext().empty()) {
                Pattern const & ct = header.rightContext();
                if (!matchLength (ct, axiom, axiomIndex+mainLen))
                        return 0;
        }
        return mainLen;
}

optional<Pattern> apply(Production const &production, Pattern const &axiom) {
        Pattern ret;

        bool any = false;

        unsigned int i=0;
        while (i<axiom.size()) {
                if (int len=matchLength(production, axiom, i)) {
                        any = true;
                        const ProductionBody body = production.body();
                        for (unsigned int b=0; b<body.pattern().size(); ++b)
                                ret.push_back(body.pattern()[b]);
                        i += len;
                } else {
                        ret.push_back(axiom[i]);
                        ++i;
                }
        }

        if (!any)
                return optional<Pattern>();
        return ret;
}


int main()
{
        // f(x) < y(x)   should yield an error "parameter names may only appear once"
        const char * code =
                "first:     b     --> First;\n"
                "sec:       b > a --> Second;"
                "third: a < b     --> Third;\n"
        ;
        compile(code);

        return 0;
}
