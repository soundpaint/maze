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

#include <cmath>
#include <sobel.hh>
#include <log.hh>

const double
Sobel::NORM_SCALE = 0.008;

const double
Sobel::GX[SIZE][SIZE] = {
  { -5.0,  -4.0, 0.0,  +4.0,  +5.0},
  { -8.0, -10.0, 0.0, -10.0,  +8.0},
  {-10.0, -20.0, 0.0, +20.0, +10.0},
  { -8.0, -10.0, 0.0, -10.0,  +8.0},
  { -5.0,  -4.0, 0.0,  +4.0,  +5.0},
};

const double
Sobel::GY[SIZE][SIZE] = {
  {+5.0,  +8.0, +10.0,  +8.0, +5.0},
  {+4.0, +10.0, +20.0, +10.0, +4.0},
  { 0.0,   0.0,   0.0,   0.0,  0.0},
  {-4.0, -10.0, -20.0, -10.0, -4.0},
  {-5.0,  -8.0, -10.0,  -8.0, -5.0},
};

Sobel::Sobel(const uint16_t width, const uint16_t height)
{
  if (width < SIZE) {
    std::stringstream msg;
    msg << "Sobel::Sobel(): minimum input array width is " << SIZE;
    Log::fatal(msg.str());
  }
  _width = width;
  if (height < SIZE) {
    std::stringstream msg;
    msg << "Sobel::Sobel(): minimum input array height is " << SIZE;
    Log::fatal(msg.str());
  }
  _height = height;
  _gx = (double *)calloc(width * height, sizeof(double));
  if (!_gx) {
    Log::fatal("Sobel::Sobel(): not enough memory");
  }
  _gy = (double *)calloc(width * height, sizeof(double));
  if (!_gy) {
    Log::fatal("Sobel::Sobel(): not enough memory");
  }
}

Sobel::~Sobel()
{
  _width = 0;
  _height = 0;
  free(_gx);
  _gx = 0;
  free(_gy);
  _gy = 0;
}

const double
Sobel::convolute_with_kernel(const double *matrix,
                             const uint32_t my0,
                             const double *kernel) const
{
  double sum = 0.0;
  for (uint8_t y = 0; y < SIZE; y++) {
    const uint32_t mx0 = my0 + y * _width;
    const uint8_t gy0 = y * SIZE;
    for (uint8_t x = 0; x < SIZE; x++) {
      sum += matrix[mx0 + x] * kernel[gy0 + x];
    }
  }
  return NORM_SCALE * sum;
}

void
Sobel::compute_convolution(const double *matrix)
{
  const uint8_t padding_size = (SIZE - 1) / 2;
  for (uint16_t y = 0; y < padding_size; y++) {
    const uint32_t yb = (_height - padding_size) * _width;
    for (uint16_t x = 0; x < _width; x++) {
      _gx[x] = 0.0;
      _gy[x] = 0.0;
      _gx[yb + x] = 0.0;
      _gy[yb + x] = 0.0;
    }
  }
  for (uint16_t y = padding_size; y < _height - padding_size; y++) {
    const int32_t m0 = (y - padding_size) * _width - padding_size;
    const uint32_t g0 = y * _width;
    const uint32_t gr = g0 + _width - padding_size;
    for (uint16_t x = 0; x < padding_size; x++) {
      _gx[g0 + x] = 0.0;
      _gy[g0 + x] = 0.0;
      _gx[gr + x] = 0.0;
      _gy[gr + x] = 0.0;
    }
    for (uint16_t x = padding_size; x < _width - padding_size; x++) {
      _gx[g0 + x] = convolute_with_kernel(matrix, m0 + x, GX[0]);
      _gy[g0 + x] = convolute_with_kernel(matrix, m0 + x, GY[0]);
    }
  }
}

const double
Sobel::get_gx(const uint16_t x, const uint16_t y) const
{
  if ((x < 0) || (x >= _width)) {
    Log::fatal("Sobel::get_gx(): x out of range");
  }
  if ((y < 0) || (y >= _height)) {
    Log::fatal("Sobel::get_gx(): y out of range");
  }
  return _gx[y * _width + x];
}

const double
Sobel::get_gy(const uint16_t x, const uint16_t y) const
{
  if ((x < 0) || (x >= _width)) {
    Log::fatal("Sobel::get_gx(): x out of range");
  }
  if ((y < 0) || (y >= _height)) {
    Log::fatal("Sobel::get_gx(): y out of range");
  }
  return _gy[y * _width + x];
}

const double
Sobel::abs(const double x)
{
  return x >= 0 ? x : -x;
}

const double
Sobel::get_edge_magnitude(const uint16_t x, const uint16_t y) const
{
  const double gx = get_gx(x, y);
  const double gy = get_gy(x, y);
  return sqrt(gx * gx + gy * gy);
}

const double
Sobel::get_edge_orientation(const uint16_t x, const uint16_t y) const
{
  const double gx = _width * get_gx(x, y);
  const double gy = _height * get_gy(x, y);
  double theta;
  if ((gx == 0.0) && (gy == 0.0)) {
    theta = nan("");
  } else {
    theta = atan2(gy, gx);
  }
  return theta;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
