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

#ifndef SHAPE_HH
#define SHAPE_HH

#include <ishape.hh>
#include <shape-expression.hh>

class Shape : public IShape
{
public:
  Shape(const Shape_terms *shape_expression,
        const QBrush *foreground,
        const double foreground_potential,
        const QBrush *background,
        const double background_potential);
  virtual ~Shape();
  virtual const std::string to_string() const;
  virtual const double get_potential(const double x, const double y) const;
  virtual const QBrush *get_brush(const double x, const double y) const;
  virtual const double get_avg_tan(const double block_offset_x,
                                   const double block_offset_y,
                                   const double dx,
                                   const double dy) const;
private:
  const Shape_terms *_shape_expression;
  const QBrush *_foreground;
  const double _foreground_potential;
  const QBrush *_background;
  const double _background_potential;
};

#endif /* SHAPE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
