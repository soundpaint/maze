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

#include <implicit-curve.hh>
#include <log.hh>

Implicit_curve::Implicit_curve(const double weight_term_yy,
                               const double weight_term_xy,
                               const double weight_term_xx,
                               const double weight_term_y,
                               const double weight_term_x,
                               const double weight_term_const) :
  Implicit_curve(create_bivariate_quadratic_function(weight_term_yy,
                                                     weight_term_xy,
                                                     weight_term_xx,
                                                     weight_term_y,
                                                     weight_term_x,
                                                     weight_term_const))
{
}

Implicit_curve::Implicit_curve(const Bivariate_quadratic_function *function)
  : Implicit_curve(function,
                   function->create_d_dx(),
                   function->create_d_dy())
{
}

Implicit_curve::Implicit_curve(const Bivariate_quadratic_function *function,
                               const Bivariate_quadratic_function *function_d_dx,
                               const Bivariate_quadratic_function *function_d_dy)
{
  if (!function) {
    Log::fatal("function is null");
  }
  if (!function_d_dx) {
    Log::fatal("function_d_dx is null");
  }
  if (!function_d_dy) {
    Log::fatal("function_d_dy is null");
  }
  _function = function;
  _function_d_dx = function_d_dx;
  _function_d_dy = function_d_dy;
}

Implicit_curve::~Implicit_curve()
{
  _function = 0;
  delete _function_d_dx;
  _function_d_dx = 0;
  delete _function_d_dy;
  _function_d_dy = 0;
}

const Bivariate_quadratic_function *
Implicit_curve::create_bivariate_quadratic_function(const double weight_term_yy,
                                                    const double weight_term_xy,
                                                    const double weight_term_xx,
                                                    const double weight_term_y,
                                                    const double weight_term_x,
                                                    const double weight_term_const)
{
  Bivariate_quadratic_function *function =
    new Bivariate_quadratic_function(weight_term_yy,
                                     weight_term_xy,
                                     weight_term_xx,
                                     weight_term_y,
                                     weight_term_x,
                                     weight_term_const);
  if (!function) {
    Log::fatal("not enough memory");
  }
  return function;
}

const bool
Implicit_curve::is_inside(const double x, const double y) const
{
  return _function->is_inside(x, y);
}

const double
Implicit_curve::get_avg_tan(const double x, const double y) const
{
  return _function->get_avg_tan(x, y);
}

const std::string
Implicit_curve::to_string() const
{
  return _function->to_string();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
