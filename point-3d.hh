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

#ifndef POINT_3D_HH
#define POINT_3D_HH

class Point_3D
{
public:
  Point_3D(const double x, const double y, const double z);
  virtual ~Point_3D();
  static Point_3D *from_polar(const double phi, const double theta,
			      const double r);
  const double get_x() const;
  const double get_y() const;
  const double get_z() const;
  double *get_rx();
  double *get_ry();
  double *get_rz();
  const double get_length() const;
  const Point_3D *scale(const double s) const;
  const Point_3D *translate(const double x, const double y, const double z) const;
  const Point_3D *with_length(const double length) const;
private:
  double _x, _y, _z;
};

#endif /* POINT_3D_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
