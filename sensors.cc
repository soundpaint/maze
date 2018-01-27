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
//#include <QtGui/QCursor>
#include <log.hh>

Sensors::Sensors(QObject *parent) : QTimer(parent)
{
  _pitch = 0;
  _roll = 0;
  _accel_x = 0;
  _accel_y = 0;

  start(10);

  _settings = new RTIMUSettings("RTIMULib");
  if (!_settings) {
    Log::fatal("Sensors::Sensors(): not enough memory");
  }

  _imu = RTIMU::createIMU(_settings);
  if ((_imu == NULL) || (_imu->IMUType() == RTIMU_TYPE_NULL)) {
    Log::warn("Sensors::Sensors(): no IMU found => sensors will not work");
  } else {
    _imu->IMUInit();
    _imu->setSlerpPower(0.02);
    _imu->setGyroEnable(true);
    _imu->setAccelEnable(true);
    _imu->setCompassEnable(true);
  }
  _display_timer = RTMath::currentUSecsSinceEpoch();
  QObject::connect(this, SIGNAL(timeout()),
                   this, SLOT(sample_and_hold()));
  QObject::connect(this, SIGNAL(sample_updated(double, double, double, double, double)),
                   parent, SLOT(slot_update_sensors_display(double, double, double, double, double)));
}

Sensors::~Sensors()
{
  if (_imu) {
    delete _imu;
    _imu = 0;
  }

  if (_settings) {
    delete _settings;
    _settings = 0;
  }

  _display_timer = 0;
  _pitch = 0;
  _roll = 0;
  _accel_x = 0;
  _accel_y = 0;
}

void
Sensors::sample_and_hold()
{
  const uint64_t now = RTMath::currentUSecsSinceEpoch();
  if ((now - _display_timer) > 100000) {
    if (_imu->IMURead()) {
      const RTIMU_DATA imuData = _imu->getIMUData();
      _pitch = imuData.fusionPose.x(); // * RTMATH_RAD_TO_DEGREE;
      _roll = imuData.fusionPose.y(); // * RTMATH_RAD_TO_DEGREE;
      _accel_x = imuData.accel.x();
      _accel_y = imuData.accel.y();
      _temperature = imuData.temperature;

      // update sensors status display (TODO)
      emit sample_updated(_pitch, _roll, _accel_x, _accel_y,
                          _temperature);
    }
    _display_timer = now;
  }
}

const RTFLOAT
Sensors::get_pitch() const
{
  return _pitch;
}

const RTFLOAT
Sensors::get_roll() const
{
  return _roll;
}

const RTFLOAT
Sensors::get_accel_x() const
{
  return _accel_x;
}

const RTFLOAT
Sensors::get_accel_y() const
{
  return _accel_y;
}

const RTFLOAT
Sensors::get_temperature() const
{
  return _temperature;
}

void
Sensors::geometry_changed(const uint16_t width,
                          const uint16_t height)
{
  _field_width = width;
  _field_height = height;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
