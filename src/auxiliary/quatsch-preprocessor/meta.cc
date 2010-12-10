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

#include <vector>
#include <stdexcept>
#include <sstream>
#include "meta.hh"

#include <QDebug>

namespace quatsch_preprocessor {
        bool domainMustBeImplicit(DeclaredType t) {
                switch (t) {
                case Integer:
                case Real:
                case EnumerationValue:
                        return false;
                case Boolean:
                case Filename:
                        return true;
                }
                throw std::runtime_error ("domainMustBeImplicit(): enum value not "
                                          "handled.");
        }


        DomainScalar::DomainScalar() {}
        DomainScalar::DomainScalar (double val) : value_ (val) {}

        void DomainScalar::setValue (double val) { value_ = val; }
        double DomainScalar::value () const { return value_; }
        bool DomainScalar::asBool() const { return value_ > 0.5; }

        std::string DomainScalar::displayValue() const {
                std::stringstream ss;
                ss << std::fixed << value_;
                return ss.str();
        }

        bool operator < (DomainScalar const &lhs,
                                 DomainScalar const &rhs)
        {
                return lhs.value() < rhs.value();
        }
        bool operator > (DomainScalar const &lhs,
                                 DomainScalar const &rhs)
        {
                return lhs.value() > rhs.value();
        }
        bool operator <= (DomainScalar const &lhs,
                                 DomainScalar const &rhs)
        {
                return lhs.value() <= rhs.value();
        }
        bool operator >= (DomainScalar const &lhs,
                                 DomainScalar const &rhs)
        {
                return lhs.value() >= rhs.value();
        }
        DomainScalar operator- (DomainScalar const &rhs) {
                return DomainScalar (-rhs.value());
        }
        DomainScalar operator - (DomainScalar const &lhs,
                                 DomainScalar const &rhs
        ) {
                return DomainScalar (lhs.value() - rhs.value());
        }


        DomainInterval::DomainInterval() {}
        DomainInterval::DomainInterval (DomainScalar from, DomainScalar to)
        : from_(from), to_(to)
        {}

        DomainScalar DomainInterval::from() const { return from_; }
        DomainScalar DomainInterval::to  () const { return to_; }

        void DomainInterval::setFrom (DomainScalar val) { from_ = val; }
        void DomainInterval::setTo   (DomainScalar val) { to_ = val; }
        DomainScalar DomainInterval::min() const {
                return from_<to_ ? from_ : to_;
        }
        DomainScalar DomainInterval::max() const {
                return from_>to_ ? from_ : to_;
        }
        bool DomainInterval::isLinear() const {
                return true;
        }
        bool DomainInterval::isFinite() const {
                return false;
                // note that a Declaration's type might still make the
                // declaration finite.
        }
        unsigned int DomainInterval::elementCount() const {
                throw std::runtime_error("DomainInterval::elementCount() "
                                         "called for non-finite DomainInterval"
                                         );
        }
        std::list<DomainScalar> DomainInterval::elements() const {
                throw std::runtime_error("DomainInterval::elements() "
                                 "called for non-finite DomainInterval"
                                 );
        }



        /*DomainFilename::DomainFilename() {
        }
        DomainFilename::DomainFilename(std::string const &val) : filename_(val)
        {
        }
        void DomainFilename::setValue (std::string const &val) {
                filename_ = val;
        }
        std::string DomainFilename::value() const {
                return filename_;
        }*/


        DomainEnumeration::DomainEnumeration() {
        }
        std::vector<std::string> DomainEnumeration::elements() const {
                return elements_;
        }

        void DomainEnumeration::push_back (DomainEnumeration::value_type const &d) {
                elements_.push_back (d);
        }

        DomainEnumeration::iterator       DomainEnumeration::begin() {
                return elements_.begin();
        }
        DomainEnumeration::const_iterator DomainEnumeration::begin() const {
                return elements_.begin();
        }
        DomainEnumeration::iterator       DomainEnumeration::end() {
                return elements_.end();
        }
        DomainEnumeration::const_iterator DomainEnumeration::end() const {
                return elements_.end();
        }

        DomainEnumeration::size_type      DomainEnumeration::size () const {
                return elements_.size();
        }

        DomainEnumeration::value_type       &DomainEnumeration::at         (DomainEnumeration::size_type i) {
                return elements_.at(i);
        }
        const DomainEnumeration::value_type &DomainEnumeration::at         (DomainEnumeration::size_type i) const {
                return elements_.at(i);
        }
        DomainEnumeration::value_type       &DomainEnumeration::operator[] (DomainEnumeration::size_type i) {
                return elements_[i];
        }
        const DomainEnumeration::value_type &DomainEnumeration::operator[] (DomainEnumeration::size_type i) const {
                return elements_[i];
        }




        DomainValue::DomainValue (DomainScalar scalar) : type_(Scalar), scalar_(scalar) {}
        DomainValue::DomainValue (DomainInterval inter): type_(Interval), interval_(inter) {}
        DomainValue::DomainValue (DomainEnumeration enumeration)
                : type_(Enumeration), enumeration_(enumeration) {}

        DomainType DomainValue::type() const { return type_; }

        void DomainValue::toScalar (DomainScalar scalar) {
                reset();
                type_ = Scalar;
                scalar_ = scalar;
        }
        void DomainValue::toInterval (DomainInterval interval) {
                reset();
                type_ = Interval;
                interval_ = interval;
        }
        void DomainValue::toEnumeration (DomainEnumeration enumeration) {
                reset();
                type_ = Enumeration;
                enumeration_ = enumeration;
        }
        /*void DomainValue::toUnrestricted() {
                reset();
                type_ = Unrestricted;
        }*/
        /*void DomainValue::toFilename (DomainFilename filename) {
                reset();
                type_ = Filename;
                filename_ = filename;
        }*/
        void DomainValue::reset() {
                //filename_ = optional<DomainFilename>();
                interval_ = optional<DomainInterval>();
                scalar_ = optional<DomainScalar>();
        }

        DomainScalar DomainValue::scalar() const {
                if (!scalar_) {
                        throw std::runtime_error(
                                "DomainValue::scalar() called,"
                                " but it is not a scalar"
                        );
                }
                return *scalar_;
        }
        DomainInterval DomainValue::interval() const {
                if (!interval_) {
                        throw std::runtime_error(
                                "DomainValue::interval() called,"
                                " but it is not an interval"
                        );
                }
                return *interval_;
        }
        DomainEnumeration DomainValue::enumeration() const {
                if (!enumeration_) {
                        throw std::runtime_error(
                                "DomainValue::enumeration() called,"
                                " but it is not an enumeration"
                        );
                }
                return *enumeration_;
        }
        /*DomainInterval DomainValue::filename() const {
                if (!filename_) {
                        throw std::runtime_error(
                                "DomainValue::filename() called,"
                                " but it is not an interval"
                        );
                }
                return *filename_;
        }*/

        DomainScalar DomainValue::min() const {
                switch (type()) {
                case Scalar: return scalar();
                case Interval: return interval_->min();
                case Enumeration: throw std::logic_error(
                                "DomainValue::min() called for Enumeration");
                }
                throw std::runtime_error("in DomainValue::min(), "
                        "an enum value is not handled in a switch statement");
        }
        DomainScalar DomainValue::max() const {
                switch (type()) {
                case Scalar: return scalar();
                case Interval: return interval_->max();
                case Enumeration: throw std::logic_error(
                                "DomainValue::max() called for Enumeration");
                }
                throw std::runtime_error("in DomainValue::max(), "
                        "an enum value is not handled in a switch statement");
        }
        bool DomainValue::isLinear() const {
                switch (type()) {
                case Scalar: return true;
                case Interval: return interval_->isLinear();
                case Enumeration: return false;
                }
                return false;
        }
        bool DomainValue::isFinite() const {
                switch (type()) {
                case Scalar: return true;
                case Interval: return interval_->isFinite();
                case Enumeration: return true;
                }
                return false;
        }
        unsigned int DomainValue::elementCount() const {
                if (!isFinite())
                        throw std::runtime_error("DomainValue::elementCount() "
                                                 "called for DomainValue that "
                                                 "is not finite");
                switch (type()) {
                case Scalar: return 1;
                case Interval: return interval_->elementCount();
                case Enumeration: return enumeration_->size();
                /*case Unrestricted: throw std::logic_error(
                        "DomainValue::elementCount() called for Unrestricted");*/
                }
                throw std::runtime_error("in DomainValue::elementCount(), "
                        "an enum value is not handled in a switch statement");
        }
        std::list<DomainScalar> DomainValue::elements() const {
                if (!isFinite())
                        throw std::runtime_error("DomainValue::elements() "
                                 "called for non-finite DomainValue");

                switch (type()) {
                case Scalar: {
                        std::list<DomainScalar> ret;
                        ret.push_back (*scalar_);
                        return ret;
                }
                case Interval:
                        return interval_->elements();
                case Enumeration: throw std::logic_error(
                        "DomainValue::elements() called for Enumeration");
                /*case Unrestricted: throw std::logic_error(
                        "DomainValue::isLinear() called for Unrestricted");*/
                }
                throw std::runtime_error("in DomainValue::elements(), "
                        "an enum value is not handled in a switch statement");
        }



        DomainScalar Domain::min() const {
                DomainScalar val = DomainScalar::max();
                for (const_iterator it = values_.begin(), end = values_.end();
                        it != end; ++it)
                {
                        DomainScalar tmp = it->min();
                        if (tmp < val)
                                val = tmp;
                }
                return val;
        }
        DomainScalar Domain::max() const {
                DomainScalar val = -DomainScalar::max();
                for (const_iterator it = values_.begin(), end = values_.end();
                        it != end; ++it)
                {
                        DomainScalar tmp = it->max();
                        if (tmp > val)
                                val = tmp;
                }
                return val;
        }
        bool Domain::isLinear () const {
                if (values_.size() == 1) {
                        return values_[0].isLinear();
                } /*else if (values_.size() >= 2) {

                        if (values_[0].type() != Scalar
                            || values_[1].type() != Scalar)
                                return false;

                        const DomainScalar step = values_[1].scalar() -
                                                  values_[0].scalar();
                        for (const_iterator
                             it=values_.begin()+1, end=values_.end()-1;
                             it != end; ++it)
                        {
                                if (it->type() != Scalar
                                    || (it+1)->type() != Scalar)
                                        return false;

                                const DomainScalar diff = (it+1)->scalar()-
                                                          it->scalar();
                                if (std::fabs(step, diff) > 0.0001)
                                        return false;
                        }
                        return true;
                }*/
                return false;
        }
        bool Domain::empty() const {
                return values_.size() == 0;
        }
        bool Domain::isFinite() const {
                if (values_.size() == 0) {
                        return false;
                } else if (values_.size() == 1) {
                        return values_[0].isFinite();
                } else {
                        for (const_iterator
                             it=values_.begin(), end=values_.end();
                             it != end; ++it)
                        {
                                if (!it->isFinite())
                                        return false;
                        }
                        return true;
                }
        }
        bool Domain::isEnumeration() const {
                if (values_.size() != 1) return false;
                return values_[0].type() == Enumeration;
        }
        DomainEnumeration Domain::asEnumeration() const {
                if (!isEnumeration())
                        throw std::logic_error("Domain::asEnumeration() called "
                                "on domain that is not an enumeration");
                return values_[0].enumeration();
        }
        unsigned int Domain::elementCount() const {
                if (!isFinite())
                        throw std::runtime_error("Domain::elementCount() called"
                                                 " for non-finite Domain");
                unsigned int ret = 0;
                for (const_iterator
                     it=values_.begin(), end=values_.end();
                     it != end; ++it)
                {
                        ret += it->elementCount();
                }
                return ret;
        }
        std::list<DomainScalar> Domain::elements() const {
                if (!isFinite())
                        throw std::runtime_error("Domain::elements() called"
                                                 " for non-finite Domain");
                std::list<DomainScalar> ret;
                for (const_iterator
                     it=values_.begin(), end=values_.end();
                     it != end; ++it)
                {
                        std::list<DomainScalar> l = it->elements();
                        ret.insert (ret.end(), l.begin(), l.end());
                }
                return ret;
        }

        void Domain::push_back (DomainValue const &d) { values_.push_back (d); }

        Domain::iterator       Domain::begin()       { return values_.begin(); }
        Domain::const_iterator Domain::begin() const { return values_.begin(); }
        Domain::iterator       Domain::end()         { return values_.end();   }
        Domain::const_iterator Domain::end()   const { return values_.end();   }

        Domain::size_type      Domain::size () const { return values_.size(); }

        DomainValue       &Domain::at         (size_type i)       { return values_.at(i); }
        const DomainValue &Domain::at         (size_type i) const { return values_.at(i); }
        DomainValue       &Domain::operator[] (size_type i)       { return values_[i]; }
        const DomainValue &Domain::operator[] (size_type i) const { return values_[i]; }



        Declaration::Declaration () : type_(Real)
        {}

        std::string Declaration::id() const { return id_; }
        void Declaration::setId(std::string const & id) { id_ = id; }

        std::string Declaration::displayName() const {
                // 0) remove leading $
                const std::string nd = id_.substr(1);

                // 1) insert whitespace before inner uppercase letters
                std::string tmp;
                bool isInner = false;
                for (std::string::const_iterator it=nd.begin(), end=nd.end();
                        it != end; ++it)
                {
                        if (isInner && isupper(*it))
                                tmp += " ";
                        tmp += *it;
                        isInner = true;
                }
                return tmp;
        }

        DeclaredType Declaration::type() const { return type_; }
        void Declaration::setType(DeclaredType const &t) { type_ = t; }

        Domain Declaration::domain() const { return domain_; }
        void Declaration::setDomain(Domain const &d) { domain_ = d; }

        DomainScalar Declaration::domainMin() const {
                return domain_.min();
        }
        DomainScalar Declaration::domainMax() const {
                return domain_.max();
        }
        bool Declaration::hasFiniteDomain() const {
                if (domain_.isFinite())
                        return true;
                // It might still be finite as for the declared type.
                if (type_ == Boolean)
                        return true;
                return false;
        }
        bool Declaration::hasImplicitDomain() const {
                return domain_.empty();
        }
        unsigned int Declaration::domainElementCount() const {
                if (!hasFiniteDomain())
                        throw std::runtime_error(
                                "Declaration::domainElementCount() called for "
                                "Declaration that does not have a finite "
                                "Domain");
                if (type_ == Boolean)
                        return 2;
                return domain_.elementCount();
        }
        std::list<DomainScalar> Declaration::domainElements() const {
                if (!hasFiniteDomain())
                        throw std::runtime_error(
                                "Declaration::domainElements() called for "
                                "Declaration that does not have a finite "
                                "Domain");
                if (type_ == Boolean) {
                        std::list<DomainScalar> ds;
                        ds.push_back (0);
                        ds.push_back (1);
                        return ds;
                }
                return domain_.elements();
        }
        bool Declaration::hasDefaultValue() const {
                return defaultValue_;
        }
        DomainValue Declaration::defaultValue() const {
                return *defaultValue_;
        }
        void Declaration::setDefaultValue (DomainValue const &d) {
                defaultValue_ = d;
        }



        /*DomainScalar Declarations::domainMin() const {
                DomainScalar val = DomainScalar::max();
                for (const_iterator
                        it = declarations_.begin(), end = declarations_.end();
                        it != end; ++it)
                {
                        const DomainScalar tmp = it->domainMin();
                        if (tmp < val)
                                val = tmp;
                }
                return val;
        }

        DomainScalar Declarations::domainMax() const {
                DomainScalar val = -DomainScalar::max();
                for (const_iterator
                        it = declarations_.begin(), end = declarations_.end();
                        it != end; ++it)
                {
                        const DomainScalar tmp = it->domainMax();
                        if (tmp > val)
                                val = tmp;
                }
                return val;
        }*/

        void Declarations::push_back (Declaration const &d) { declarations_.push_back (d); }

        Declarations::iterator       Declarations::begin()       { return declarations_.begin(); }
        Declarations::const_iterator Declarations::begin() const { return declarations_.begin(); }
        Declarations::iterator       Declarations::end()         { return declarations_.end();   }
        Declarations::const_iterator Declarations::end()   const { return declarations_.end();   }

        Declarations::size_type      Declarations::size () const { return declarations_.size(); }

        Declaration       &Declarations::at         (size_t i)       { return declarations_.at(i); }
        const Declaration &Declarations::at         (size_t i) const { return declarations_.at(i); }
        Declaration       &Declarations::operator[] (size_t i)       { return declarations_[i]; }
        const Declaration &Declarations::operator[] (size_t i) const { return declarations_[i]; }
}
