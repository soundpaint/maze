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

#include <point-3d.hh>
#include <cmath>
#include <cstring>
#include <log.hh>

Point_3D::Point_3D(const double x, const double y, const double z)
{
  _x = x;
  _y = y;
  _z = z;
}

Point_3D::~Point_3D()
{
}

Point_3D *
Point_3D::from_polar(const double phi, const double theta, const double r)
{
  const double x = cos(phi) * sin(theta);
  const double y = sin(phi) * sin(theta);
  const double z = cos(theta);
  Point_3D *result = new Point_3D(r * x, r * y, r * z);
  if (!result) {
    Log::fatal("Point_3D::from_polar(): not enough memory");
  }
  return result;
}

const double
Point_3D::get_x() const
{
  return _x;
}

const double
Point_3D::get_y() const
{
  return _y;
}

const double
Point_3D::get_z() const
{
  return _z;
}

double *
Point_3D::get_rx()
{
  return &_x;
}

double *
Point_3D::get_ry()
{
  return &_y;
}

double *
Point_3D::get_rz()
{
  return &_z;
}

const double
Point_3D::get_length() const
{
  return sqrt(_x * _x + _y * _y + _z * _z);
}

const Point_3D *
Point_3D::scale(const double s) const
{
  Point_3D *result = new Point_3D(_x * s, _y * s, _z * s);
  if (!result) {
    Log::fatal("Point_3D::scale(): not enough memory");
  }
  return result;
}

const Point_3D *
Point_3D::translate(const double x, const double y, const double z) const
{
  Point_3D *result = new Point_3D(_x + x, _y + y, _z + z);
  if (!result) {
    Log::fatal("Point_3D::translate(): not enough memory");
  }
  return result;
}

const Point_3D *
Point_3D::with_length(const double length) const
{
  const double current_length = get_length();
  if (current_length == 0.0) {
    Log::fatal("Point_3D::with_length(): "
               "zero point does not have any direction for scaling");
  }
  return scale(length / current_length);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
