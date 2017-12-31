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

#ifndef FRACTALS_FACTORY_HH
#define FRACTALS_FACTORY_HH

#include <QtGui/QPixmap>
#include <ibrush-factory.hh>

class Fractals_factory : public IBrush_factory
{
public:
  static QPixmap * const create_mandelbrot_set(const uint16_t width,
                                               const uint16_t height,
                                               const double x0 = 0.0,
                                               const double y0 = 0.0,
                                               const double x_scale = 1.0,
                                               const double y_scale = 1.0);
  Fractals_factory(const double x0 = 0.0,
                   const double y0 = 0.0,
                   const double x_scale = 1.0,
                   const double y_scale = 1.0);
  virtual ~Fractals_factory();
  virtual QBrush create_brush(const uint16_t width, const uint16_t height);
private:
  const double _x0;
  const double _y0;
  const double _x_scale;
  const double _y_scale;
  QPixmap *_cached_pixmap;
  uint16_t _width_of_cached_pixmap;
  uint16_t _height_of_cached_pixmap;
};

#endif /* FRACTALS_FACTORY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
