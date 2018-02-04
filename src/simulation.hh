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

#ifndef SIMULATION_HH
#define SIMULATION_HH

#include <isimulation.hh>
#include <QtCore/QTimer>
#include <balls.hh>
#include <main-window.hh>
#include <sys/timeb.h>

class Simulation : public QTimer, public ISimulation
{
  Q_OBJECT
public:
  explicit Simulation(Balls *balls, Main_window *main_window);
  virtual ~Simulation();
  void begin();
  const bool is_running();
  void pause();
  const bool is_pausing();
  void resume();
  void stop();
  void set_speed(const double speed);
  const double get_speed();
  const bool is_velocity_visible() const;
  void set_velocity_visible(const bool velocity_visible);
  const bool is_force_field_visible() const;
  void set_force_field_visible(const bool force_field_visible);
  const bool is_ball_visible() const;
  void set_ball_visible(const bool ball_visible);
private slots:
  void update();
private:
  enum Status {starting, running, pausing, stopping, stopped, exiting};
  Status _status;
  Main_window *_main_window;
  Balls *_balls;
  struct timeb _last_state_change_ftime;
  void set_status(const Status status);
  const bool has_status(const Status status) const;
  const uint64_t get_time_since_last_state_change() const;
};

#endif /* SIMULATION_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
