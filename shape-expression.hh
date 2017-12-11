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

#ifndef SHAPE_EXPRESSION_HH
#define SHAPE_EXPRESSION_HH

#include <vector>
#include <iterator.hh>
#include <implicit-curve.hh>

/*
 * shape_expression ::= shape_terms .
 * shape_terms ::= shape_term |
 *                 "<or>" shape_term+ "</or>" .
 * shape_term ::= shape_factors .
 * shape_factors ::= shape_unary_expression |
 *                   "<and>" shape_unary_expression+ "</and>" .
 * shape_unary_expression ::= shape_primary_expression |
 *                            "<not>" shape_primary_expression "</not>" .
 * shape_primary_expression ::= shape_prime | shape_expression .
 * shape_prime ::= implicit_curve .
 * implicit_curve ::= "<implicit-curve>" curve_expression "</implicit-curve>" .
 */

class Shape_unary_expression
{
public:
  virtual ~Shape_unary_expression();
  const bool is_negated() const;
  void set_negated(const bool negated);
  virtual const bool is_inside(const double x, const double y) const = 0;
private:
  bool _negated;
};

class Shape_prime : public Shape_unary_expression
{
public:
  Shape_prime(const Implicit_curve *implicit_curve);
  virtual ~Shape_prime();
  const Implicit_curve *get_implicit_curve() const;
  const bool is_inside(const double x, const double y) const;
private:
  const Implicit_curve *_implicit_curve;
};

class Shape_terms;

class Shape_factors
{
public:
  Shape_factors();
  virtual ~Shape_factors();
  void add_factor(const Shape_unary_expression *factor);
  const int size() const;
  virtual void clear();
  //virtual const Iterator<Shape_unary_expression> *create_iterator() const;
  const bool is_inside(const double x, const double y) const;
private:
  std::vector<const Shape_unary_expression *> *_factors;
};

class Shape_terms : public Shape_unary_expression
{
public:
  Shape_terms();
  virtual ~Shape_terms();
  void add_term(const Shape_factors *term);
  const int size() const;
  virtual void clear();
  //virtual const Iterator<Shape_factors> *create_iterator() const;
  const bool is_inside(const double x, const double y) const;
private:
  std::vector<const Shape_factors *> *_terms;
};

#endif /* SHAPE_EXPRESSION_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
