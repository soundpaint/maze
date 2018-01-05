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

#include <message-overlay.hh>
#include <log.hh>
#include <QtGui/QFont>

Message_overlay::Message_overlay(const uint16_t interval_on,
                                 const uint16_t interval_off,
                                 QWidget *parent)
  : QLabel(parent)
{
  setWindowTitle(QString(""));
  setStyleSheet("background:transparent;");
  setAttribute(Qt::WA_TranslucentBackground);
  setStyleSheet("QLabel { color : blue; }");
  QFont font = QLabel::font();
  font.setPointSize(216);
  font.setBold(true);
  setFont(font);
  if ((interval_on > 0) || (interval_off > 0)) {
    _blinker = new QTimer();
    if (!_blinker) {
      Log::fatal("Message_overlay::Message_overlay(): not enough memory");
    }
    connect(_blinker, SIGNAL(timeout()), this, SLOT(slot_tick()));
  } else {
    _blinker = 0;
  }
  _interval_on = interval_on;
  _interval_off = interval_off;
}

Message_overlay::~Message_overlay()
{
  if (_blinker) {
    _blinker->stop();
  }
  _blinker = 0;
  _interval_on = 0;
  _interval_off = 0;
}

void
Message_overlay::slot_tick()
{
  if (isVisible()) {
    QLabel::hide();
    _blinker->start(_interval_off);
  } else {
    QLabel::show();
    _blinker->start(_interval_on);
  }
}

void
Message_overlay::show(const char *message)
{
  if (!message) {
    Log::fatal("Message_overlay::Message_overlay(): message is null");
  }
  setText(QString::fromUtf8(message));
  adjustSize();
  const QWidget *parent = parentWidget()->window();
  move(parent->frameGeometry().topLeft() +
       parent->rect().center() - rect().center());
  QLabel::show();
  if (_blinker) {
    _blinker->start(_interval_on);
  }
}

void
Message_overlay::hide()
{
  _blinker->stop();
  QLabel::hide();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
