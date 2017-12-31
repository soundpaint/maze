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

#include <tile.hh>
#include <stdlib.h>
#include <log.hh>

Tile::Tile(const Xml_string *id,
           IBrush_factory *foreground_brush_factory,
           IBrush_factory *background_brush_factory,
           const double foreground_potential,
           const double background_potential,
           const Shape *shape) :
  _id(id),
  _foreground_brush_factory(foreground_brush_factory),
  _background_brush_factory(background_brush_factory),
  _foreground_potential(foreground_potential),
  _background_potential(background_potential),
  _shape(shape),
  _width(0),
  _height(0)
{
  if (!id) {
    Log::fatal("id is null");
  }
  if (!shape) {
    Log::fatal("shape is null");
  }
}

Tile::~Tile()
{
  // The caller of this class's constructor
  // manages (de)allocation of brush factories.
  // Therefore, we just set it to 0.
  _foreground_brush_factory = 0;
  _background_brush_factory = 0;

  delete _id;
  _id = 0;

  delete _shape;
  _shape = 0;

  _width = 0;
  _height = 0;
}

const std::string
Tile::to_string() const
{
  std::stringstream str;
  str << "Tile{";
  str << "id=" << _id;
  str << ", foreground_brush_factory="
      << _foreground_brush_factory->to_string();
  str << ", background_brush_factory="
      << _background_brush_factory->to_string();
  str << ", width=" << &_width;
  str << ", height=" << &_height;
  str << ", foreground_potential=" << _foreground_potential;
  str << ", background_potential=" << _background_potential;
  str << ", shape=" << _shape->to_string();
  str <<"}";
  return std::string(str.str());
}

void
Tile::geometry_changed(const uint16_t width, const uint16_t height)
{
  if ((_width != width) || (_height != height)) {
    _width = width;
    _height = height;
    _foreground = _foreground_brush_factory->create_brush(width, height);
    _background = _background_brush_factory->create_brush(width, height);
  }
}

const double
Tile::get_potential(const double x, const double y) const
{
  if (_shape->is_inside(x, y)) {
    return _foreground_potential;
  } else {
    return _background_potential;
  }
}

const QBrush *
Tile::get_brush(const double x, const double y) const
{
  const QBrush *brush;
  if (_shape->is_inside(x, y)) {
    brush = &_foreground;
  } else {
    brush = &_background;
  }
return brush;
}

const double
Tile::get_avg_tan(const double tile_offset_x,
                  const double tile_offset_y,
                  const double dx,
                  const double dy) const
{
  // TODO
  return 0.0;
}

const Xml_string *
Tile::get_id() const
{
  return _id;
}

const Shape *
Tile::get_shape() const
{
  return _shape;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
