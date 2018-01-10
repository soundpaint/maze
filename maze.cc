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
#include <maze.hh>
#include <unistd.h>
#include <log.hh>
#include <playing-field.hh>

Simulation *
Maze::_simulation;

const char *
Maze::STYLE_SHEET =
  "QGroupBox {\n"
  "    border: 1px solid gray;\n"
  "    border-radius: 3px;\n"
  "    margin-top: 0.5em;\n"
  "}\n"
  "\n"
  "QGroupBox::title {\n"
  "    subcontrol-origin: margin;\n"
  "    left: 10px;\n"
  "    padding: 0 3px 0 3px;\n"
  "}\n"
  "\n";

Maze::Maze(int &argc, char **argv)
  : QApplication(argc, argv, 0)
{
  srand(1);
  _main_window = 0;

  _config = new Maze_config("config.xml");
  if (!_config) {
    Log::fatal("Maze(): not enough memory");
  }

  _sensors = new Sensors();
  if (!_sensors) {
    Log::fatal("Maze(): not enough memory");
  }

  const Brush_field *brush_field = _config->get_brush_field();
  std::vector<const Ball_init_data *> balls_init_data =
    brush_field->get_balls_init_data();
  const uint16_t rows = brush_field->get_rows();
  const uint16_t columns = brush_field->get_columns();
  _balls = new Balls(_sensors,
                     balls_init_data,
                     rows,
                     columns);
  if (!_balls) {
    Log::fatal("Maze(): not enough memory");
  }

  setStyleSheet(STYLE_SHEET);
  _main_window = new Main_window(_config, _balls);
  if (!_main_window) {
    Log::fatal("Maze(): not enough memory");
  }

  _main_window->get_playing_field()->add_field_geometry_listener(_sensors);

  _simulation = new Simulation(_balls, _main_window);
  if (!_simulation) {
    Log::fatal("Maze(): not enough memory");
  }
  _simulation->start(50);

  _main_window->get_status_line()->set_simulation(_simulation);

  //_main_window->showFullScreen();
  _main_window->show();

  _simulation->start();
}

Maze::~Maze()
{
  // Q objects will be deleted by Qt, just set them to 0
  _main_window = 0;
  _simulation = 0;

  delete _sensors;
  _sensors = 0;

  delete _config;
  _config = 0;
}

void
Maze::slot_last_window_closed()
{
  // TODO
}

int main(int argc, char *argv[])
{
  Maze *maze = new Maze(argc, argv);
  int result = maze->exec();

  delete maze;
  maze = 0;

  exit(result);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
