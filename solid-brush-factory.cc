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

#include <solid-brush-factory.hh>
#include <log.hh>

Solid_brush_factory::Solid_brush_factory(const Xml_string *id,
                                         const QColor color) :
  _id(id),
  _brush(QBrush(color))
{
}

Solid_brush_factory::~Solid_brush_factory()
{
  if (_id) {
    delete _id;
    _id = 0;
  }
}

const Xml_string *
Solid_brush_factory::get_id() const
{
  return _id;
}

QBrush
Solid_brush_factory::create_brush(const uint16_t width, const uint16_t height)
{
  return _brush;
}

std::string
Solid_brush_factory::to_string()
{
  std::stringstream str;
  str << "Solid_brush_factory{" <<
    "id=" << _id <<
    ", color=" << _brush.color().name().toStdString() <<
    "}";
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
