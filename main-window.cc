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

#include <main-window.hh>
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QScreen>
#include <log.hh>

Main_window::Main_window(const Maze_config *config, Balls *balls,
                         QWidget *parent)
  : QMainWindow(parent)
{
  if (!balls) {
    Log::fatal("Main_window(): balls is null");
  }

#if 0 // full-screen mode
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  setCursor(Qt::BlankCursor);
  const QScreen *screen = qApp->primaryScreen();
  const QRect geometry = screen->geometry();
  const uint16_t width = geometry.width();
  const uint16_t height = geometry.height();
#else // window mode
  const uint16_t width = 1300;
  const uint16_t height = 600;
#endif

  setWindowTitle(tr("Maze"));
  const QIcon icon("main-window-icon.png");
  setWindowIcon(icon);

  _central_widget_layout = new QVBoxLayout();
  if (!_central_widget_layout) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }

  _central_widget = new QWidget();
  if (!_central_widget) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  _central_widget->setLayout(_central_widget_layout);
  setCentralWidget(_central_widget);

  Brush_field *brush_field = config->get_brush_field();
  brush_field->geometry_changed(width, height);

  _playing_field = new Playing_field(brush_field, width, height, balls, this);
  if (!_playing_field) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  _central_widget_layout->addWidget(_playing_field, 1.0);

  _status_line = new Status_line(this);
  if (!_status_line) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  _central_widget_layout->addWidget(_status_line, 0.0);

  _message_overlay = new Message_overlay(900, 600, this);
  if (!_message_overlay) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
}

Main_window::~Main_window()
{
  // Q objects will be deleted by Qt, just set them to 0
  _playing_field = 0;
  _central_widget_layout = 0;
  _central_widget = 0;
  _message_overlay = 0;
}

void
Main_window::show_overlay_message(const char *message)
{
  _message_overlay->show(message);
}

void
Main_window::hide_overlay_message()
{
  _message_overlay->hide();
}

Playing_field *
Main_window::get_playing_field()
{
  return _playing_field;
}

Status_line *
Main_window::get_status_line()
{
  return _status_line;
}

void
Main_window::slot_repaint_playing_field()
{
  _playing_field->update();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
