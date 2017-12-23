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

#include <shape-expression.hh>
#include <log.hh>

Shape_prime::Shape_prime(const Implicit_curve *implicit_curve)
{
  if (!implicit_curve) {
    Log::fatal("implicit_curve is null");
  }
  _implicit_curve = implicit_curve;
}

Shape_prime::~Shape_prime() {
  _implicit_curve = 0;
}

const Implicit_curve *
Shape_prime::get_implicit_curve() const
{
  return _implicit_curve;
}

const bool
Shape_prime::is_inside(const double x, const double y) const
{
  return _implicit_curve->is_inside(x, y) != is_negated();
}

Shape_unary_expression::Shape_unary_expression() {
  _negated = false;
}

Shape_unary_expression::~Shape_unary_expression() {
  _negated = false;
}

void
Shape_unary_expression::set_negated(const bool negated)
{
  _negated = negated;
}

const bool
Shape_unary_expression::is_negated() const
{
  return _negated;
}

Shape_factors::Shape_factors()
{
  _factors = new std::vector<const Shape_unary_expression *,
                             std::allocator<const Shape_unary_expression *>>();
  if (!_factors) {
    Log::fatal("not enough memory");
  }
}

Shape_factors::~Shape_factors()
{
  // TODO: Check: First delete vector elements before deleting the
  // vector (memory leak)?
  delete _factors;
  _factors = 0;
}

void
Shape_factors::add_factor(const Shape_unary_expression *factor)
{
  _factors->push_back(factor);
  //return 0; // TODO
}

const int
Shape_factors::size() const
{
  return _factors->size();
}

void
Shape_factors::clear()
{
  _factors->clear();
}

const bool
Shape_factors::is_inside(const double x, const double y) const
{
  for (const Shape_unary_expression *factor : *_factors) {
    if (!(factor->is_inside(x, y)))
      return false;
  }
  return true;
}

Shape_terms::Shape_terms()
{
  _terms =
    new std::vector<const Shape_factors *,
		    std::allocator<const Shape_factors *>>();
  if (!_terms) {
    Log::fatal("not enough memory");
  }
}

Shape_terms::~Shape_terms()
{
  // TODO: Check: First delete vector elements before deleting the
  // vector (memory leak)?
  delete _terms;
  _terms = 0;
}

void
Shape_terms::add_term(const Shape_factors *term)
{
  _terms->push_back(term);
  //return 0; // TODO
}

const int
Shape_terms::size() const
{
  return _terms->size();
}

void
Shape_terms::clear()
{
  _terms->clear();
}

const bool
Shape_terms::is_inside(const double x, const double y) const
{
  for (const Shape_factors *term : *_terms) {
    if (term->is_inside(x, y))
      return !is_negated();
  }
  return is_negated();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
