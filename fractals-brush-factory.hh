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

#ifndef FRACTALS_BRUSH_FACTORY_HH
#define FRACTALS_BRUSH_FACTORY_HH

#include <QtGui/QPixmap>
#include <ibrush-factory.hh>
#include <ifractal-set.hh>

class Fractals_brush_factory : public IBrush_factory
{
public:
  Fractals_brush_factory(const Xml_string *id,
                         const IFractal_set *fractal_set,
                         const uint16_t max_iterations = 256,
                         const double x0 = 0.0,
                         const double y0 = 0.0,
                         const double x_scale = 1.0,
                         const double y_scale = 1.0);
  virtual ~Fractals_brush_factory();
  const Xml_string *get_id() const;
  virtual QBrush create_brush(const uint16_t width, const uint16_t height);
  virtual std::string *to_string();
private:
  const Xml_string *_id;
  const IFractal_set *_fractal_set;
  const uint16_t _max_iterations;
  const double _x0;
  const double _y0;
  const double _x_scale;
  const double _y_scale;
  QPixmap *_cached_pixmap;
  uint16_t _cached_pixmap_width;
  uint16_t _cached_pixmap_height;
  static const QBrush count_to_brush(const uint16_t count,
                                     const uint16_t max_count);
  static QPixmap * const create_fractal_pixmap(const IFractal_set *fractal_set,
                                               const uint16_t max_iterations,
                                               const uint16_t width,
                                               const uint16_t height,
                                               const double x0,
                                               const double y0,
                                               const double x_scale,
                                               const double y_scale);
};

#endif /* FRACTALS_BRUSH_FACTORY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
