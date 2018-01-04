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

#include <ball-init-data.hh>
#include <log.hh>

Ball_init_data::Ball_init_data(const uint16_t column,
                               const uint16_t row,
                               const double align_x,
                               const double align_y,
                               const double velocity_x,
                               const double velocity_y,
                               const double mass) :
  _column(column),
  _row(row),
  _align_x(align_x),
  _align_y(align_y),
  _velocity_x(velocity_x),
  _velocity_y(velocity_y),
  _mass(mass)
{
}

Ball_init_data::~Ball_init_data()
{
}

const uint16_t
Ball_init_data::get_column() const
{
  return _column;
}

const uint16_t
Ball_init_data::get_row() const
{
  return _row;
}

const double
Ball_init_data::get_align_x() const
{
  return _align_x;
}

const double
Ball_init_data::get_align_y() const
{
  return _align_y;
}

const double
Ball_init_data::get_velocity_x() const
{
  return _velocity_x;
}

const double
Ball_init_data::get_velocity_y() const
{
  return _velocity_y;
}

const double
Ball_init_data::get_mass() const
{
  return _mass;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
