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

#include <pixmap-brush-factory.hh>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <log.hh>

const QBrush
Pixmap_brush_factory::create_brush(const char *file_path)
{
  const QPixmap pixmap(file_path);
  return QBrush(pixmap);
}

Pixmap_brush_factory::Pixmap_brush_factory(const char *file_path) :
  _brush(create_brush(file_path))
{
}

Pixmap_brush_factory::~Pixmap_brush_factory()
{
}

QBrush
Pixmap_brush_factory::create_brush(const uint16_t width, const uint16_t height)
{
  return _brush;
}

std::string
Pixmap_brush_factory::to_string()
{
  QPixmap pixmap = _brush.texture();
  std::stringstream str;
  str << "Pixmap_brush_factory{" <<
    "width=" << pixmap.width() <<
    ", height=" << pixmap.height() <<
    "}";
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
