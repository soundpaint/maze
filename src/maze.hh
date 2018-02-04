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

#ifndef MAZE_HH
#define MAZE_HH

#include <pthread.h>
#include <QtWidgets/QApplication>
#include <splash-screen.hh>
#include <maze-config.hh>
#include <sensors.hh>
#include <balls.hh>
#include <main-window.hh>
#include <simulation.hh>

class Maze : public QApplication
{
  Q_OBJECT
public:
  explicit Maze(int &argc, char **argv);
  virtual ~Maze();
  void init(IProgress_info *progress_info);
  Main_window *get_main_window() const;
private slots:
  void slot_last_window_closed();
private:
  static const char *STYLE_SHEET;
  static Simulation *_simulation;
  Maze_config *_config;
  Sensors *_sensors;
  Balls *_balls;
  Main_window *_main_window;
};

#endif /* MAZE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
