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

#include <sensors.hh>
#include <cmath>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtGui/QCursor>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <log.hh>

Sensors::Sensors()
{
  _pitch = 0;
  _roll = 0;
  _yaw = 0;
}

Sensors::~Sensors()
{
  _pitch = 0;
  _roll = 0;
  _yaw = 0;
}

void
Sensors::sample_and_hold()
{
#if 1
  QScreen *screen = QGuiApplication::primaryScreen();
#else
  QScreen *screen = qApp->primaryScreen();
#endif
  QRect geometry = screen->geometry();
  uint16_t width = geometry.width();
  uint16_t height = geometry.height();
  QPoint pos = QPoint(width / 2, height / 2); // QCursor::pos();
  double pitch = atan(pos.x() - width / 2);
  double roll = atan(pos.y() - height / 2);
  _pitch = (int)(0.5 + floor(180.0 * pitch / M_PI));
  _roll = (int)(0.5 + floor(180.0 * roll / M_PI));

  // DEBUG START
  _pitch = 3;
  _roll = 1;
  // DEBUG STOP

  _yaw = 0;
}

uint16_t
Sensors::get_pitch() const
{
  return _pitch;
}

uint16_t
Sensors::get_roll() const
{
  return _roll;
}

uint16_t
Sensors::get_yaw() const
{
  return _yaw;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
