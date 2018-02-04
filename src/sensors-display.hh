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

#ifndef SENSORS_DISPLAY_HH
#define SENSORS_DISPLAY_HH

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class Sensors_display : public QWidget
{
  Q_OBJECT
public:
  Sensors_display(QWidget *parent = 0);
  virtual ~Sensors_display();
  QSize minimumSizeHint() const Q_DECL_OVERRIDE;
  QSize sizeHint() const Q_DECL_OVERRIDE;
public slots:
  void slot_update_sensors_display(const double pitch,
				   const double roll,
				   const double ax,
				   const double ay,
				   const double temperature);
protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
  double _pitch;
  double _roll;
  double _accel_x;
  double _accel_y;
  double _temperature;
  const uint16_t WIDTH = 50;
  const uint16_t HEIGHT = 50;
};

#endif /* SENSORS_DISPLAY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
