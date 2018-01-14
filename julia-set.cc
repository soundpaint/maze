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

#include <julia-set.hh>
#include <cmath>
#include <log.hh>

Julia_set::Julia_set(const uint16_t n, const std::complex<double> c) :
  _n(n),
  _c(c)
{
}

Julia_set::~Julia_set()
{
}

bool
Julia_set::assume_unconverged(const std::complex<double> z) const
{
  return std::norm(z) < 4.0;
}

const std::complex<double>
Julia_set::next(const std::complex<double> z,
                const std::complex<double> pos) const
{
  return pow(z, _n) + _c;
}

std::string *
Julia_set::to_string()
{
  std::stringstream str;
  str << "Julia_set{" <<
    "n=" << _n << ", " <<
    "c=(" << _c.real() << ", " << _c.imag() << "i)" <<
    "}";
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
