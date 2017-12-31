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

#ifndef SENSORS_HH
#define SENSORS_HH

#include <ifield-geometry-listener.hh>

class Sensors : public IField_geometry_listener
{
public:
  Sensors();
  virtual ~Sensors();
  void sample_and_hold();
  const uint16_t get_pitch() const;
  const uint16_t get_roll() const;
  const uint16_t get_yaw() const;
  virtual void geometry_changed(const uint16_t width,
                                const uint16_t height);
private:
  uint16_t _field_width;
  uint16_t _field_height;
  uint16_t _pitch;
  uint16_t _roll;
  uint16_t _yaw;
};

#endif /* SENSORS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
