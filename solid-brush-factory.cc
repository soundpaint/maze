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

Solid_brush_factory::Solid_brush_factory(const QColor color) :
  _brush(QBrush(color))
{
}

Solid_brush_factory::~Solid_brush_factory()
{
}

QBrush
Solid_brush_factory::create_brush(const uint16_t width, const uint16_t height)
{
  return _brush;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
