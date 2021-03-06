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

#include <brush-field.hh>
#include <cmath>
#include <log.hh>

Brush_field::Brush_field(const uint16_t columns,
                         const uint16_t rows,
                         const std::vector<Tile *> field,
                         std::vector<const Ball_init_data *> balls) :
  _columns(columns),
  _rows(rows),
  _field(field),
  _balls(balls),
  _tile_pixel_width(0.0),
  _tile_pixel_height(0.0)
{
}

Brush_field::~Brush_field()
{
}

#define EPSILON 0.0001

void
Brush_field::geometry_changed(const uint16_t width, const uint16_t height)
{
  _tile_pixel_width = width ? (1.0 + EPSILON) * _columns / width : 0.0;
  _tile_pixel_height = height ? (1.0 + EPSILON) * _rows / height : 0.0;
  {
    std::stringstream str;
    str << "brush field: geometry changed: width=" << width <<
      ", height=" << height <<
      ", tile_pixel_width=" << _tile_pixel_width <<
      ", tile_pixel_height=" << _tile_pixel_height;
    Log::debug(str.str());
  }
  for (Tile *tile : _field) {
    tile->geometry_changed(width, height);
  }
}

const std::string
Brush_field::to_string() const
{
  std::stringstream str;
  str << "Brush_field{" <<
    "columns=" << _columns <<
    ", rows=" << _rows <<
    ", field={" << std::endl;
  for (const Tile *tile : _field) {
    str << "  " << tile->to_string() << std::endl;
  }
  str << "}}";
  return std::string(str.str());
}

const uint16_t
Brush_field::get_columns() const
{
  return _columns;
}

const uint16_t
Brush_field::get_rows() const
{
  return _rows;
}

const Tile *
Brush_field::get_tile(const double x, const double y,
                      double * const tile_offset_x,
                      double * const tile_offset_y,
                      bool *left_border,
                      bool *top_border,
                      bool *right_border,
                      bool *bottom_border) const
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
  const double pos_x = x * _columns;
  const double pos_y = y * _rows;
  const uint16_t tile_index_x = (uint16_t)(pos_x);
  const uint16_t tile_index_y = (uint16_t)(pos_y);
  const double __tile_offset_x = pos_x - tile_index_x;
  const double __tile_offset_y = pos_y - tile_index_y;
  *left_border = __tile_offset_x < _tile_pixel_width;
  *top_border = __tile_offset_y < _tile_pixel_height;
  *right_border = __tile_offset_x > 1.0 - _tile_pixel_width;
  *bottom_border =  __tile_offset_y > 1.0 - _tile_pixel_height;
  if ((tile_index_x < 0) || (tile_index_x >= _columns)) {
    std::stringstream msg;
    msg << "tile_index_x=" << tile_index_x;
    Log::info(msg.str());
    Log::fatal("Brush_field::get_tile(): tile index x out of range");
  }
  if ((tile_index_y < 0) || (tile_index_y >= _rows)) {
    Log::fatal("Brush_field::get_tile(): tile index y out of range");
  }
  if ((__tile_offset_x < 0.0) || (__tile_offset_x >= 1.0)) {
    Log::fatal("Brush_field::get_tile(): tile offset x out of range");
  }
  if ((__tile_offset_y < 0.0) || (__tile_offset_y >= 1.0)) {
    Log::fatal("Brush_field::get_tile(): tile offset y out of range");
  }
  const uint16_t field_index = tile_index_y * _columns + tile_index_x;
  if ((field_index < 0) || (field_index >= _field.size())) {
    std::stringstream msg;
    msg << "Brush_field::get_tile(): field index out of range:" <<
      "index=" << field_index << ", field size=" << _field.size();
    Log::fatal(msg.str());
  }
  const Tile *tile = _field[field_index];
  *tile_offset_x = __tile_offset_x;
  *tile_offset_y = __tile_offset_y;
  return tile;
}

const QBrush *
Brush_field::get_brush(const double x, const double y) const
{
  double tile_offset_x;
  double tile_offset_y;
  bool _;
  const Tile *tile = get_tile(x, y, &tile_offset_x, &tile_offset_y,
                              &_, &_, &_, &_);
  return tile->get_brush(tile_offset_x, tile_offset_y);
}

const double
Brush_field::get_potential(const double x, const double y) const
{
  double tile_offset_x;
  double tile_offset_y;
  bool left_border;
  bool top_border;
  bool right_border;
  bool bottom_border;
  const Tile *tile = get_tile(x, y, &tile_offset_x, &tile_offset_y,
                              &left_border, &top_border,
                              &right_border, &bottom_border);

  // corner case: assume border of tiles (if potential is 1.0) is
  // straight line of reflection
  if (tile->get_potential(tile_offset_x, tile_offset_y) == 1.0) {
    if (left_border || bottom_border || right_border || top_border) {
      return 0.0;
    }
  }

  return tile->get_potential(tile_offset_x, tile_offset_y);
}

const double
Brush_field::get_avg_tan(const double x, const double y) const
{
  double tile_offset_x;
  double tile_offset_y;
  bool left_border;
  bool top_border;
  bool right_border;
  bool bottom_border;
  const Tile *tile = get_tile(x, y, &tile_offset_x, &tile_offset_y,
                              &left_border, &top_border,
                              &right_border, &bottom_border);

  // corner case: assume border of tiles (if potential is 1.0) is
  // straight line of reflection
  if (tile->get_potential(tile_offset_x, tile_offset_y) == 1.0) {
    if (left_border) {
      return 0.0;
    } else if (bottom_border) {
      return 0.5 * M_PI;
    } else if (right_border) {
      return M_PI;
    } else if (top_border) {
      return -0.5 * M_PI;
    }
  }

  // standard case
  return tile->get_avg_tan(tile_offset_x, tile_offset_y);
}

const bool
Brush_field::matches_goal(const double x, const double y) const
{
  // TODO: Consider geometry (width, height) of the ball.
  return get_potential(x, y) < 0.0;
}

const std::vector<const Ball_init_data *>
Brush_field::get_balls_init_data() const
{
  return _balls;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
