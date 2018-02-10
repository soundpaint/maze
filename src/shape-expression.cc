/*
 * Maze -- A maze / flipper game implementation for RPi with Sense Hat
 * Copyright (C) 2016, 2017, 2018 Jürgen Reuter
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
#include <cmath>
#include <log.hh>

Shape_prime::Shape_prime(const Implicit_curve *implicit_curve,
                         const bool negated) :
  Shape_unary_expression(negated)
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

const double
Shape_prime::get_avg_tan(const double x, const double y) const
{
  const double raw_theta = _implicit_curve->get_avg_tan(x, y);
  return is_negated() ? -raw_theta : raw_theta;
}

const std::string
Shape_prime::to_string() const
{
  std::stringstream str;
  str << "Shape_prime{negated=" << is_negated() <<
    ", implicit_curve=" << _implicit_curve->to_string();
  str <<"}";
  return std::string(str.str());
}

Shape_unary_expression::Shape_unary_expression() :
  Shape_unary_expression(false)
{
}

Shape_unary_expression::Shape_unary_expression(const bool negated) {
  _negated = negated;
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

const double
Shape_factors::get_avg_tan(const double x, const double y) const
{
  if (size() == 0) {
    return std::nan("");
  }
  const bool inside = is_inside(x, y);
  uint16_t count = 0;
  double theta = 0.0;
  for (const Shape_unary_expression *factor : *_factors) {
    if (factor->is_inside(x, y) == inside) {
      const double factor_theta = factor->get_avg_tan(x, y);
      if (!std::isnan(factor_theta)) {
        theta += factor_theta;
        count++;
      }
    }
  }
  if (!count) {
    return std::nan("");
  }
  return theta / count;
}

const std::string
Shape_factors::to_string() const
{
  std::stringstream str;
  str << "Shape_factors{factors={";
  bool first = true;
  for (const Shape_unary_expression *factor : *_factors) {
    if (first) {
      first = false;
    } else {
      str << ", ";
    }
    str << factor->to_string();
  }
  str <<"}}";
  return std::string(str.str());
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

const double
Shape_terms::get_avg_tan(const double x, const double y) const
{
  if (size() == 0) {
    return std::nan("");
  }
  const bool inside = is_inside(x, y);
  uint16_t count = 0;
  double theta = 0.0;
  for (const Shape_factors *term : *_terms) {
    if (term->is_inside(x, y) == inside) {
      const double term_theta = term->get_avg_tan(x, y);
      if (!std::isnan(term_theta)) {
        theta += term_theta;
        count++;
      }
    }
  }
  if (!count) {
    return std::nan("");
  }
  return (is_negated() ? -theta : theta) / count;
}

const std::string
Shape_terms::to_string() const
{
  std::stringstream str;
  str << "Shape_terms{negated=" << is_negated() << ", terms={";
  bool first = true;
  for (const Shape_factors *term : *_terms) {
    if (first) {
      first = false;
    } else {
      str << ", ";
    }
    str << term->to_string();
  }
  str <<"}}";
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
