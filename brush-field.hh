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

#ifndef BRUSH_FIELD_HH
#define BRUSH_FIELD_HH

#include <vector>
#include <string>
#include <inttypes.h>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <ifield-geometry-listener.hh>
#include <tile.hh>
#include <ball-init-data.hh>

class Brush_field : public IField_geometry_listener
{
public:
  Brush_field(const uint16_t columns,
              const uint16_t rows,
              const std::vector<Tile *> field,
              const std::vector<const Ball_init_data *> balls);
  virtual ~Brush_field();
  const std::string to_string() const;
  const uint16_t get_columns() const;
  const uint16_t get_rows() const;
  const QBrush *get_brush(const double x, const double y) const;
  const double get_potential(const double x, const double y) const;
  const double get_avg_tan(const double x0, const double y0,
                           const double dx, const double dy) const;
  const bool matches_goal(const double x, const double y) const;
  virtual void geometry_changed(const uint16_t columns, const uint16_t rows);
  const std::vector<const Ball_init_data *> get_balls_init_data() const;
private:
  const uint16_t _columns;
  const uint16_t _rows;
  const std::vector<Tile *> _field;
  const std::vector<const Ball_init_data *> _balls;
  const Tile *get_tile(const double x, const double y,
                       double * const tile_offset_x,
                       double * const tile_offset_y) const;
};

#endif /* BRUSH_FIELD_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
