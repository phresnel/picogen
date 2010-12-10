//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef META_HH_INCLUDED_20101118
#define META_HH_INCLUDED_20101118

#include <list>
#include <vector>
#include <string>
#include <limits>
#include "optional.hh"

namespace quatsch_preprocessor {
class DomainScalar {
public:
        DomainScalar();
        DomainScalar (double val);

        void setValue (double val);
        double value () const;

        bool asBool() const;

        std::string displayValue() const;

        static DomainScalar max() {
                return std::numeric_limits<double>::max();
        }
private:
        double value_;
};
bool operator < (DomainScalar const &lhs, DomainScalar const &rhs);
bool operator > (DomainScalar const &lhs, DomainScalar const &rhs);
bool operator <= (DomainScalar const &lhs, DomainScalar const &rhs);
bool operator >= (DomainScalar const &lhs, DomainScalar const &rhs);
DomainScalar operator - (DomainScalar const &rhs);
DomainScalar operator - (DomainScalar const &lhs, DomainScalar const &rhs);



class DomainInterval {
public:
        DomainInterval();
        DomainInterval (DomainScalar from, DomainScalar to);

        DomainScalar from() const;
        DomainScalar to  () const;

        void setFrom (DomainScalar val);
        void setTo   (DomainScalar val);

        DomainScalar min() const;
        DomainScalar max() const;
        bool isLinear() const;
        bool isFinite() const;
        unsigned int elementCount() const;
        std::list<DomainScalar> elements() const;
private:
        DomainScalar from_, to_;
};



class DomainEnumeration {
public:
        typedef std::string value_type;

        DomainEnumeration();

        std::vector<value_type> elements() const;

        // Container.
        typedef std::vector<value_type>::iterator        iterator;
        typedef std::vector<value_type>::const_iterator  const_iterator;
        typedef std::vector<value_type>::size_type       size_type;

        void push_back (value_type const &d);

        iterator       begin();
        const_iterator begin() const;

        iterator       end();
        const_iterator end() const;

        size_type      size () const;

        value_type       &at         (size_type i);
        const value_type &at         (size_type i) const;
        value_type       &operator[] (size_type i);
        const value_type &operator[] (size_type i) const;
private:
        std::vector<std::string> elements_;
};



/*class DomainFilename {
public:
        DomainFilename();
        DomainFilename(std::string const &);

        void setValue (std::string const &);
        std::string value() const;
private:
        std::string value_;
};*/



enum DomainType {
        Scalar,
        Interval,
        Enumeration
};



class DomainValue {
public:
        DomainValue (DomainScalar scalar);
        DomainValue (DomainInterval inter);
        DomainValue (DomainEnumeration Enum);

        DomainType type() const;

        void toScalar (DomainScalar scalar);
        void toInterval (DomainInterval interval);
        void toEnumeration(DomainEnumeration enumeration);

        DomainScalar scalar() const;
        DomainInterval interval() const;
        DomainEnumeration enumeration() const;

        DomainScalar min() const;
        DomainScalar max() const;
        bool isLinear() const;
        bool isFinite() const;
        unsigned int elementCount() const;
        std::list<DomainScalar> elements() const;

private:
        void reset();
        DomainType type_;
        optional<DomainScalar> scalar_;
        optional<DomainInterval> interval_;
        optional<DomainEnumeration> enumeration_;

        DomainValue();
};



class Domain {
public:
        // Query.
        DomainScalar min() const;
        DomainScalar max() const;

        bool isLinear () const;
        bool isFinite () const;
        bool isEnumeration () const;
        bool empty() const;
        unsigned int elementCount() const;
        std::list<DomainScalar> elements() const;
        DomainEnumeration asEnumeration() const;

        // Container.
        typedef std::vector<DomainValue>::iterator        iterator;
        typedef std::vector<DomainValue>::const_iterator  const_iterator;
        typedef std::vector<DomainValue>::size_type       size_type;

        void push_back (DomainValue const &d);

        iterator       begin();
        const_iterator begin() const;

        iterator       end();
        const_iterator end() const;

        size_type      size () const;

        DomainValue       &at         (size_type i);
        const DomainValue &at         (size_type i) const;
        DomainValue       &operator[] (size_type i);
        const DomainValue &operator[] (size_type i) const;
private:
        std::vector<DomainValue> values_;
};



enum DeclaredType {
        Integer,
        Real,
        Boolean,
        Filename,
        EnumerationValue
};
bool domainMustBeImplicit(DeclaredType t);



class Declaration {
public:
        Declaration ();

        std::string id() const;
        void setId(std::string const & id);

        std::string displayName() const;

        DeclaredType type() const;
        void setType(DeclaredType const &t);

        Domain domain() const;
        void setDomain(Domain const &d);

        DomainScalar domainMin() const;
        DomainScalar domainMax() const;
        bool hasFiniteDomain() const;
        bool hasImplicitDomain() const;
        unsigned int domainElementCount() const;
        std::list<DomainScalar> domainElements() const;

        bool hasDefaultValue() const;
        DomainValue defaultValue() const;
        void setDefaultValue (DomainValue const &);
private:
        std::string id_;
        DeclaredType type_;
        Domain domain_;
        optional<DomainValue> defaultValue_;
};



class Declarations {
public:
        // Queries.
        //DomainScalar domainMin() const;
        //DomainScalar domainMax() const;

        // Container.
        typedef std::vector<Declaration>::iterator        iterator;
        typedef std::vector<Declaration>::const_iterator  const_iterator;
        typedef std::vector<Declaration>::size_type       size_type;

        void push_back (Declaration const &d);

        iterator       begin();
        const_iterator begin() const;

        iterator       end();
        const_iterator end() const;

        size_type      size () const;

        Declaration       &at         (size_t i);
        const Declaration &at         (size_t i) const;
        Declaration       &operator[] (size_t i);
        const Declaration &operator[] (size_t i) const;
private:
        std::vector<Declaration> declarations_;
};

}
#endif // META_HH_INCLUDED_20101118
