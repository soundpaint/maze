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

#ifndef MESSAGE_OVERLAY_HH
#define MESSAGE_OVERLAY_HH

#include <inttypes.h>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtWidgets/QLabel>

class Message_overlay : public QLabel
{
  Q_OBJECT
public:
  explicit Message_overlay(const uint16_t interval_on,
			   const uint16_t interval_off,
			   QWidget *parent);
  virtual ~Message_overlay();
  void show(const char *message);
  void hide();
private slots:
  void slot_tick();
private:
  uint16_t _interval_on;
  uint16_t _interval_off;
  QTimer *_blinker;
};

#endif /* MESSAGE_OVERLAY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
