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

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QApplication>
#include <sensors-display.hh>
#include <log.hh>

Sensors_display::Sensors_display(QWidget *parent) : QWidget(parent)
{
  _pitch = 0.0;
  _roll = 0.0;
  _accel_x = 0.0;
  _accel_y = 0.0;
  _temperature = 0.0;
}

Sensors_display::~Sensors_display()
{
  _pitch = 0.0;
  _roll = 0.0;
  _accel_x = 0.0;
  _accel_y = 0.0;
  _temperature = 0.0;
}

QSize
Sensors_display::sizeHint() const
{
  return QSize(WIDTH, HEIGHT);
}

QSize
Sensors_display::minimumSizeHint() const
{
  return QSize(WIDTH, HEIGHT);
}

void
Sensors_display::paintEvent(QPaintEvent *event)
{
  const uint16_t pos_accel_x = (0.5 + 0.5 * _accel_x) * WIDTH;
  const uint16_t pos_accel_y = (0.5 + 0.5 * _accel_y) * HEIGHT;
  const uint16_t center_x = WIDTH / 2;
  const uint16_t center_y = HEIGHT / 2;
  QPainter painter(this);
  QPen pen(Qt::red);
  painter.setPen(pen);
  painter.drawRect(0, 0, WIDTH - 1, HEIGHT - 1);
  {
    std::stringstream msg;
    msg << "_accel_x=" << _accel_x << ", _accel_y=" << _accel_y;
    Log::info(msg.str());
  }
  painter.drawLine(center_x, center_y, pos_accel_x, pos_accel_y);
  painter.end();
}

void
Sensors_display::slot_update_sensors_display(const double pitch,
                                             const double roll,
                                             const double accel_x,
                                             const double accel_y,
                                             const double temperature)
{
  _pitch = pitch;
  _roll = roll;
  _accel_x = accel_x;
  _accel_y = accel_y;
  _temperature = temperature;
  const QRect paintRect(0, 0, width(), height());
  update(paintRect);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
