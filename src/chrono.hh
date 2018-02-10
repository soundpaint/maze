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

#ifndef CHRONO_HH
#define CHRONO_HH

#include <chrono>
#include <string>

class Chrono
{
public:
  Chrono(const std::string title);
  ~Chrono();
  void start();
  void stop() const;
private:
  std::string _title;
  std::chrono::time_point<std::chrono::system_clock> clock_start;
};

#endif /* CHRONO_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
