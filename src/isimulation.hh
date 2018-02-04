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

#ifndef ISIMULATION_HH
#define ISIMULATION_HH

class ISimulation
{
public:
  virtual void begin() = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual void stop() = 0;
  virtual void set_speed(const double speed) = 0;
  virtual const bool is_velocity_visible() const = 0;
  virtual void set_velocity_visible(const bool velocity_visible) = 0;
  virtual const bool is_force_field_visible() const = 0;
  virtual void set_force_field_visible(const bool force_field_visible) = 0;
  virtual const bool is_ball_visible() const = 0;
  virtual void set_ball_visible(const bool ball_visible) = 0;
  virtual const bool is_running() = 0;
  virtual const bool is_pausing() = 0;
};

#endif /* ISIMULATION_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
