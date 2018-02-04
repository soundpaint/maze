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

#ifndef IMPLICIT_CURVE_HH
#define IMPLICIT_CURVE_HH

#include <bivariate-quadratic-function.hh>

class Implicit_curve
{
public:
  Implicit_curve(const double weight_term_yy,
                 const double weight_term_xy,
                 const double weight_term_xx,
                 const double weight_term_y,
                 const double weight_term_x,
                 const double weight_term_const);
  virtual ~Implicit_curve();
  const bool is_inside(const double x, const double y) const;
  const double get_avg_tan(const double x, const double y) const;
  const std::string to_string() const;
private:
  const Bivariate_quadratic_function *_function;
  const Bivariate_quadratic_function *_function_d_dx;
  const Bivariate_quadratic_function *_function_d_dy;
  Implicit_curve(const Bivariate_quadratic_function *function);
  Implicit_curve(const Bivariate_quadratic_function *function,
                 const Bivariate_quadratic_function *function_d_dx,
                 const Bivariate_quadratic_function *function_d_dy);
  const static Bivariate_quadratic_function *
  create_bivariate_quadratic_function(const double weight_term_yy,
                                      const double weight_term_xy,
                                      const double weight_term_xx,
                                      const double weight_term_y,
                                      const double weight_term_x,
                                      const double weight_term_const);
};

#endif /* IMPLICIT_CURVE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
