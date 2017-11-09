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

Implicit_curve::Implicit_curve(const char *expression)
{
  _weight_term_yy = 0.0;
  _weight_term_xy = 0.0;
  _weight_term_xx = 0.0;
  _weight_term_x = 0.0;
  _weight_term_y = 0.0;
  _weight_term_const = 0.0;
  parse(expression);
}

Implicit_curve::~Implicit_curve()
{
  _weight_term_yy = 0.0;
  _weight_term_xy = 0.0;
  _weight_term_xx = 0.0;
  _weight_term_y = 0.0;
  _weight_term_x = 0.0;
  _weight_term_const = 0.0;
}

void
Implicit_curve::parse(const char *expression)
{
  // implicit_curve ::= weighted_term { add_op weighted_term } .
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
