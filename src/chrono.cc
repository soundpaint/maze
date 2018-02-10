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

#include <chrono.hh>
#include <log.hh>

Chrono::Chrono(const std::string title)
{
  _title = title;
}

Chrono::~Chrono()
{
}

void
Chrono::start()
{
  clock_start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(clock_start);
  {
    std::stringstream str;
    str << "started computation of " << _title << " at " <<
      std::ctime(&start_time);
    Log::debug(str.str());
  }
}

void
Chrono::stop() const
{
  std::chrono::time_point<std::chrono::system_clock> clock_stop =
    std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = clock_stop - clock_start;
  std::time_t stop_time = std::chrono::system_clock::to_time_t(clock_stop);
  {
    std::stringstream str;
    str << "finished computation of " << _title << " at " <<
      std::ctime(&stop_time) << ", elapsed time: " <<
      elapsed_seconds.count() << "s";
    Log::debug(str.str());
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
