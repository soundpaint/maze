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
#include <log.hh>

Brush_field::Brush_field(const uint16_t width,
                         const uint16_t height,
                         const std::vector<const IShape *> field) :
  _width(width),
  _height(height),
  _field(field)
{
}

Brush_field::~Brush_field()
{
}

const std::string
Brush_field::to_string() const
{
    std::stringstream str;
    str << "Brush_field{" <<
      "width=" << _width <<
      ", height=" << _height <<
      ", field={\r\n";
    for (const IShape *shape : _field) {
      str << "  " << shape->to_string() << "\r\n";
    }
    str << "}}";
    return std::string(str.str());
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
  const double field_offset_x = (pos_x - field_index_x - 0.5) * 2.0;
  const double field_offset_y = (pos_y - field_index_y - 0.5) * 2.0;
  if ((field_index_x < 0) || (field_index_x >= _width)) {
    Log::fatal("Brush_field::get_brush(): field index x out of range");
  }
  if ((field_index_y < 0) || (field_index_y >= _height)) {
    Log::fatal("Brush_field::get_brush(): field index y out of range");
  }
  if ((field_offset_x < -1.0) || (field_offset_x >= 1.0)) {
    Log::fatal("Brush_field::get_brush(): field offset x out of range");
  }
  if ((field_offset_y < -1.0) || (field_offset_y >= 1.0)) {
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
