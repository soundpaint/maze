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

#include <ball.hh>
#include <log.hh>

#define HAVE_SENSE_HAT 0

// TODO: Deploy z coordinates to consider position energy.
// -jr 2016-02-01

/*const*/ QPixmap *
Ball::DEFAULT_PIXMAP = 0;

const uint16_t
Ball::DEFAULT_PIXMAP_WIDTH = 16;

const uint16_t
Ball::DEFAULT_PIXMAP_HEIGHT = 16;

const uint16_t
Ball::DEFAULT_PIXMAP_ORIGIN_X = 7;

const uint16_t
Ball::DEFAULT_PIXMAP_ORIGIN_Y = 7;

Ball::Ball(const double px, const double py,
           const double vx, const double vy,
           const double mass) :
  _mass(mass)
{
  _position = new Point_3D(px, py, 0.0);
  if (!_position) {
    Log::fatal("Ball(): not enough memory");
  }
  _velocity = new Point_3D(vx, vy, 0.0);
  if (!_velocity) {
    Log::fatal("Ball(): not enough memory");
  }

  // Qt: Must construct a QGuiApplication before a QPixmap
  // => need lazy initialization of DEFAULT_PIXMAP.
  if (!DEFAULT_PIXMAP) {
    DEFAULT_PIXMAP = create_default_pixmap();
  }

  _pixmap = DEFAULT_PIXMAP;
  _is_in_goal = false;
  _max_vx = 0.0;
  _max_vy = 0.0;

  _op_force_field = 0;
  _force_field_width = 0;
  _force_field_height = 0;
}

Ball::~Ball()
{
  delete _position;
  _position = 0;
  delete _velocity;
  _velocity = 0;
  //_mass = 0.0;
  _is_in_goal = false;
  _max_vx = 0.0;
  _max_vy = 0.0;

  if (_op_force_field) {
    delete [] _op_force_field;
    _op_force_field = 0;
  }
  _force_field_width = 0;
  _force_field_height = 0;
}

const double MIN_X = 0.000;
const double MAX_X = 0.999;
const double MIN_Y = 0.000;
const double MAX_Y = 0.999;

/*const*/ QPixmap *
Ball::create_default_pixmap()
{
  /*const*/ QPixmap *pixmap = new QPixmap("ball.png");
  if (!pixmap) {
    Log::fatal("Ball::create_default_pixmap(): not enough memory");
  }
  return pixmap;
}

void
Ball::precompute_forces(const uint16_t x,
                        const uint16_t y,
                        const Force_field *force_field)
{
  const uint32_t ff0 = y * _force_field_width + x;
  struct velocity_op_t *ball_op = &_op_force_field[ff0];
  uint16_t reflection_count = 0;
  double theta = 0;
  int16_t ball_y = y - get_pixmap_origin_y();
  for (uint8_t pixmap_y = 0; pixmap_y < get_pixmap_height(); pixmap_y++) {
    if ((ball_y >= 0) && (ball_y < _force_field_height)) {
      int16_t ball_x = x - get_pixmap_origin_x();
      for (uint8_t pixmap_x = 0; pixmap_x < get_pixmap_width(); pixmap_x++) {
        if ((ball_x >= 0) && (ball_x < _force_field_width)) {
          if (get_potential(pixmap_x, pixmap_y) > 0.0) {
            //std::cout << "y";
            ball_op->is_exclusion_zone |=
              force_field->is_exclusion_zone(ball_x, ball_y);
            if (force_field->is_reflection(ball_x, ball_y)) {
              reflection_count++;
              theta += force_field->get_theta(ball_x, ball_y);
            }
          } else {
            //std::cout << "n";
          }
        }
        ball_x++;
      }
    }
    ball_y++;
  }
#if 1
  theta = force_field->get_theta(x, y);
  reflection_count = force_field->is_reflection(x, y) ? 1 : 0;
#endif
  if (reflection_count > 0) {
    theta /= (double)reflection_count;
    ball_op->theta = theta;
    ball_op->m00 = 1.0 * -cos(2.0 * theta);
    ball_op->m01 = 1.0 * +sin(2.0 * theta);
    ball_op->m10 = 1.0 * +sin(2.0 * theta);
    ball_op->m11 = 1.0 * +cos(2.0 * theta);
    ball_op->is_reflection = true;
  } else {
    ball_op->theta = 0.0;
    ball_op->m00 = 1.0;
    ball_op->m01 = 0.0;
    ball_op->m10 = 0.0;
    ball_op->m11 = 1.0;
    ball_op->is_reflection = false;
  }
}

void
Ball::precompute_forces(const Force_field *force_field)
{
  if (_op_force_field) {
    delete [] _op_force_field;
    _op_force_field = 0;
    _force_field_width = 0;
    _force_field_height = 0;
  }

  _force_field_width = force_field->get_width();
  _force_field_height = force_field->get_height();
  _op_force_field = (struct velocity_op_t *)
    calloc(_force_field_width * _force_field_height,
           sizeof(struct velocity_op_t));
  if (!_op_force_field) {
    Log::fatal("Force_field::load_field(): not enough memory");
  }

  for (uint16_t x = 0; x < _force_field_width; x++) {
    for (uint16_t y = 0; y < _force_field_height; y++) {
      precompute_forces(x, y, force_field);
    }
  }
}

const double
Ball::abs(const double x)
{
  return x >= 0 ? x : -x;
}

const bool
Ball::update_velocity(const struct velocity_op_t velocity_op,
                      Point_3D *velocity) const
{
  double *vx = velocity->get_rx();
  double *vy = velocity->get_ry();

  const double new_vx = velocity_op.m00 * (*vx) + velocity_op.m01 * (*vy);
  if ((new_vx < -1.0) || (new_vx > 1.0) ||
      std::isnan(new_vx) || std::isinf(new_vx)) {
    std::stringstream msg;
    msg << "new_vx=" << new_vx;
    Log::debug(msg.str());
    Log::fatal("Ball::update_velocity(): new_vx out of range");
  }

  const double new_vy = velocity_op.m10 * (*vx) + velocity_op.m11 * (*vy);
  if ((new_vy < -1.0) || (new_vy > 1.0) ||
      std::isnan(new_vy) || std::isinf(new_vy)) {
    std::stringstream msg;
    msg << "new_vy=" << new_vy;
    Log::debug(msg.str());
    Log::fatal("Ball::update_velocity(): new_vy out of range");
  }

  *vx = new_vx;
  *vy = new_vy;
  return velocity_op.is_reflection;
}

void
Ball::update(const Sensors *sensors,
             const uint16_t width, const uint16_t height)
{
  if (!sensors) {
    Log::fatal("Ball::update(): sensors is null");
  }

#if HAVE_SENSE_HAT
  double pitch = sensors->get_pitch();
  double roll = sensors->get_roll();
#else
  double pitch = 0.0;
  double roll = 0.0;
#endif

  double *px = _position->get_rx();
  double *py = _position->get_ry();
  double *vx = _velocity->get_rx();
  double *vy = _velocity->get_ry();

  double old_px = *px;
  double old_py = *py;
  uint16_t old_x = (uint16_t)(old_px * width);
  uint16_t old_y = (uint16_t)(old_py * height);

  *px += *vx;
  *py += *vy;

  if (*px < MIN_X) {
    Log::fatal("px < MIN");
  } else if (*px > MAX_X) {
    Log::fatal("px > MAX");
  }
  if (*py < MIN_Y) {
    Log::fatal("py < MIN");
  } else if (*py > MAX_Y) {
    Log::fatal("py > MAX");
  }

  double new_px = *px;
  double new_py = *py;
  uint16_t new_x = (uint16_t)(new_px * width);
  uint16_t new_y = (uint16_t)(new_py * height);

  const struct velocity_op_t velocity_op =
    _op_force_field[new_y * _force_field_width + new_x];

  if ((new_x != old_x) || (new_y || old_y)) {
    // new position in force field => test for collision
    if (!velocity_op.is_exclusion_zone) {
      if (update_velocity(velocity_op, _velocity)) {
        // collision => continue with previous position, but with
        // updated velocity

        // step back to the center of the previous pixel (but only in
        // one direction (px or py)) in order to avoid infinitive
        // loops due to rounding errors on the edge between adjacent
        // pixels.
        const double alpha = 0.45 + 0.1 * ((double)rand() / RAND_MAX);
        if (new_x != old_x) {
          *px = (old_x + alpha) / width;
        } else {
          *py = (old_y + alpha) / height;
        }
      }
    }
  }

  if (*px < MIN_X) {
    *px = MIN_X;
    *vx = +abs(*vx);
  } else if (*px > MAX_X) {
    *px = MAX_X;
    *vx = -abs(*vx);
  } else {
    *vx += 0.0000000003 * pitch;
    if (abs(*vx) > _max_vx) {
      _max_vx = abs(*vx);
      std::stringstream msg;
      msg << "_max_vx=" << _max_vx;
      Log::debug(msg.str());
    }
  }

  if (*py < MIN_Y) {
    *py = MIN_Y;
    *vy = +abs(*vy);
  } else if (*py > MAX_Y) {
    *py = MAX_Y;
    *vy = -abs(*vy);
  } else {
    *vy += 0.0000000002 * roll;
    if (abs(*vy) > _max_vy) {
      _max_vy = abs(*vy);
      std::stringstream msg;
      msg << "_max_vy=" << _max_vy;
      Log::debug(msg.str());
    }
  }
}

const Point_3D *
Ball::get_position() const
{
  return _position;
}

const Point_3D *
Ball::get_velocity() const
{
  return _velocity;
}

const QPixmap *
Ball::get_pixmap() const
{
  return _pixmap;
}

const uint16_t
Ball::get_pixmap_width() const
{
  return DEFAULT_PIXMAP_WIDTH;
}

const uint16_t
Ball::get_pixmap_height() const
{
  return DEFAULT_PIXMAP_HEIGHT;
}

const uint16_t
Ball::get_pixmap_origin_x() const
{
  return DEFAULT_PIXMAP_ORIGIN_X;
}

const uint16_t
Ball::get_pixmap_origin_y() const
{
  return DEFAULT_PIXMAP_ORIGIN_Y;
}

const bool
Ball::get_is_in_goal() const
{
  return _is_in_goal;
}

const double
Ball::get_potential(const uint16_t x, const uint16_t y)
{
  if (x >= get_pixmap_width()) {
    Log::fatal("Ball::get_potential(): x out of range");
  }
  if (y >= get_pixmap_height()) {
    Log::fatal("Ball::get_potential(): y out of range");
  }
  const int8_t dx = x - get_pixmap_origin_x();
  const int8_t dy = y - get_pixmap_origin_y();
  const uint16_t r2 = dx * dx + dy * dy;
  return r2 < 49 ? 1.0 : 0.0;
}

void
Ball::set_is_in_goal(const bool is_in_goal)
{
  _is_in_goal = is_in_goal;
}

// DEBUG
const double
Ball::get_theta(const uint16_t x, const uint16_t y) const
{
  if ((x >= _force_field_width) || (y >= _force_field_height)) {
    Log::fatal("Ball::get_theta(): x or y out of range");
  }
  return _op_force_field[y * _force_field_width + x].theta;
}

// DEBUG
const double
Ball::is_reflection(const uint16_t x, const uint16_t y) const
{
  if ((x >= _force_field_width) || (y >= _force_field_height)) {
    Log::fatal("Ball::is_reflection(): x or y out of range");
  }
  return _op_force_field[y * _force_field_width + x].is_reflection;
}

// DEBUG
const double
Ball::is_exclusion_zone(const uint16_t x, const uint16_t y) const
{
  if ((x >= _force_field_width) || (y >= _force_field_height)) {
    Log::fatal("Ball::is_reflection(): x or y out of range");
  }
  return _op_force_field[y * _force_field_width + x].is_exclusion_zone;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
