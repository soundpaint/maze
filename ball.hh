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

#ifndef BALL_HH
#define BALL_HH

#include <inttypes.h>
#include <QtGui/QPixmap>
#include <sensors.hh>
#include <point-3d.hh>
#include <force-field.hh>

class Ball
{
public:
  Ball(const double px = 0.5, const double py = 0.5,
       const double vx = 0.0, const double vy = 0.0,
       const double mass = 1.0);
  virtual ~Ball();
  void update(const Sensors *sensors,
              const uint16_t width, const uint16_t height);
  const Point_3D *get_position() const;
  const Point_3D *get_velocity() const;
  const QPixmap *get_pixmap() const;
  const uint16_t get_pixmap_width() const;
  const uint16_t get_pixmap_height() const;
  const uint16_t get_pixmap_origin_x() const;
  const uint16_t get_pixmap_origin_y() const;
  const double get_potential(const uint16_t x, const uint16_t y);
  const bool get_is_in_goal() const;
  void set_is_in_goal(const bool is_in_goal);
  void precompute_forces(const Force_field *force_field);
  const double get_theta(const uint16_t x, const uint16_t y) const; // DEBUG
  const double is_reflection(const uint16_t x, const uint16_t y) const; // DEBUG
  const double is_exclusion_zone(const uint16_t x, const uint16_t y) const; // DEBUG
private:
  struct velocity_op_t {
    double m00, m10, m01, m11, theta;
    bool is_reflection;
    bool is_exclusion_zone;
  };
  static /*const*/ QPixmap *DEFAULT_PIXMAP;
  static const uint16_t DEFAULT_PIXMAP_WIDTH;
  static const uint16_t DEFAULT_PIXMAP_HEIGHT;
  static const uint16_t DEFAULT_PIXMAP_ORIGIN_X;
  static const uint16_t DEFAULT_PIXMAP_ORIGIN_Y;
  static const double abs(const double x);
  static /*const*/ QPixmap *create_default_pixmap();
  Point_3D *_position;
  Point_3D *_velocity;
  const double _mass;
  const QPixmap *_pixmap;
  bool _is_in_goal;
  double _max_vx = 0.0;
  double _max_vy = 0.0;

  const bool update_velocity(const struct velocity_op_t velocity_op,
                             Point_3D *velocity) const;
  void precompute_forces(const uint16_t x, const uint16_t y,
                         const Force_field *force_field);
  uint16_t _force_field_width;
  uint16_t _force_field_height;
  struct velocity_op_t *_op_force_field;
};

#endif /* BALL_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
