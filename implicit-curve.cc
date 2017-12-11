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
#include <string.h>
#include <log.hh>

Implicit_curve::Implicit_curve(const double weight_term_yy,
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

Implicit_curve::~Implicit_curve()
{
}

const bool
Implicit_curve::is_inside(const double x, const double y) const
{
  return
    _weight_term_yy * y * y +
    _weight_term_xy * x * y +
    _weight_term_xx * x * x +
    _weight_term_y * y +
    _weight_term_x * x +
    _weight_term_const <= 0.0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
