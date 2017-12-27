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
                         const std::vector<const Tile *> field) :
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
      ", field={" << std::endl;
    for (const Tile *tile : _field) {
      str << "  " << tile->to_string() << std::endl;
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

const Tile *
Brush_field::get_tile(const double x, const double y,
                      double * const tile_offset_x,
                      double * const tile_offset_y) const
{
  if ((x < 0.0) || (x >= 1.0) || std::isnan(x) || std::isinf(x)) {
    std::stringstream msg;
    msg << "Brush_field::get_potential(): x out of range: ";
    msg << "x=" << x;
    Log::fatal(msg.str());
  }
  if ((y < 0.0) || (y >= 1.0) || std::isnan(y) || std::isinf(y)) {
    std::stringstream msg;
    msg << "Brush_field::get_potential(): y out of range: ";
    msg << "y=" << y;
    Log::fatal(msg.str());
  }
  const double pos_x = x * _width;
  const double pos_y = y * _height;
  const uint16_t tile_index_x = (uint16_t)(pos_x);
  const uint16_t tile_index_y = (uint16_t)(pos_y);
  const double __tile_offset_x = pos_x - tile_index_x;
  const double __tile_offset_y = pos_y - tile_index_y;
  if ((tile_index_x < 0) || (tile_index_x >= _width)) {
    std::stringstream msg;
    msg << "tile_index_x=" << tile_index_x;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_tile(): tile index x out of range");
  }
  if ((tile_index_y < 0) || (tile_index_y >= _height)) {
    Log::fatal("Brush_field::get_tile(): tile index y out of range");
  }
  if ((__tile_offset_x < 0.0) || (__tile_offset_x >= 1.0)) {
    Log::fatal("Brush_field::get_tile(): tile offset x out of range");
  }
  if ((__tile_offset_y < 0.0) || (__tile_offset_y >= 1.0)) {
    Log::fatal("Brush_field::get_tile(): tile offset y out of range");
  }
  const Tile *tile = _field[tile_index_y * _width + tile_index_x];
  *tile_offset_x = __tile_offset_x;
  *tile_offset_y = __tile_offset_y;
  return tile;
}

const QBrush *
Brush_field::get_brush(const double x, const double y) const
{
  double tile_offset_x;
  double tile_offset_y;
  const Tile *tile = get_tile(x, y, &tile_offset_x, &tile_offset_y);
  return tile->get_brush(tile_offset_x, tile_offset_y);
}

const double
Brush_field::get_potential(const double x, const double y) const
{
  double tile_offset_x;
  double tile_offset_y;
  const Tile *tile = get_tile(x, y, &tile_offset_x, &tile_offset_y);
  return tile->get_potential(tile_offset_x, tile_offset_y);
}

const double
Brush_field::get_avg_tan(const double x0, const double y0,
                         const double dx, const double dy) const
{
  double tile_offset_x;
  double tile_offset_y;
  const Tile *tile = get_tile(x0, y0, &tile_offset_x, &tile_offset_y);
  return tile->get_avg_tan(tile_offset_x, tile_offset_y, dx, dy);
}

const bool
Brush_field::matches_goal(const double x, const double y) const
{
  // TODO: Consider geometry (width, height) of the ball.
  return get_potential(x, y) < 0.0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
