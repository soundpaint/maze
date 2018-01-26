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

#include <cmath>
#include <force-field.hh>
#include <log.hh>

#define USE_IMPLICIT_CURVES 1

Force_field::Force_field()
{
  _width = 0;
  _height = 0;
  _op_field = 0;
}

Force_field::~Force_field()
{
  _width = 0;
  _height = 0;
  if (_op_field) {
    delete _op_field;
  }
  _op_field = 0;
}

double *
Force_field::create_potential_field(const Brush_field *brush_field) const
{
  double *potential_field = (double *)calloc(_width * _height, sizeof(double));
  if (!potential_field) {
    Log::fatal("Force_field::create_potential_field(): not enough memory");
  }
  for (uint16_t y = 0; y < _height; y++) {
    const double field_y = ((double)y) / _height;
    for (uint16_t x = 0; x < _width; x++) {
      const double field_x = ((double)x) / _width;
      potential_field[y * _width + x] =
        brush_field->get_potential(field_x, field_y);
    }
  }
  return potential_field;
}

void
Force_field::load_field_border()
{
  const double theta_right = 0.0;
  const double theta_down = 0.5 * M_PI;
  const double theta_left = M_PI;
  const double theta_up = -0.5 * M_PI;
  for (uint16_t x = 0; x < _width; x++) {
    _op_field[x].theta = theta_up;
    _op_field[_height * _width - _width + x].theta = theta_down;
    _op_field[x].is_reflection = true;
    _op_field[_height * _width - _width + x].is_reflection = true;
    _op_field[x].is_exclusion_zone = true;
    _op_field[_height * _width - _width + x].is_exclusion_zone = true;
  }
  for (uint16_t y = 0; y < _height; y++) {
    _op_field[y * _width].theta = theta_left;
    _op_field[y * _width + _width - 1].theta = theta_right;
    _op_field[y * _width].is_reflection = true;
    _op_field[y * _width + _width - 1].is_reflection = true;
    _op_field[y * _width].is_exclusion_zone = true;
    _op_field[y * _width + _width - 1].is_exclusion_zone = true;
  }
}

void
Force_field::load_field(const uint16_t x, const uint16_t y,
                        const Brush_field *brush_field,
                        const Sobel *sobel)
{
  struct velocity_op_t velocity_op;

  // consider that result of sobel convolution has 2 pixels less of
  // field width and height
  if ((x > 0) && (x < _width - 1) && (y > 0) && (y < _height - 1)) {
    const double r = sobel->get_edge_magnitude(x, y);
    if ((r < 0.0) || (r > 1.0)) {
      std::stringstream msg;
      msg << "r=" << r << std::endl;
      Log::info(msg.str());
      Log::fatal("Force_field::load_field(): edge magnitude out of range");
    }
    const double theta = sobel->get_edge_orientation(x, y);
    if (!std::isnan(theta) && !std::isinf(theta)) {
      velocity_op.theta = theta;
      velocity_op.is_reflection = true;
    } else {
      // sobel did not find any orientation to determine
      // => no reflection
      velocity_op.is_reflection = false;
    }
  } else {
    // treat remaining pixels just as non-reflecting pixels
    velocity_op.is_reflection = false;
  }
  velocity_op.is_exclusion_zone =
    brush_field->get_potential(1.0 * x / _width, 1.0 * y / _height) >= 1.0;
  _op_field[y * _width + x] = velocity_op;
}

const bool
Force_field::is_exclusion_zone(const double x, const double y,
                               const Brush_field *brush_field) const
{
  return brush_field->get_potential(x, y) == 1.0;
}

void
Force_field::load_field(const uint16_t x, const uint16_t y,
                        const Brush_field *brush_field)
{
  struct velocity_op_t velocity_op;
  const double i_width = 1.0 / _width;
  const double i_height = 1.0 / _height;
  velocity_op.is_exclusion_zone =
    is_exclusion_zone(x * i_width, y * i_height, brush_field);
  if (velocity_op.is_exclusion_zone) {
    const bool p0 =
      is_exclusion_zone((x - 1) * i_width, (y - 1) * i_height, brush_field);
    const bool p1 =
      is_exclusion_zone((x + 0) * i_width, (y - 1) * i_height, brush_field);
    const bool p2 =
      is_exclusion_zone((x + 1) * i_width, (y - 1) * i_height, brush_field);
    const bool p3 =
      is_exclusion_zone((x - 1) * i_width, (y + 0) * i_height, brush_field);
    const bool p4 =
      is_exclusion_zone((x + 1) * i_width, (y + 0) * i_height, brush_field);
    const bool p5 =
      is_exclusion_zone((x - 1) * i_width, (y + 1) * i_height, brush_field);
    const bool p6 =
      is_exclusion_zone((x + 0) * i_width, (y + 1) * i_height, brush_field);
    const bool p7 =
      is_exclusion_zone((x + 1) * i_width, (y + 1) * i_height, brush_field);
    const bool have_neighbour_in_exclusion_zone =
      p0 || p1 || p2 || p3 || p4 || p5 || p6 || p7;
    if (have_neighbour_in_exclusion_zone) {
      velocity_op.theta = brush_field->get_avg_tan(x * i_width, y * i_height);
      velocity_op.is_reflection = !std::isnan(velocity_op.theta);
    } else {
      velocity_op.is_reflection = false;
    }
  }
  _op_field[y * _width + x] = velocity_op;
}

void
Force_field::load_field(const Brush_field *brush_field,
                        const uint16_t width, const uint16_t height)
{
  if (width <= 0) {
    Log::fatal("Force_field::load_field(): width <= 0");
  }
  if (height <= 0) {
    Log::fatal("Force_field::load_field(): height <= 0");
  }
  _width = width;
  _height = height;

  if (!_op_field) {
    delete _op_field;
    _op_field = 0;
  }
  _op_field =
    (struct velocity_op_t *)calloc(_width * _height,
                                   sizeof(struct velocity_op_t));
  if (!_op_field) {
    Log::fatal("Force_field::load_field(): not enough memory");
  }

#if USE_IMPLICIT_CURVES // use implicit curves
  load_field_border();
  for (uint16_t x = 1; x < _width - 1; x++) {
    for (uint16_t y = 1; y < _height - 1; y++) {
      load_field(x, y, brush_field);
    }
  }
#else // use sobel
  Sobel *sobel = new Sobel(width, height);
  if (!sobel) {
    Log::fatal("Force_field::load_field(): not enough memory");
  }
  double *potential_field = create_potential_field(brush_field);
  sobel->compute_convolution(potential_field);
  free(potential_field);
  potential_field = 0;

  for (uint16_t x = 0; x < _width; x++) {
    for (uint16_t y = 0; y < _height; y++) {
      load_field(x, y, brush_field, sobel);
    }
  }

  delete sobel;
  sobel = 0;
#endif
}

const uint16_t
Force_field::get_width() const
{
  return _width;
}

const uint16_t
Force_field::get_height() const
{
  return _height;
}

const double
Force_field::get_theta(const uint16_t x, const uint16_t y) const
{
  if ((x >= _width) || (y >= _height)) {
    Log::fatal("Force_field::get_velocity00(): x or y out of range");
  }
  return _op_field[y * _width + x].theta;
}

const bool
Force_field::is_reflection(const uint16_t x, const uint16_t y) const
{
  if ((x >= _width) || (y >= _height)) {
    Log::fatal("Force_field::get_velocity10(): x or y out of range");
  }
  return _op_field[y * _width + x].is_reflection;
}

const bool
Force_field::is_exclusion_zone(const uint16_t x, const uint16_t y) const
{
  if ((x >= _width) || (y >= _height)) {
    Log::fatal("Force_field::get_velocity10(): x or y out of range");
  }
  return _op_field[y * _width + x].is_exclusion_zone;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
