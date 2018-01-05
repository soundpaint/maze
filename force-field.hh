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

#ifndef FORCE_FIELD_HH
#define FORCE_FIELD_HH

#include <inttypes.h>
#include <sobel.hh>
#include <point-3d.hh>
#include <brush-field.hh>

class Force_field
{
public:
  Force_field();
  virtual ~Force_field();
  void load_field(const Brush_field *brush_field,
                  const uint16_t width, const uint16_t height);
  const double get_theta(const uint16_t x, const uint16_t y) const;
  const bool is_reflection(const uint16_t x, const uint16_t y) const;
  const bool is_exclusion_zone(const uint16_t x, const uint16_t y) const;
  const uint16_t get_width() const;
  const uint16_t get_height() const;
private:
  uint16_t _width;
  uint16_t _height;
  struct velocity_op_t {
    double theta;
    bool is_reflection;
    bool is_exclusion_zone;
  };
  struct velocity_op_t *_op_field;
  double *create_potential_field(const Brush_field *brush_field) const;
  void load_field(const uint16_t x, const uint16_t y,
                  const Brush_field *brush_field,
                  const Sobel *sobel);
  void load_field(const uint16_t x, const uint16_t y,
                  const Brush_field *brush_field);
};

#endif /* FORCE_FIELD_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
