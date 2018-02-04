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

#include <bivariate-quadratic-function.hh>
#include <string>
#include <cmath>
#include <log.hh>

Bivariate_quadratic_function::Bivariate_quadratic_function(const double weight_term_yy,
                                                           const double weight_term_xy,
                                                           const double weight_term_xx,
                                                           const double weight_term_y,
                                                           const double weight_term_x,
                                                           const double weight_term_const) :
  _weight_term_yy(weight_term_yy),
  _weight_term_xy(weight_term_xy),
  _weight_term_xx(weight_term_xx),
  _weight_term_y(weight_term_y),
  _weight_term_x(weight_term_x),
  _weight_term_const(weight_term_const)
{
}

Bivariate_quadratic_function::~Bivariate_quadratic_function()
{
}

const bool
Bivariate_quadratic_function::is_inside(const double x, const double y) const
{
  return
    _weight_term_yy * y * y +
    _weight_term_xy * x * y +
    _weight_term_xx * x * x +
    _weight_term_y * y +
    _weight_term_x * x +
    _weight_term_const <= 0.0;
}

const double
Bivariate_quadratic_function::get_avg_tan(const double x, const double y) const
{
  // F_x(x, y) := df(x, y) / dx = w_xy * y + 2.0 * w_xx * x + w_x
  // F_y(x, y) := df(x, y) / dy = 2.0 * w_yy * y + w_xy * x + w_y
  //
  // norm vector: n(x0, y0) = (F_x(x0, y0), F_y(x, y0))^T
  //
  const double fx =
    _weight_term_xy * y + 2.0 * _weight_term_xx * x + _weight_term_x;
  const double fy =
    2.0 * _weight_term_yy * y + _weight_term_xy * x + _weight_term_y;
  double theta;
  if ((fx == 0.0) && (fy == 0.0)) {
    theta = std::nan("");
  } else {
    theta = M_PI - std::atan2(fy, fx);
  }
  return theta;
}

const Bivariate_quadratic_function *
Bivariate_quadratic_function::create_d_dx() const
{
  const Bivariate_quadratic_function *d_dx =
    new Bivariate_quadratic_function(0.0,
                                     0.0,
                                     0.0,
                                     _weight_term_xy,
                                     2.0 * _weight_term_xx,
                                     _weight_term_x);
  if (!d_dx) {
    Log::fatal("not enough memory");
  }
  return d_dx;
}

const Bivariate_quadratic_function *
Bivariate_quadratic_function::create_d_dy() const
{
  const Bivariate_quadratic_function *d_dy =
    new Bivariate_quadratic_function(0.0,
                                     0.0,
                                     0.0,
                                     2.0 * _weight_term_yy,
                                     _weight_term_xy,
                                     _weight_term_y);
  if (!d_dy) {
    Log::fatal("not enough memory");
  }
  return d_dy;
}

const std::string
Bivariate_quadratic_function::to_string() const
{
  std::stringstream str;
  str << "(";
  str << _weight_term_yy << "y² + ";
  str << _weight_term_xy << "xy + ";
  str << _weight_term_xx << "x² + ";
  str << _weight_term_y << "y + ";
  str << _weight_term_x << "x + ";
  str << _weight_term_const;
  str << " <= 0.0)";
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
