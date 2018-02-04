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

#ifndef JULIA_SET_HH
#define JULIA_SET_HH

#include <ifractal-set.hh>

class Julia_set : public IFractal_set
{
public:
  Julia_set(const uint16_t n, const complex_t c);
  virtual ~Julia_set();
  virtual bool assume_unconverged(const complex_t z) const;
  virtual const complex_t next(const complex_t z, const complex_t pos) const;
  virtual std::string *to_string();
private:
  const uint16_t _n;
  const complex_t _c;
};

#endif /* JULIA_SET_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
