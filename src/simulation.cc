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

#include <simulation.hh>
#include <playing-field.hh>
#include <log.hh>

Simulation::Simulation(Balls *balls, Main_window *main_window)
  : QTimer(main_window)
{
  set_status(starting);
  if (!balls) {
    Log::fatal("Simulation::Simulation(): "
               "balls is null");
  }
  _balls = balls;
  if (!main_window) {
    Log::fatal("Simulation::Simulation(): "
               "main_window is null");
  }
  _main_window = main_window;
  connect(this, SIGNAL(timeout()),
          this, SLOT(update()));
  begin();
}

Simulation::~Simulation()
{
  set_status(exiting);
  _main_window = 0;
  _balls = 0;
}

void
Simulation::set_status(const Status status)
{
  // TODO: check for invalid state change
  _status = status;
  ftime(&_last_state_change_ftime);
  if (_status == exiting) {
    _main_window->get_status_line()->slot_quit();
  }
}

const bool
Simulation::has_status(const Status status) const
{
  return _status == status;
}

const uint64_t
Simulation::get_time_since_last_state_change() const
{
  timeb now_ftime;
  ftime(&now_ftime);
  time_t diff_time = now_ftime.time - _last_state_change_ftime.time;
  int16_t diff_millitm =
    now_ftime.millitm - _last_state_change_ftime.millitm;
  uint64_t diff = diff_time * 1000 + diff_millitm;
  if (diff < 0) {
    Log::fatal("Simulation::has_current_status_for_longer_than(): "
               "time value overflow");
  }
  return diff;
}

void
Simulation::begin()
{
  if (!has_status(starting)) {
    Log::fatal("Simulation::start(): invalid state change");
  }
  set_status(running);
}

const bool
Simulation::is_running()
{
  return has_status(running);
}

void
Simulation::pause()
{
  if (has_status(running)) {
    set_status(pausing);
  } else {
    // pause function disabled during other states
  }
}

const bool
Simulation::is_pausing()
{
  return has_status(pausing);
}

void
Simulation::resume()
{
  if (has_status(pausing)) {
    set_status(running);
  } else {
    // pause function disabled during other states
  }
}

void
Simulation::stop()
{
  if (!has_status(running)) {
    Log::fatal("Simulation::stop(): invalid state change");
  }
  set_status(stopping);
}

void
Simulation::set_speed(const double speed)
{
  if ((speed < 0.0) || (speed > 1.0)) {
    Log::fatal("Simulation::set_speed(): speed out of range");
  }
  const uint16_t oversampling = (uint16_t)(exp(speed * log(UINT16_MAX)) + 0.5);
  _balls->set_oversampling(oversampling);
}

const double
Simulation::get_speed()
{
  const uint16_t oversampling = _balls->get_oversampling();
  return log(oversampling) / log(UINT16_MAX);
}

const bool
Simulation::is_velocity_visible() const
{
  Playing_field *playing_field = _main_window->get_playing_field();
  return playing_field->is_velocity_visible();
}

void
Simulation::set_velocity_visible(const bool velocity_visible)
{
  Playing_field *playing_field = _main_window->get_playing_field();
  playing_field->set_velocity_visible(velocity_visible);
}

const bool
Simulation::is_force_field_visible() const
{
  Playing_field *playing_field = _main_window->get_playing_field();
  return playing_field->is_force_field_visible();
}

void
Simulation::set_force_field_visible(const bool force_field_visible)
{
  Playing_field *playing_field = _main_window->get_playing_field();
  playing_field->set_force_field_visible(force_field_visible);
}

const bool
Simulation::is_ball_visible() const
{
  Playing_field *playing_field = _main_window->get_playing_field();
  return playing_field->is_ball_visible();
}

void
Simulation::set_ball_visible(const bool ball_visible)
{
  Playing_field *playing_field = _main_window->get_playing_field();
  playing_field->set_ball_visible(ball_visible);
}

void
Simulation::update()
{
  switch (_status)
  {
    case starting:
      // done => do nothing
      break;
    case running:
      {
        Playing_field *playing_field = _main_window->get_playing_field();
        _balls->update(playing_field);
        if (_balls->all_balls_in_goal()) {
          set_status(stopping);
          _main_window->show_overlay_message("Game\nover!");
        }
      }
      break;
    case pausing:
      // pause => do nothing
      break;
    case stopping:
      {
        const uint64_t elapsed_time = get_time_since_last_state_change();
        if (elapsed_time >= 4200) {
          _main_window->hide_overlay_message();
          set_status(stopped);
        }
      }
      break;
    case stopped:
      {
        const uint64_t elapsed_time = get_time_since_last_state_change();
        if (elapsed_time >= 1000) {
          _main_window->hide_overlay_message();
          set_status(exiting);
        }
      }
      break;
    case exiting:
      // done => do nothing
      break;
    default:
      Log::fatal("Simulation::update(): unexpected case fall-through");
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
