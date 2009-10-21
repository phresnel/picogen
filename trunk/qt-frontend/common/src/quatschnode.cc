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



#include "kallisto/common.hh"
#include "../include/quatschnode.hh"


std::string QuatschNode::genJuxCode (JuxGeneratorState &state) const {
        //return "([Noise2d frequency{10} filter{nearest} seed{41}] x y)";
        JuxGeneratorState::Frame frame = state.getFrame();
        const std::string indent = state.getIndendationString();

        std::string tmp;

        std::string prefix = "";
        std::string postfix = "";
        switch (value.getScaleOffsetMode()) {
        case QuatschNodeValue::disable:
                break;
        case QuatschNodeValue::scale_offset:
                prefix = "(+ " + number(value.getOffset())
                       + " (* " + number(value.getScale()) + " ";
                postfix = "))";
                break;
        case QuatschNodeValue::offset_scale:
                prefix = "(* " + number(value.getScale())
                       + " (+ " + number(value.getOffset()) + " ";
                postfix = "))";
                break;
        };

        switch (this->type) {
        case PredefinedConstant:
                switch (value.asPredefinedConstant()) {
                case QuatschNodeValue::Pi: return indent + prefix + "3.1415926535897932385/*pi*/" + postfix + "\n";
                case QuatschNodeValue::e:  return indent + prefix + "2.7182818284590452355/*e*/" + postfix + "\n";
                };

        case UserConstant:
                return indent + prefix + number(value.asFloatConstant()) + postfix + "\n";

        case Parameter:
                return indent + prefix + value.asParameter() + postfix + "\n";

        case Undefined:

        case Addition:
                tmp = indent + "( +\n";
                goto aggregate;
        case Subtraction:
                tmp = indent + "( -\n";
                goto aggregate;
        case Multiplication:
                tmp = indent + "( *\n";
                goto aggregate;
        case Division:
                tmp = indent + "( /\n";
                goto aggregate;

        case Exponentiate:
                tmp = indent + "( ^ \n";
                goto aggregate;
        case Minimize:
                tmp = indent + "( min \n";
                goto aggregate;
        case Maximize:
                tmp = indent + "( max \n";
                goto aggregate;
        case Negate:
                tmp = indent + "( neg \n";
                goto aggregate;

        case Lerp:
                tmp = indent + "( lerp \n";
                goto aggregate;

        case And:
                tmp = indent + "( and \n";
                goto aggregate;
        case Or:
                tmp = indent + "( or \n";
                goto aggregate;
        case Not:
                tmp = indent + "( not \n";
                goto aggregate;
        case LessThan:
                tmp = indent + "( < \n";
                goto aggregate;
        case LessThanOrEqual:
                tmp = indent + "( <= \n";
                goto aggregate;
        case GreaterThan:
                tmp = indent + "( > \n";
                goto aggregate;
        case GreaterThanOrEqual:
                tmp = indent + "( >= \n";
                goto aggregate;

        case Inverse:
                tmp = indent + "( inv\n";
                goto aggregate;
        case Sine:
                tmp = indent + "( sin\n";
                goto aggregate;
        case Cosine:
                tmp = indent + "( cos\n";
                goto aggregate;

        case Floor:
                tmp = indent + "( floor\n";
                goto aggregate;
        case Absolute:
                tmp = indent + "( abs\n";
                goto aggregate;
        case Truncate:
                tmp = indent + "( trunc\n";
                goto aggregate;
        case Fractional:
                tmp = indent + "( frac\n";
                goto aggregate;

        case Sqrt:
                tmp = indent + "( sqrt\n";
                goto aggregate;
        case Log:
                tmp = indent + "( log\n";
                goto aggregate;
        case Log10:
                tmp = indent + "( log10\n";
                goto aggregate;
        case Exp:
                tmp = indent + "( exp\n";
                goto aggregate;

        case IfThenElse:
                tmp = indent + "(if \n";
                goto aggregate;

        case Noise2d:
                tmp = indent + "( [Noise2d " +
                      "frequency{" + number(value.asNoise2d().width) + "} " +
                      "seed{" + number(value.asNoise2d().seed) + "}";
                switch (value.asNoise2d().filter) {
                case QuatschNodeValue::Noise2d::Nearest: tmp += "filter{nearest}"; break;
                case QuatschNodeValue::Noise2d::Bilinear: tmp += "filter{bilinear}"; break;
                case QuatschNodeValue::Noise2d::Cosine: tmp += "filter{cosine}"; break;
                };
                tmp += "] \n";
                goto aggregate;
        case LayeredNoise2d:
                tmp = indent + "( [LayeredNoise2d " +
                      "frequency{" + number(value.asLayeredNoise2d().width) + "} " +
                      "seed{" + number(value.asLayeredNoise2d().seed) + "}"
                      "persistence{0.5}" // + std::string::number(value.asNoise2d().seed) + "}"
                      "layercount{" + number(value.asLayeredNoise2d().depth) + "}"
                ;
                switch (value.asLayeredNoise2d().filter) {
                case QuatschNodeValue::LayeredNoise2d::Nearest: tmp += "filter{nearest}"; break;
                case QuatschNodeValue::LayeredNoise2d::Bilinear: tmp += "filter{bilinear}"; break;
                case QuatschNodeValue::LayeredNoise2d::Cosine: tmp += "filter{cosine}"; break;
                };
                tmp += "] \n";
                goto aggregate;

        case MultiplyWithPi:
                tmp = indent + "( *\n"
                    + indent + indent + "3.1415926535897932385/*pi*/\n";
                goto aggregate;
        aggregate:

                if (hasDefaultParameters() && children.size() == 0) {
                       tmp += indent + getDefaultParameters();
                } else for (std::vector<QuatschNode>::const_iterator it=children.begin();
                     it != children.end();
                     ++it
                ) {
                        tmp += it->genJuxCode (state);
                }

                tmp += indent + ")\n";
                return prefix + tmp + postfix;
        };
        return "<err>";
        throw NodeException ("switch() in QuatschNode::genJuxCode() is incomplete");
}




bool QuatschNode::isTerminal () const {
        // Note: isTerminal() is not necessarily !isAggregate(), in case type is Undefined
        switch (type) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
                return true;

        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case And:
        case Or:
        case Not:
        case LessThan:
        case LessThanOrEqual:
        case GreaterThan:
        case GreaterThanOrEqual:
        case Inverse:
        case Sine:
        case Cosine:
        case Floor:
        case Absolute:
        case Truncate:
        case Fractional:
        case Sqrt:
        case Log:
        case Log10:
        case Exp:
        case IfThenElse:
        case Noise2d:
        case LayeredNoise2d:
        case MultiplyWithPi:
                return false;
        };
        throw NodeException ("switch() in QuatschNode::isTerminal() is incomplete");
}



bool QuatschNode::isAggregate () const {
        // Note: isTerminal() is not necessarily !isAggregate(), in case type is Undefined
        switch (type) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
                return false;

        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case And:
        case Or:
        case Not:
        case LessThan:
        case LessThanOrEqual:
        case GreaterThan:
        case GreaterThanOrEqual:
        case Inverse:
        case Sine:
        case Cosine:
        case Floor:
        case Absolute:
        case Truncate:
        case Fractional:
        case Sqrt:
        case Log:
        case Log10:
        case Exp:
        case IfThenElse:
        case Noise2d:
        case LayeredNoise2d:
        case MultiplyWithPi:
                return true;
        };
        throw NodeException ("switch() in QuatschNode::isAggregate() is incomplete");
}



bool QuatschNode::hasDefaultParameters () const {
        switch (type) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case And:
        case Or:
        case Not:
        case LessThan:
        case LessThanOrEqual:
        case GreaterThan:
        case GreaterThanOrEqual:
        case Inverse:
        case Sine:
        case Cosine:
        case Floor:
        case Absolute:
        case Truncate:
        case Fractional:
        case Sqrt:
        case Log:
        case Log10:
        case Exp:
        case IfThenElse:
        case MultiplyWithPi:
                return false;

        case Noise2d:
        case LayeredNoise2d:
                return true;
        };
        throw NodeException ("switch() in QuatschNode::isAggregate() is incomplete");
}



std::string QuatschNode::getDefaultParameters () const {
        switch (type) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case And:
        case Or:
        case Not:
        case LessThan:
        case LessThanOrEqual:
        case GreaterThan:
        case GreaterThanOrEqual:
        case Inverse:
        case Sine:
        case Cosine:
        case MultiplyWithPi:
        case Floor:
        case Absolute:
        case Truncate:
        case Fractional:
        case Sqrt:
        case Log:
        case Log10:
        case Exp:
        case IfThenElse:
                return false;

        case Noise2d:
        case LayeredNoise2d:
                return "x y";
        };
        throw NodeException ("switch() in isAggregate() is incomplete");
}



int QuatschNode::getMinimumParameterCount () const {
        return getParameterCount (true);
}



int QuatschNode::getMaximumParameterCount () const {
        return getParameterCount (false);
}



int QuatschNode::getParameterCount (bool getMinCount) const {
        switch (type) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
                return 0;

        case Undefined:
                return -1;

        case Addition:
                if (getMinCount) return 2;
                else return -1;
        case Subtraction:
                if (getMinCount) return 2;
                else return -1;
        case Multiplication:
                if (getMinCount) return 2;
                else return -1;
        case Division:
                if (getMinCount) return 2;
                else return -1;

        case Exponentiate:
                if (getMinCount) return 2;
                else return -1;
        case Minimize:
                if (getMinCount) return 1;
                else return -1;
        case Maximize:
                if (getMinCount) return 1;
                else return -1;
        case Negate:
                if (getMinCount) return 1;
                else return 1;

        case Lerp:
                if (getMinCount) return 2;
                else return -1;

        case And:
                if (getMinCount) return 2;
                else return -1;
        case Or:
                if (getMinCount) return 2;
                else return -1;
        case Not:
                if (getMinCount) return 1;
                else return 1;
        case LessThan:
                if (getMinCount) return 2;
                else return -1;
        case LessThanOrEqual:
                if (getMinCount) return 2;
                else return -1;
        case GreaterThan:
                if (getMinCount) return 2;
                else return -1;
        case GreaterThanOrEqual:
                if (getMinCount) return 2;
                else return -1;

        case Inverse:
                if (getMinCount) return 1;
                else return 1;
        case Sine:
                if (getMinCount) return 1;
                else return 1;
        case Cosine:
                if (getMinCount) return 1;
                else return 1;

        case Floor:
                if (getMinCount) return 1;
                else return 1;
        case Absolute:
                if (getMinCount) return 1;
                else return 1;
        case Truncate:
                if (getMinCount) return 1;
                else return 1;
        case Fractional:
                if (getMinCount) return 1;
                else return 1;

        case Sqrt:
                if (getMinCount) return 1;
                else return 1;
        case Log:
                if (getMinCount) return 1;
                else return 1;
        case Log10:
                if (getMinCount) return 1;
                else return 1;
        case Exp:
                if (getMinCount) return 1;
                else return 1;

        case IfThenElse:
                if (getMinCount) return 3;
                else return 3;

        case Noise2d:
                if (getMinCount) return 2;
                else return 2;
        case LayeredNoise2d:
                if (getMinCount) return 2;
                else return 2;

        case MultiplyWithPi:
                if (getMinCount) return 1;
                else return 1;
        };
        throw NodeException ("switch() in getParameterCount() is incomplete");
}



bool QuatschNode::isChildCountOkay () const {
        if (isTerminal() && children.size() == 0)
                return true;
        if (hasDefaultParameters() && children.size() == 0)
                return true;
        if (isAggregate()) {
                const int
                        minpc = getMinimumParameterCount(),
                        maxpc = getMaximumParameterCount(),
                        cc = children.size(),
                        min_ok = (minpc == -1) || (cc >= minpc),
                        max_ok = (maxpc == -1) || (cc <= maxpc),
                        both_ok = min_ok && max_ok
                ;
                if (both_ok)
                        return true;
        }
        return false;
}



bool QuatschNode::areChildrenCompilable () const {
        using std::vector;
        for (vector<QuatschNode>::const_iterator it=children.begin();
             it!=children.end();
             ++it)
        {
                if (!it->isCompilable())
                        return false;
        }
        return true;
}



int QuatschNode::getMissingChildrenCount () const {
        if (isAggregate()) {
                const int cs = static_cast<int>(children.size());

                if (hasDefaultParameters() && cs == 0)
                        return 0;

                if (-1 != getMinimumParameterCount()
                &&  cs < getMinimumParameterCount()) {
                        return getMinimumParameterCount() - cs;
                }

                if (-1 != getMaximumParameterCount()
                && cs > getMaximumParameterCount()) {
                        return getMaximumParameterCount() - cs;
                }
        }
        return 0;
}



bool QuatschNode::isCompilable () const {
        if (isChildCountOkay() && areChildrenCompilable())
                return true;
        return false;
}
