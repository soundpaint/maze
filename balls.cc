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

#include <balls.hh>
#include <log.hh>

const uint16_t
Balls::DEFAULT_OVERSAMPLING = 100;

Balls::Balls(const std::vector<const Ball_init_data *> balls_init_data,
             const uint16_t rows,
             const uint16_t columns)
{
  _balls = new std::vector<Ball *>();
  if (!_balls) {
    Log::fatal("Balls(): not enough memory");
  }
  for (const Ball_init_data *ball_init_data : balls_init_data) {
    const uint16_t column = ball_init_data->get_column();
    const uint16_t row = ball_init_data->get_row();
    const double align_x = ball_init_data->get_align_x();
    const double align_y = ball_init_data->get_align_y();
    const double x = (column + align_x) / columns;
    const double y = (row + align_y) / rows;
    const double vx = ball_init_data->get_velocity_x();
    const double vy = ball_init_data->get_velocity_y();
    const double mass = ball_init_data->get_mass();
    _balls->push_back(new Ball(x, y, vx, vy, mass));
  }
  _oversampling = DEFAULT_OVERSAMPLING;
  _sensors = 0;
}

Balls::~Balls()
{
  _sensors = 0;
  // TODO: Check: First delete vector elements before deleting the
  // vector (memory leak)?
  delete _balls;
  _balls = 0;
  _oversampling = 0;
}

void
Balls::set_sensors(Sensors *sensors)
{
  _sensors = sensors;
}

const uint8_t
Balls::get_count() const
{
  return _balls->size();
}

Ball *
Balls::at(const uint8_t index) const
{
  if (index < 0) {
    Log::fatal("Balls::get_position(): index < 0");
  }
  if (index >= _balls->size()) {
    Log::fatal("Balls::get_position(): index > _balls->size()");
  }
  return _balls->at(index);
}

void
Balls::update(IPlaying_field *playing_field)
{
  // TODO: Fix memory leaks from use of Point_3D.
  const uint16_t width = playing_field->get_width();
  const uint16_t height = playing_field->get_height();
  for (uint16_t i = 0; i < _balls->size(); i++) {
    Ball *ball = _balls->at(i);
    const uint16_t pixmap_width = ball->get_pixmap_width();
    const uint16_t pixmap_height = ball->get_pixmap_height();
    const uint16_t pixmap_origin_x = ball->get_pixmap_origin_x();
    const uint16_t pixmap_origin_y = ball->get_pixmap_origin_y();

    const double old_px = ball->get_position()->get_x();
    const double old_py = ball->get_position()->get_y();

    if (_sensors) {
      for (uint16_t i = 0; i < _oversampling; i++) {
        ball->update(_sensors, width, height);
      }
    }

    const double new_px = ball->get_position()->get_x();
    const double new_py = ball->get_position()->get_y();

    // schedule undrawing ball at old position
    playing_field->invalidate_rect(old_px, old_py,
                                   pixmap_width, pixmap_height,
                                   pixmap_origin_x, pixmap_origin_y);

    // schedule drawing ball at new position
    playing_field->invalidate_rect(new_px, new_py,
                                   pixmap_width, pixmap_height,
                                   pixmap_origin_x, pixmap_origin_y);

    if (playing_field->matches_goal(new_px, new_py)) {
      ball->set_is_in_goal(true);
    }
  }
}

const bool
Balls::all_balls_in_goal() const
{
  // TODO: Performance: Do not loop each time through all balls, but
  // maintain a counter of the number of balls in goal.
  for (uint16_t i = 0; i < _balls->size(); i++) {
    Ball *ball = _balls->at(i);
    if (!ball->get_is_in_goal()) {
      return false;
    }
  }
  return true;
}

void
Balls::set_oversampling(const uint16_t oversampling)
{
  _oversampling = oversampling;
}

const uint16_t
Balls::get_oversampling()
{
  return _oversampling;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
