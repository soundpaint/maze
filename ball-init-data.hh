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

#ifndef BALL_INIT_DATA_HH
#define BALL_INIT_DATA_HH

#include <inttypes.h>

class Ball_init_data
{
public:
  Ball_init_data(const uint16_t column,
                 const uint16_t row,
                 const double align_x = 0.5,
                 const double align_y = 0.5,
                 const double velocity_x = 0.0,
                 const double velocity_y = 0.0,
                 const double mass = 1.0);
  virtual ~Ball_init_data();
  const uint16_t get_column() const;
  const uint16_t get_row() const;
  const double get_align_x() const;
  const double get_align_y() const;
  const double get_velocity_x() const;
  const double get_velocity_y() const;
  const double get_mass() const;
private:
  const uint16_t _column;
  const uint16_t _row;
  const double _align_x;
  const double _align_y;
  const double _velocity_x;
  const double _velocity_y;
  const double _mass;
};

#endif /* BALL_INIT_DATA_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
