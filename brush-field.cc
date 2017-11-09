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

#include <brush-field.hh>
#include <string>
#include <log.hh>
#include <square-shape.hh>
#include <rounded-square-shape.hh>
#include <tubes.hh>
#include <fractals-factory.hh>

Brush_field::Brush_field()
{
  // corridor
  /*
  _corridor_pixmap = QPixmap("cracked.png");
  _corridor_brush = QBrush(_corridor_pixmap);
  */
  /*
  const QPixmap *fractal =
    Fractals_factory::create_fractal(1280, 768,
				     -2.0, +1.0,
				     3.0 / 1280.0,
				     -2.0 / 768.0);
  */
  const QPixmap *fractal =
    Fractals_factory::create_fractal(1380, 768,
				     -1.45, +0.125,
				     0.25 / 1380.0,
				     -0.25 / 768.0);
  _corridor_brush = QBrush(*fractal);

  _c = new Square_shape(&_corridor_brush, 0.0);
  if (!_c) {
    Log::fatal("Brush_field::Brush_field(): not enough memory");
  }

  // wall
  _wall_pixmap = QPixmap("sm_squares.png");
  _wall_brush = QBrush(_wall_pixmap);
  _w =
    new Rounded_square_shape(1.0, 1.0, 1.0, 1.0,
			     &_wall_brush, 1.0,
			     &_corridor_brush, 0.0);
  if (!_w) {
    Log::fatal("Brush_field::Brush_field(): not enough memory");
  }

  // half-cross
  _x =
    new Tubes(true, true, true, false, true, true, true, false,
	      &_wall_brush, 1.0,
	      &_corridor_brush, 0.0);

  // hole
  _hole_pixmap = QPixmap("hole.png");
  _hole_brush = QBrush(_hole_pixmap);
  _h = new Square_shape(&_hole_brush, 0.0);
  if (!_h) {
    Log::fatal("Brush_field::Brush_field(): not enough memory");
  }

  const uint16_t HEIGHT = 8;
  const uint16_t WIDTH = 8;
  IShape * const FIELD[HEIGHT][WIDTH] = {
    {_w, _w, _w, _w, _w, _w, _w, _w},
    {_w, _c, _c, _c, _c, _c, _c, _w},
    {_w, _w, _w, _c, _w, _c, _c, _w},
    {_w, _c, _w, _c, _w, _w, _w, _w},
    {_w, _c, _c, _x, _c, _c, _c, _w},
    {_w, _c, _w, _w, _w, _w, _h, _w},
    {_w, _c, _c, _w, _c, _c, _c, _w},
    {_w, _w, _w, _w, _w, _w, _w, _w}
  };

  _width = WIDTH;
  _height = HEIGHT;
  _field = new IShape *[_width * _height];
  if (!_field) {
    Log::fatal("Brush_field::Brush_field(): not enough memory");
  }
  for (uint16_t y = 0; y < _height; y++) {
    for (uint16_t x = 0; x < _width; x++) {
      _field[y * _width + x] = FIELD[y][x];
    }
  }
}

Brush_field::~Brush_field()
{
  delete [] _field;
  _field = 0;
  _width = 0;
  _height = 0;
}

const uint16_t
Brush_field::get_width() const
{
  return _width;
}

const uint16_t
Brush_field::get_height() const
{
  return _height;
}

const IShape *
Brush_field::get_block(const double x, const double y,
		       double *block_offset_x,
		       double *block_offset_y) const
{
  if ((x < 0.0) || (x >= 1.0) || std::isnan(x) || std::isinf(x)) {
    std::stringstream msg;
    msg << "x=" << x;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_potential(): x out of range");
  }
  if ((y < 0.0) || (y >= 1.0) || std::isnan(y) || std::isinf(y)) {
    Log::fatal("Brush_field::get_potential(): y out of range");
  }
  const double pos_x = x * _width;
  const double pos_y = y * _height;
  const uint16_t block_index_x = (uint16_t)(pos_x);
  const uint16_t block_index_y = (uint16_t)(pos_y);
  const double __block_offset_x = pos_x - block_index_x;
  const double __block_offset_y = pos_y - block_index_y;
  if ((block_index_x < 0) || (block_index_x >= _width)) {
    std::stringstream msg;
    msg << "block_index_x=" << block_index_x;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_block(): block index x out of range");
  }
  if ((block_index_y < 0) || (block_index_y >= _height)) {
    Log::fatal("Brush_field::get_block(): block index y out of range");
  }
  if ((__block_offset_x < 0.0) || (__block_offset_x >= 1.0)) {
    Log::fatal("Brush_field::get_block(): block offset x out of range");
  }
  if ((__block_offset_y < 0.0) || (__block_offset_y >= 1.0)) {
    Log::fatal("Brush_field::get_block(): block offset y out of range");
  }
  const IShape *block = _field[block_index_y * _width + block_index_x];
  *block_offset_x = __block_offset_x;
  *block_offset_y = __block_offset_y;
  return block;
}

const double
Brush_field::get_potential(const double x, const double y) const
{
  double block_offset_x;
  double block_offset_y;
  const IShape *block = get_block(x, y, &block_offset_x, &block_offset_y);
  return block->get_potential(block_offset_x, block_offset_y);
}

const double
Brush_field::get_avg_tan(const double x0, const double y0,
			 const double dx, const double dy) const
{
  double block_offset_x;
  double block_offset_y;
  const IShape *block = get_block(x0, y0, &block_offset_x, &block_offset_y);
  return block->get_avg_tan(block_offset_x, block_offset_y,
			    dx, dy);
}

const bool
Brush_field::matches_goal(const double x, const double y) const
{
  // TODO: Consider geometry (width, height) of the ball.
  return get_potential(x, y) < 0.0;
}

const QBrush *
Brush_field::get_brush(const double x, const double y) const
{
  if ((x < 0.0) || (x >= 1.0) || std::isnan(x) || std::isinf(x)) {
    std::stringstream msg;
    msg << "x=" << x;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_brush(): x out of range");
  }
  if ((y < 0.0) || (y >= 1.0) || std::isnan(y) || std::isinf(y)) {
    std::stringstream msg;
    msg << "y=" << y;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_brush(): y out of range");
  }
  const double pos_x = x * _width;
  const double pos_y = y * _height;
  const uint16_t field_index_x = (uint16_t)(pos_x);
  const uint16_t field_index_y = (uint16_t)(pos_y);
  const double field_offset_x = pos_x - field_index_x;
  const double field_offset_y = pos_y - field_index_y;
  if ((field_index_x < 0) || (field_index_x >= _width)) {
    Log::fatal("Brush_field::get_brush(): field index x out of range");
  }
  if ((field_index_y < 0) || (field_index_y >= _height)) {
    Log::fatal("Brush_field::get_brush(): field index y out of range");
  }
  if ((field_offset_x < 0.0) || (field_offset_x >= 1.0)) {
    Log::fatal("Brush_field::get_brush(): field offset x out of range");
  }
  if ((field_offset_y < 0.0) || (field_offset_y >= 1.0)) {
    Log::fatal("Brush_field::get_brush(): field offset y out of range");
  }
  const IShape *block = _field[field_index_y * _width + field_index_x];
  return block->get_brush(field_offset_x, field_offset_y);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
