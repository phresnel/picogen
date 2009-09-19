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

#ifndef PARSING_HH_INCLUDED_20090829
#define PARSING_HH_INCLUDED_20090829

#include <queue>
#include <string>
#include <map>
#include <stack>
#include <utility>
#include <list>
// TODO sort+prune includes
// TODO make sure that id's are valid (tho we must define what is valid first)
// TODO kill unused function

namespace actuarius { namespace detail {
        
        // TODO is incomplete
        template <typename iterator_t>
        std::string
        unescape_nonstring_terminal (iterator_t begin, iterator_t end) {
                std::string ret;
                iterator_t it = begin;
                while (it != end) {
                        if (*it == '\\' && (it+1) != end) {
                                if(*(it+1) == ';') {
                                        ++it;
                                } else if(*(it+1) == '\\') {
                                        ++it;
                                } else {
                                        throw std::invalid_argument (
                                        "invalid escape sequence found in '" +
                                        std::string (begin, end) + "' at '" +                                        
                                        std::string (it, end) + "'");
                                }
                        }
                        ret += *it;
                        ++it;
                }
                return ret;
        }
        
        template <typename T>
        std::string
        unescape_nonstring_terminal (T const & val) {
                return unescape_nonstring_terminal (val.begin(), val.end());
        }
        
        
        
        
        template <typename iterator_t>
        bool is_whitespace (iterator_t it) {
                switch (*it) {
                case ' ':
                case '\t':
                case '\r':
                case '\n': return true;
                default: return false;
                };
        }
        
        template <typename iterator_t>
        bool is_num (iterator_t it) {
                switch (*it) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': return true;
                default: return false;
                };
        }
        
        template <typename iterator_t>
        bool is_alpha (iterator_t it) {
                switch (*it) {
                case 'a': case 'A':
                case 'b': case 'B':
                case 'c': case 'C':
                case 'd': case 'D':
                case 'e': case 'E':
                case 'f': case 'F':
                case 'g': case 'G':
                case 'h': case 'H':
                case 'i': case 'I':
                case 'j': case 'J':
                case 'k': case 'K':
                case 'l': case 'L':
                case 'm': case 'M':
                case 'n': case 'N':
                case 'o': case 'O':
                case 'p': case 'P':
                case 'q': case 'Q':
                case 'r': case 'R':
                case 's': case 'S':
                case 't': case 'T':
                case 'u': case 'U':
                case 'v': case 'V':
                case 'w': case 'W':
                case 'x': case 'X':
                case 'y': case 'Y':
                case 'z': case 'Z': return true;
                default: return false;
                };
        }
        
        template <typename iterator_t>
        bool is_alphanum (iterator_t it) {
                return is_alpha (it) || is_num (it);
        }
        
        template <typename iterator_t>
        bool is_idchar (iterator_t it) {
                return is_alphanum (it) || '_' == *it;
        }
        
        template <typename iterator_t>
        bool is_open_brace (iterator_t it) {
                return *it == '{';
        }
        
        template <typename iterator_t>
        bool is_close_brace (iterator_t it) {
                return *it == '}';
        }
        
        template <typename iterator_t>
        bool is_colon (iterator_t it) {
                return *it == ':';
        }
        
        template <typename iterator_t>
        bool is_semicolon (iterator_t it) {
                return *it == ';';
        }
        
        template <typename iterator_t>
        bool is_open_block (iterator_t it) {
                switch (*it) {
                case '{':
                return true;
                default: return false;
                };
        }
        
        template <typename iterator_t>
        bool is_open_terminal (iterator_t it) {
                switch (*it) {
                case ':':
                return true;
                default: return false;
                };
        }
        
        template <typename iterator_t>
        bool is_valid_for_escape (iterator_t it) {
                switch (*it) {
                case ':':
                case ';':
                case '{':
                case '}':
                        return true;
                default:
                        return false;
                };
        }
        
        template <typename iterator_t>
        std::string parse_id (iterator_t &it, iterator_t end) {
                std::string ret;
                while (it != end && is_idchar (it)) {
                        ret += *it;
                        ++it;
                }
                return ret;
        }
        
        template <typename iterator_t>
        void eat_whitespace (iterator_t &it, iterator_t end) {
                while (it != end && is_whitespace (it)) {                        
                        ++it;
                }
        }        
} }



namespace actuarius { namespace detail {
        
template <typename iterator_t>
match_t<iterator_t>
identifier (iterator_t it, iterator_t end) {
        
        if (is_idchar (it)) {
                const iterator_t id_begin = it;
                const std::string name = parse_id (it, end);
                const iterator_t id_end = it;
                return match_t<iterator_t>(id_begin, id_end);
        }

        return match_t<iterator_t>();
}



template <typename iterator_t>
block_match_t<iterator_t>
block (iterator_t it, iterator_t end) {
        const iterator_t block_begin = it;
        
        // Parse Id.
        const match_t<iterator_t> id_match = identifier (it, end);
        if (!id_match)
                return block_match_t<iterator_t>();
        
        
        // Forward and eat whitespace.
        it = id_match.end();
        eat_whitespace (it, end);
        

        // Id must be followed by open-brace.
        if (!is_open_brace (it))
                return block_match_t<iterator_t>();
        ++it; // after brace

        
        // Parse whole block.
        const iterator_t content_begin = it;
        int balance = 1;
        while (0 != balance) {
                if (it == end)
                        throw std::invalid_argument(
                        "reached end-of-file thanks to unbalanced braces"
                        );
                if (is_open_brace (it))
                        ++balance;
                else if (is_close_brace (it))
                        --balance;
                else if ('\\' == *it) {
                        const iterator_t peek = it+1;
                        if (peek == end)
                                throw std::invalid_argument (
                                "escape character ('\\') at end of file"
                                );
                        if (is_valid_for_escape (peek))
                                ++it;
                        else
                                throw std::invalid_argument (
                                "invalid escape sequence found in '" +
                                std::string (block_begin, end) + "' at '" +
                                std::string (it, end) + "'");                                
                }
                ++it;
        }
        const iterator_t content_end = it-1; // before brace
        
        
        // Compose block_match.
        return block_match_t<iterator_t> (
                id_match,
                match_t<iterator_t> (content_begin, content_end)
        );
}




template <typename iterator_t>
value_match_t<iterator_t>
value (iterator_t it, iterator_t end) {
        const iterator_t begin = it;
        
        // Parse Id.
        const match_t<iterator_t> id_match = identifier (it, end);
        if (!id_match)
                return value_match_t<iterator_t>();
        
        
        // Forward and eat whitespace.
        it = id_match.end();
        eat_whitespace (it, end);
        

        // Id must be followed by colon.
        if (!is_colon (it))
                return value_match_t<iterator_t>();
        ++it; // after colon

        
        // Parse whole value.
        const iterator_t value_begin = it;        
        while (!is_semicolon (it)) {
                if (it == end)
                        throw std::invalid_argument(
                        "reached end-of-file before reaching semicolon at '" +
                        std::string (value_begin, end) + 
                        "'"
                        );
                if ('\\' == *it) {
                        const iterator_t peek = it+1;
                        if (peek == end)
                                throw std::invalid_argument (
                                "escape character ('\\') at end of file"
                                );
                        if (is_valid_for_escape (peek))
                                ++it;
                        else
                                throw std::invalid_argument (
                                "invalid escape sequence found in '" +
                                std::string (begin, end) + "' at '" +
                                std::string (it, end) + "'");                                
                }
                ++it;
        }

        const iterator_t value_end = it;
        
        
        // Compose block_match.
        return value_match_t<iterator_t> (
                id_match,
                match_t<iterator_t> (value_begin, value_end)
        );
}



template <typename iterator_t>
//block_t parse (iterator_t it, iterator_t end) {
block_t<iterator_t>
 parse (block_match_t<iterator_t> const & parent_block) {
        const iterator_t parse_begin = parent_block.content().begin();
        iterator_t       it  = parent_block.content().begin();
        const iterator_t end = parent_block.content().end();
        
        block_t<iterator_t> ret (parent_block);                
        
        eat_whitespace (it, end);
        while (it != end) {
                
                bool matchAny = false;                

                if (!matchAny) {
                        const block_match_t<iterator_t> block_match = block (it, end);
                        if (block_match) {
                                matchAny = true;
                                
                                //std::cout << "[[[" << std::string (block_match.id().begin(), block_match.id().end()) << "]]]\n";
                                //std::cout << "<<<" << std::string (block_match.content.begin, block_match.content.end) << ">>>\n";
                                
                                it = block_match.behind_content();
                                
                                const block_t<iterator_t> block = parse (block_match);
                                if (block)
                                        ret.add_child (block);
                        }
                }
                
                if (!matchAny) {
                        const value_match_t<iterator_t> value_match = value (it, end);
                        if (value_match) {
                                ret.add_value (value_match);
                                matchAny = true;
                                
                                //std::cout << "(((" << std::string (value_match.id().begin(), value_match.id().end()) << ")))\n";
                                //std::cout << "===" << std::string (value_match.value().begin(), value_match.value().end()) << "===\n";
                                
                                it = value_match.behind_value();
                        }
                }
                
                if (!matchAny) {
                        throw std::invalid_argument (
                                "unexpected character found in '" +
                                std::string (parse_begin, end) + "' at '" +
                                std::string (it, end) + "'");
                }
                
                eat_whitespace (it, end);
        }
        
        return ret;
}
} }

#endif // PARSING_HH_INCLUDED_20090829
