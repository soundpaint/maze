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

#ifndef SENSORS_HH
#define SENSORS_HH

#include <QtCore/QTimer>
#include <RTIMULib.h>
#include <ifield-geometry-listener.hh>

class Sensors : public QTimer, public IField_geometry_listener
{
  Q_OBJECT
public:
  Sensors(QObject *parent = 0);
  virtual ~Sensors();
  const RTFLOAT get_pitch() const;
  const RTFLOAT get_roll() const;
  const RTFLOAT get_accel_x() const;
  const RTFLOAT get_accel_y() const;
  const RTFLOAT get_temperature() const;
  virtual void geometry_changed(const uint16_t width,
                                const uint16_t height);
private slots:
  void sample_and_hold();
signals:
  void sample_updated(const RTFLOAT pitch,
                      const RTFLOAT roll,
                      const RTFLOAT accel_x,
                      const RTFLOAT accel_y,
                      const RTFLOAT temperature);
private:
  uint16_t _field_width;
  uint16_t _field_height;
  uint64_t _display_timer;
  RTIMUSettings *_settings;
  RTIMU *_imu;
  RTFLOAT _pitch;
  RTFLOAT _roll;
  RTFLOAT _accel_x;
  RTFLOAT _accel_y;
  RTFLOAT _temperature;
};

#endif /* SENSORS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
