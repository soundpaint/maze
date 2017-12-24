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

#ifndef BIVARIATE_QUADRATIC_FUNCTION_HH
#define BIVARIATE_QUADRATIC_FUNCTION_HH

#include <string>

class Bivariate_quadratic_function
{
public:
  Bivariate_quadratic_function(const double weight_term_yy,
                               const double weight_term_xy,
                               const double weight_term_xx,
                               const double weight_term_y,
                               const double weight_term_x,
                               const double weight_term_const);
  virtual ~Bivariate_quadratic_function();
  const bool is_inside(const double x, const double y) const;
  const std::string to_string() const;
  const Bivariate_quadratic_function *create_d_dx() const;
  const Bivariate_quadratic_function *create_d_dy() const;
private:
  const double _weight_term_yy;
  const double _weight_term_xy;
  const double _weight_term_xx;
  const double _weight_term_y;
  const double _weight_term_x;
  const double _weight_term_const;
};

#endif /* BIVARIATE_QUADRATIC_FUNCTION_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
