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

#include <shape.hh>
#include <log.hh>

Shape::Shape(const Shape_terms *shape_expression,
             const QBrush *foreground,
             const double foreground_potential,
             const QBrush *background,
             const double background_potential) :
  _shape_expression(shape_expression),
  _foreground(foreground),
  _foreground_potential(foreground_potential),
  _background(background),
  _background_potential(background_potential)
{
  if (!shape_expression) {
    Log::fatal("Shape::Shape(): shape_expression is null");
  }
  if (!foreground) {
    Log::fatal("Shape::Shape(): foreground is null");
  }
  if (!background) {
    Log::fatal("Shape::Shape(): background is null");
  }
}

Shape::~Shape()
{
}

const std::string
Shape::to_string() const
{
  std::stringstream str;
  str << "Shape{";
  // TODO
  str <<"}";
  return std::string(str.str());
}

const double
Shape::get_potential(const double x, const double y) const
{
  if (_shape_expression->is_inside(x, y)) {
    return _foreground_potential;
  } else {
    return _background_potential;
  }
}

const QBrush *
Shape::get_brush(const double x, const double y) const
{
  const QBrush *brush;
  if (_shape_expression->is_inside(x, y)) {
    brush = _foreground;
  } else {
    brush = _background;
  }
  return brush;
}

const double
Shape::get_avg_tan(const double block_offset_x,
                   const double block_offset_y,
                   const double dx,
                   const double dy) const
{
  // TODO
  return 0.0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
