/*
 * Maze -- A maze / flipper game implementation for RPi with Sense Hat
 * Copyright (C) 2016, 2017  Jürgen Reuter
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 *
 * For updates and more info or contacting the author, visit:
 * <https://github.com/soundpaint/maze>
 *
 * Author's web site: www.juergen-reuter.de
 */

#include <implicit-curve-ast.hh>
#include <log.hh>

// Implicit_curve_ast syntactical structure

Implicit_curve_ast::Implicit_curve_ast()
{
}

Implicit_curve_ast::~Implicit_curve_ast()
{
}

void
Implicit_curve_ast::clear()
{
  get_implicit_curve()->clear();
}

Implicit_curve_ast::Implicit_curve *
Implicit_curve_ast::get_implicit_curve()
{
  return &_implicit_curve;
}

void
Implicit_curve_ast::add_term(const Term::Sign sign,
                             const double weight,
                             const Term::Variable variable)
{
  _implicit_curve.add_term(sign, weight, variable);
}

// Term syntactical structure

Implicit_curve_ast::Term::Term(const Sign sign, const double weight,
                               const Variable variable) :
  _sign(sign), _weight(weight), _variable(variable)
{
}

Implicit_curve_ast::Term::~Term()
{
}

const std::string
Implicit_curve_ast::Term::to_string() const
{
  std::stringstream str;
  str << "{Term: ";
  switch (_sign) {
  case PLUS:
    str << "+";
    break;
  case MINUS:
    str << "-";
    break;
  case SIGN_UNINITIALIZED:
    Log::fatal("uninitialzed term sign");
  default:
    Log::fatal("programming error: unknown term sign");
    break;
  }
  str << _weight;
  switch (_variable) {
  case VAR_CONST:
    break;
  case VAR_X:
    str << "x";
    break;
  case VAR_Y:
    str << "y";
    break;
  case VAR_XX:
    str << "x*x";
    break;
  case VAR_XY:
    str << "x*y";
    break;
  case VAR_YY:
    str << "y*y";
    break;
  case VAR_UNINITIALIZED:
    str << "";
    break;
  default:
    Log::fatal("programming error: unknown term variable");
    break;
  }
  str << "}";
  return std::string(str.str());
}

const Implicit_curve_ast::Term::Sign
Implicit_curve_ast::Term::get_sign() const
{
  return _sign;
}

const double
Implicit_curve_ast::Term::get_weight() const
{
  return _weight;
}

const Implicit_curve_ast::Term::Variable
Implicit_curve_ast::Term::get_variable() const
{
  return _variable;
}

// Implicit_curve syntactical structure

const std::string
Implicit_curve_ast::Implicit_curve::to_string() const
{
  std::stringstream str;
  str << "{Implicit_curve: ";
  for (size_t i = 0; i < _terms.size(); i++) {
    Term *term = _terms[i];
    if (i > 0) str << ", ";
    str << term->to_string();
  }
  str << "}";
  return std::string(str.str());
}

Implicit_curve_ast::Implicit_curve::Implicit_curve()
{
}

Implicit_curve_ast::Implicit_curve::~Implicit_curve()
{
}

// Implicit_curve syntactical structure

void
Implicit_curve_ast::Implicit_curve::clear()
{
  _terms.clear();
}

void
Implicit_curve_ast::Implicit_curve::add_term(const Term::Sign sign,
                                             const double weight,
                                             const Term::Variable variable)
{
  Term *term = new Term(sign, weight, variable);
  _terms.push_back(term);
}

const std::vector<Implicit_curve_ast::Term *> *
Implicit_curve_ast::Implicit_curve::get_terms() const
{
  return &_terms;
}

// Implicit_curve semantic attributes

const bool
Implicit_curve_ast::Implicit_curve::has_summarized_term_weight(const Term::Variable variable) const
{
  return _summarized_term_weights.count(&variable) > 0;
}

const double
Implicit_curve_ast::Implicit_curve::get_summarized_term_weight(const Term::Variable variable)
{
  return _summarized_term_weights[&variable];
}

const double
Implicit_curve_ast::Implicit_curve::
try_get_summarized_term_weight_with_default(const Term::Variable variable,
                                            const double default_value)
{
  return has_summarized_term_weight(variable) ?
    _summarized_term_weights[&variable] : default_value;
}

void
Implicit_curve_ast::Implicit_curve::set_summarized_term_weight(const Term::Variable variable,
                                                               const double weight)
{
  _summarized_term_weights[&variable] = weight;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
