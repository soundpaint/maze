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

#include <fractals-brush-factory.hh>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <log.hh>

Fractals_brush_factory::Fractals_brush_factory(const Xml_string *id,
                                               const double x0,
                                               const double y0,
                                               const double x_scale,
                                               const double y_scale) :
  _id(id),
  _x0(x0),
  _y0(y0),
  _x_scale(x_scale),
  _y_scale(y_scale)
{
  _cached_pixmap = 0;
  _width_of_cached_pixmap = 0;
  _height_of_cached_pixmap = 0;
}

Fractals_brush_factory::~Fractals_brush_factory()
{
  if (_id) {
    delete _id;
    _id = 0;
  }
  if (_cached_pixmap) {
    delete _cached_pixmap;
    _cached_pixmap = 0;
  }
  _width_of_cached_pixmap = 0;
  _height_of_cached_pixmap = 0;
}

const Xml_string *
Fractals_brush_factory::get_id() const
{
  return _id;
}

QBrush
Fractals_brush_factory::create_brush(const uint16_t width,
                                     const uint16_t height)
{
  if ((width != _width_of_cached_pixmap) ||
      (height != _height_of_cached_pixmap) ||
      (!_cached_pixmap)) {
    if (_cached_pixmap) {
      delete _cached_pixmap;
      _cached_pixmap = 0;
    }
    _cached_pixmap = create_mandelbrot_set(width, height,
                                           _x0, _y0,
                                           _x_scale, _y_scale);
    _width_of_cached_pixmap = width;
    _height_of_cached_pixmap = height;
  }
  QBrush result(*_cached_pixmap);
  return result;
}

QPixmap * const
Fractals_brush_factory::create_mandelbrot_set(const uint16_t width,
                                              const uint16_t height,
                                              const double x0,
                                              const double y0,
                                              const double x_scale,
                                              const double y_scale)
{
  QPixmap * const pixmap = new QPixmap(width, height);
  if (!pixmap) {
    Log::fatal("not enough memory");
  }
  if ((width == 0) || (height == 0))
    return pixmap;
  QPainter painter(pixmap);
  const double scaled_x_scale = x_scale / width;
  const double scaled_y_scale = y_scale / height;
  for (uint16_t y = 0; y < height; y++) {
    const double img = y0 + y * scaled_y_scale;
    for (uint16_t x = 0; x < width; x++) {
      const double real = x0 + x * scaled_x_scale;
      double z_real = 0.0;
      double z_img = 0.0;
      uint16_t count = 0;
      while ((count < 256) &&
	     ((z_real * z_real) + (z_img * z_img) < 1000000.0)) {
	const double z_real_new = z_real * z_real - z_img * z_img + real;
	const double z_img_new = 2.0 * z_real * z_img + img;
	z_real = z_real_new;
	z_img = z_img_new;
	count++;
      }
      QColor color;
      if (count < 256) {
	color = QColor::fromHsv((0x120 - count) & 0xff, 0x5f, 0x5f);
      } else {
	color = QColor(0x5f, 0x5f, 0x5f);
      }
      const QBrush brush = QBrush(color);
      painter.fillRect(x, y, 1, 1, brush);
    }
  }
  return pixmap;
}

std::string
Fractals_brush_factory::to_string()
{
  std::stringstream str;
  str << "Fractals_brush_factory{" <<
    "id=" << _id <<
    ", x0=" << _x0 << ", y0=" << _y0 <<
    ", x_scale=" << _x_scale << ", y_scale=" << _y_scale <<
    "}";
  return std::string(str.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
