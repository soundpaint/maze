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
Mandelbrot_set::assume_unconverged(const complex_t z) const
{
#if USE_STD_COMPLEX
  return std::norm(z) < 1000000.0;
#else
  return z.norm() < 1000000.0;
#endif
}

const Mandelbrot_set::complex_t
Mandelbrot_set::next(const complex_t z, const complex_t pos) const
{
#if USE_STD_COMPLEX
  return z * z + pos;
#else
  const double z_real = z.real();
  const double z_imag = z.imag();
  const double pos_real = pos.real();
  const double pos_imag = pos.imag();
  const double next_real = z_real * z_real - z_imag * z_imag + pos_real;
  const double next_imag = 2.0 * z_real * z_imag + pos_imag;
  return {next_real, next_imag};
#endif
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
