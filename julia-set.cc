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

Julia_set::Julia_set(const uint16_t n, const complex_t c) :
  _n(n),
  _c(c)
{
}

Julia_set::~Julia_set()
{
}

bool
Julia_set::assume_unconverged(const complex_t z) const
{
#if USE_STD_COMPLEX
  return std::norm(z) < 4.0;
#else
  return z.norm() < 4.0;
#endif
}

const Julia_set::complex_t
Julia_set::next(const complex_t z, const complex_t pos) const
{
#if USE_STD_COMPLEX
  return pow(z, _n) + _c;
#else
  const double x0 = z.real();
  const double y0 = z.imag();
  const double cx = _c.real();
  const double cy = _c.imag();
  const double len = pow(x0*x0 + y0*y0, _n / 2);
  const double arg = _n * atan2(y0, x0);
  const double x1 = len * cos(arg) + cx;
  const double y1 = len * sin(arg) + cy;
  return {x1, y1};
#endif
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
