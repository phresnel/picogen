//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2009  Sebastian Mach
//
//      email: a@b.c, with a=phresnel, b=gmail, c=com
//        www: http://phresnel.org
//             http://picogen.org
//
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the  Free  Software  Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed  in the hope that it will be useful, but
//    WITHOUT  ANY  WARRANTY;   without   even  the  implied  warranty  of
//    MERCHANTABILITY  or  FITNESS FOR A PARTICULAR  PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy  of  the  GNU General Public License
//    along with this program. If not, see <http://www.gnu.org/licenses/>.


#ifndef SYMBOLTABLE_H__INCLUDED__20090104
#define SYMBOLTABLE_H__INCLUDED__20090104

#include <map>
#include <string>

namespace quatsch {

    template <typename T> class SymbolTable {
            ::std::map < ::std::string, T> table;    
        public:
            
            class already_defined_exception {
                    already_defined_exception ();
                public:
                    const ::std::string symbol;
                    already_defined_exception (const ::std::string &symbol) : symbol (symbol) {}
            };
            
            class undefined_symbol_exception {
                    undefined_symbol_exception ();
                public:
                    const ::std::string symbol;
                    undefined_symbol_exception (const ::std::string &symbol) : symbol (symbol) {}
            };
            
            SymbolTable () : table () {
            }
            
            virtual ~SymbolTable () {
                table.clear();
            }
            
            typedef typename ::std::map < ::std::string, T>::const_iterator const_iterator;
            const_iterator begin () const { return table.begin(); }
            const_iterator end () const { return table.end(); }
            
            void addSymbol (const ::std::string &symname, const T &t) {
                if (table.end() != table.find (symname))
                    throw already_defined_exception (symname);
                //table [symname] = t; // Doesnn't work without default constructor T::T() ...
                table.insert (::std::pair < ::std::string, T> (symname, t)); // ... but this does.
            }

            bool hasSymbol (const ::std::string &symname) const {
                return table.end() != table.find (symname);
            }

            void probeSymbol (const ::std::string &symname) const {
                if (!hasSymbol (symname))
                    throw undefined_symbol_exception (symname);
            }

            T &getItem (const ::std::string &symname) {
                // Don't use map::operator [], as it would require the existence of T::T().
                probeSymbol (symname);
                typename ::std::map < ::std::string, T>::iterator it = table.find (symname);
                return it->second;
            }
            
            T &operator [] (const ::std::string &symname) {
                // Don't use map::operator [], as it would require the existence of T::T().
                probeSymbol (symname);                
                typename ::std::map < ::std::string, T>::iterator it = table.find (symname);
                return it->second;
            }

            const T &getItem (const ::std::string &symname) const {
                // Don't use map::operator [], as it would require the existence of T::T().
                probeSymbol (symname);
                typename ::std::map < ::std::string, T>::const_iterator it = table.find (symname);
                return it->second;
            }
            
            const T &operator [] (const ::std::string &symname) const {
                // Don't use map::operator [], as it would require the existence of T::T().
                probeSymbol (symname);
                typename ::std::map < ::std::string, T>::const_iterator it = table.find (symname);
                return it->second;
            }

            size_t size () const {
                return table.size ();
            }
    };
}
#endif // SYMBOLTABLE_H__INCLUDED__20090104
