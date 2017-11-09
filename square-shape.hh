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

#ifndef SQUARE_SHAPE_HH
#define SQUARE_SHAPE_HH

#include <ishape.hh>

class Square_shape : public IShape
{
public:
  Square_shape(QBrush *brush, const double potential);
  virtual ~Square_shape();
  QBrush *get_brush(const double x, const double y) const;
  const double get_potential(const double x, const double y) const;
  const double get_avg_tan(const double block_offset_x,
			   const double block_offset_y,
			   const double dx,
			   const double dy) const;

private:
  QBrush *_brush;
  double _potential;
};

#endif /* SQUARE_SHAPE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
