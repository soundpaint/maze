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

#include <fractals-brush-factory.hh>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <log.hh>
#include <julia-set.hh>
#include <mandelbrot-set.hh>
#include <chrono.hh>

#define LINEAR_COLOR_SHADE 0

Fractals_brush_factory::Fractals_brush_factory(const Xml_string *id,
                                               const IFractal_set *fractal_set,
                                               const uint16_t max_iterations,
                                               const double x0,
                                               const double y0,
                                               const double x_scale,
                                               const double y_scale) :
  _id(id),
  _fractal_set(fractal_set),
  _max_iterations(max_iterations),
  _x0(x0),
  _y0(y0),
  _x_scale(x_scale),
  _y_scale(y_scale)
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  if (!fractal_set) {
    Log::fatal("unexpected null fractal_set");
  }
  _cached_pixmap = 0;
  _cached_pixmap_width = 0;
  _cached_pixmap_height = 0;
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
  delete _fractal_set;
  _fractal_set = 0;
  _cached_pixmap_width = 0;
  _cached_pixmap_height = 0;
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
  if ((width != _cached_pixmap_width) ||
      (height != _cached_pixmap_height) ||
      (!_cached_pixmap)) {
    if (_cached_pixmap) {
      delete _cached_pixmap;
      _cached_pixmap = 0;
    }
    _cached_pixmap =
      create_fractal_pixmap(_fractal_set,
                            _max_iterations,
                            width, height,
                            _x0, _y0,
                            _x_scale, _y_scale);
    _cached_pixmap_width = width;
    _cached_pixmap_height = height;
  }
  QBrush result(*_cached_pixmap);
  return result;
}

const QBrush
Fractals_brush_factory::count_to_brush(const uint16_t count,
                                       const uint16_t max_count)
{
  // TODO: Add support for explicit configuration of color palette
  // configuration and coloring algorithm.
  const uint8_t saturation = 0x9f;
  const uint8_t brightness = 0x7f;
  const uint16_t start_hue = 0x120;
  const uint16_t max_color_index = 256;
  const QColor stop_color(0x5f, 0x5f, 0x5f);
#if LINEAR_COLOR_SHADE // linear color shade
  const uint16_t color_index = (max_color_index * count) / max_count;
#else // logarithmic color shade
  const uint16_t color_index =
    (uint16_t)(max_color_index * log(count) / log(max_count));
#endif
  QColor color;
  if (count < max_count) {
    color = QColor::fromHsv((start_hue - color_index) & 0xff,
                            saturation, brightness);
  } else {
    color = stop_color;
  }
  const QBrush brush = QBrush(color);
  return brush;
}

QPixmap * const
Fractals_brush_factory::create_fractal_pixmap(const IFractal_set *fractal_set,
                                              const uint16_t max_iterations,
                                              const uint16_t width,
                                              const uint16_t height,
                                              const double x0,
                                              const double y0,
                                              const double x_scale,
                                              const double y_scale)
{
  Chrono chrono("fractal");
  chrono.start();
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
    const double imag = y0 + y * scaled_y_scale;
    for (uint16_t x = 0; x < width; x++) {
      const double real = x0 + x * scaled_x_scale;
      IFractal_set::complex_t pos = {real, imag};
      IFractal_set::complex_t z = pos;
      uint16_t iteration_index = 0;
      while ((iteration_index < max_iterations) &&
             fractal_set->assume_unconverged(z)) {
        z = fractal_set->next(z, pos);
        iteration_index++;
      }
      const QBrush brush = count_to_brush(iteration_index, max_iterations);
      painter.fillRect(x, y, 1, 1, brush);
      // TODO: Maybe faster:
      //   painter.setBrush(brush);
      //   painter.drawPoint(x, y);
    }
  }
  chrono.stop();
  return pixmap;
}

std::string *
Fractals_brush_factory::to_string()
{
  std::stringstream str;
  str << "Fractals_brush_factory{" <<
    "id=" << _id <<
    ", x0=" << _x0 << ", y0=" << _y0 <<
    ", x_scale=" << _x_scale << ", y_scale=" << _y_scale <<
    "}";
  std::string *result = new std::string(str.str());
  if (!result) {
    Log::fatal("not enough memory");
  }
  return result;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
