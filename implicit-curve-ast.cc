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

Implicit_curve_ast::Implicit_curve_ast()
{
}

Implicit_curve_ast::~Implicit_curve_ast()
{
}

Implicit_curve_ast::Implicit_curve *
Implicit_curve_ast::get_implicit_curve()
{
  return &_implicit_curve;
}

Implicit_curve_ast::Term::Term(const Sign sign, const double weight) :
  _sign(sign), _weight(weight)
{
}

Implicit_curve_ast::Term::~Term()
{
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

Implicit_curve_ast::Implicit_curve::Implicit_curve()
{
}

Implicit_curve_ast::Implicit_curve::~Implicit_curve()
{
}

std::vector<Implicit_curve_ast::Term *> *
Implicit_curve_ast::Implicit_curve::get_terms()
{
  return &_terms;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
