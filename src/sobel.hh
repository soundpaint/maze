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

#ifndef SOBEL_HH
#define SOBEL_HH

#include <inttypes.h>

class Sobel
{
public:
  Sobel(const uint16_t width, const uint16_t height);
  virtual ~Sobel();
  void compute_convolution(const double *m);
  const double get_gx(const uint16_t x, const uint16_t y) const;
  const double get_gy(const uint16_t x, const uint16_t y) const;
  const double get_edge_orientation(const uint16_t x, const uint16_t y) const;
  const double get_edge_magnitude(const uint16_t x, const uint16_t y) const;
private:
  static const uint8_t SIZE = 5;
  static const double NORM_SCALE;
  static const double GX[SIZE][SIZE];
  static const double GY[SIZE][SIZE];
  uint16_t _width;
  uint16_t _height;
  double *_gx, *_gy;
  static const double abs(const double x);
  const double convolute_with_kernel(const double *m,
                                     const uint32_t my0,
                                     const double *kernel) const;
};

#endif /* SOBEL_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
