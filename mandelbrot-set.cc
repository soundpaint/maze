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

#include <mandelbrot-set.hh>
#include <log.hh>

Mandelbrot_set::Mandelbrot_set()
{
}

Mandelbrot_set::~Mandelbrot_set()
{
}

bool
Mandelbrot_set::assume_unconverged(const std::complex<double> z) const
{
  return std::norm(z) < 1000000.0;
}

const std::complex<double>
Mandelbrot_set::next(const std::complex<double> z,
                     const std::complex<double> pos) const
{
  return z * z + pos;
}

std::string *
Mandelbrot_set::to_string()
{
  std::stringstream str;
  str << "Mandelbrot_set{}";
  std::string *result = new std::string(str.str());
  if (!result) {
    Log::fatal("not enough memory");
  }
  return result;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
