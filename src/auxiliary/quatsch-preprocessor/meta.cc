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
#include "meta.hh"

namespace quatsch_preprocessor {

        DomainScalar::DomainScalar() {}
        DomainScalar::DomainScalar (double val) : value_ (val) {}

        void DomainScalar::setValue (double val) { value_ = val; }
        double DomainScalar::value () const { return value_; }

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


        DomainInterval::DomainInterval() {}
        DomainInterval::DomainInterval (DomainScalar from, DomainScalar to)
        : from_(from), to_(to)
        {}

        DomainScalar DomainInterval::from() const { return from_; }
        DomainScalar DomainInterval::to  () const { return to_; }

        void DomainInterval::setFrom (DomainScalar val) { from_ = val; }
        void DomainInterval::setTo   (DomainScalar val) { to_ = val; }




        DomainValue::DomainValue (DomainScalar scalar) : type_(Scalar), scalar_(scalar) {}
        DomainValue::DomainValue (DomainInterval inter): type_(Interval), interval_(inter) {}

        DomainType DomainValue::type() const { return type_; }

        void DomainValue::toScalar (DomainScalar scalar) {
                type_ = Scalar;
                scalar_ = scalar;
        }

        void DomainValue::toInterval (DomainInterval interval) {
                type_ = Interval;
                interval_ = interval;
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

        DomainScalar DomainValue::min() const {
                switch (type()) {
                case Scalar: return scalar();
                case Interval: return interval_->from() < interval_->to() ?
                                        interval_->from() :
                                        interval_->to()
                                        ;
                }
        }
        DomainScalar DomainValue::max() const {
                switch (type()) {
                case Scalar: return scalar();
                case Interval: return interval_->from() > interval_->to() ?
                                        interval_->from() :
                                        interval_->to()
                                        ;
                }
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

        void Domain::push_back (DomainValue const &d) { values_.push_back (d); }

        Domain::iterator       Domain::begin()       { return values_.begin(); }
        Domain::const_iterator Domain::begin() const { return values_.begin(); }
        Domain::iterator       Domain::end()         { return values_.end();   }
        Domain::const_iterator Domain::end()   const { return values_.end();   }

        Domain::size_type      Domain::size () const { return values_.size(); }

        DomainValue       &Domain::at         (size_t i)       { return values_.at(i); }
        const DomainValue &Domain::at         (size_t i) const { return values_.at(i); }
        DomainValue       &Domain::operator[] (size_t i)       { return values_[i]; }
        const DomainValue &Domain::operator[] (size_t i) const { return values_[i]; }



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



        DomainScalar Declarations::domainMin() const {
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
        }

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
