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

#include <square-shape.hh>
#include <log.hh>

Square_shape::Square_shape(QBrush *brush, const double potential)
{
  if (!brush) {
    Log::fatal("Square_shape::Square_shape(): brush is null");
  }
  _brush = brush;
  _potential = potential;
}

Square_shape::~Square_shape()
{
  _potential = 0.0;
}

QBrush *
Square_shape::get_brush(const double x, const double y) const
{
  if ((x < 0.0) || (x > 1.0)) {
    Log::fatal("Square_shape::get_brush(): x out of range");
  }
  if ((y < 0.0) || (y > 1.0)) {
    Log::fatal("Square_shape::get_brush(): y out of range");
  }
  return _brush;
}

const double
Square_shape::get_potential(const double x, const double y) const
{
  if ((x < 0.0) || (x > 1.0)) {
    Log::fatal("Square_shape::get_potential(): x out of range");
  }
  if ((y < 0.0) || (y > 1.0)) {
    Log::fatal("Square_shape::get_potential(): y out of range");
  }
  return _potential;
}

const double
Square_shape::get_avg_tan(const double block_offset_x,
			  const double block_offset_y,
			  const double dx,
			  const double dy) const
{
  // TODO
  return 0.0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
