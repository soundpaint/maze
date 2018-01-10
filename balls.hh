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

#ifndef BALLS_HH
#define BALLS_HH

#include <vector>
#include <inttypes.h>
#include <sensors.hh>
#include <ball.hh>
#include <iplaying-field.hh>

class Balls
{
public:
  Balls(Sensors *sensors,
        const std::vector<const Ball_init_data *> balls_init_data,
        const uint16_t rows,
        const uint16_t columns);
  virtual ~Balls();
  void update(IPlaying_field *playing_field);
  const uint8_t get_count() const;
  Ball *at(const uint8_t index) const;
  const bool all_balls_in_goal() const;
  void set_oversampling(const uint16_t oversampling);
  const uint16_t get_oversampling();

private:
  static const uint16_t DEFAULT_OVERSAMPLING;
  Sensors *_sensors;
  std::vector<Ball *> *_balls;
  uint16_t _oversampling;
};

#endif /* BALLS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
