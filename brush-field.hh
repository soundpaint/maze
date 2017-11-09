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

#ifndef BRUSH_FIELD_HH
#define BRUSH_FIELD_HH

#include <inttypes.h>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <ishape.hh>

class Brush_field
{
public:
  Brush_field();
  virtual ~Brush_field();
  const uint16_t get_width() const;
  const uint16_t get_height() const;
  const QBrush *get_brush(const double x, const double y) const;
  const double get_potential(const double x, const double y) const;
  const double get_avg_tan(const double x0, const double y0,
			   const double dx, const double dy) const;
  const bool matches_goal(const double x, const double y) const;

private:
  QPixmap _corridor_pixmap, _wall_pixmap, _hole_pixmap;
  QBrush _corridor_brush, _wall_brush, _hole_brush;
  IShape *_c;
  IShape *_w;
  IShape *_h;
  IShape *_x;
  uint16_t _width;
  uint16_t _height;
  IShape **_field;
  const IShape *get_block(const double x, const double y,
			  double *block_offset_x,
			  double *block_offset_y) const;
};

#endif /* BRUSH_FIELD_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
