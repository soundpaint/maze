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

#ifndef IFRACTAL_SET_HH
#define IFRACTAL_SET_HH

#include <complex>

class IFractal_set
{
public:
  virtual bool assume_unconverged(const std::complex<double> z) const = 0;
  virtual const std::complex<double>
  next(const std::complex<double> z,
       const std::complex<double> pos) const = 0;
  virtual std::string *to_string() = 0;
public:
  virtual ~IFractal_set() {};
};

#endif /* IFRACTAL_SET_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
