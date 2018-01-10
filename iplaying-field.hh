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

#ifndef IPLAYING_FIELD_HH
#define IPLAYING_FIELD_HH

#include <inttypes.h>
#include <QtGui/QPixmap>
#include <point-3d.hh>
#include <ball.hh>

/*
 * The IPlaying_field interface helps avoiding an otherwise cyclic
 * inclusion dependency between balls.hh and playing-field.hh.
 */
class IPlaying_field
{
public:
  virtual const uint16_t get_width() const = 0;
  virtual const uint16_t get_height() const = 0;
  virtual void invalidate_rect(const double px, const double py,
                               const uint16_t pixmap_width,
                               const uint16_t pixmap_height,
                               const uint16_t pixmap_origin_x,
                               const uint16_t pixmap_origin_y) = 0;
  virtual const bool matches_goal(const double px, const double py) const = 0;
protected:
  ~IPlaying_field() {};
};

#endif /* IPLAYING_FIELD_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
