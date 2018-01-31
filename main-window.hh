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

#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <iprogress-info.hh>
#include <maze-config.hh>
#include <playing-field.hh>
#include <message-overlay.hh>
#include <status-line.hh>
#include <balls.hh>

class Main_window : public QMainWindow
{
  Q_OBJECT
public:
  explicit Main_window(IProgress_info *progress_info,
                       const Maze_config *config,
                       Balls *balls, QWidget *parent = 0);
  virtual ~Main_window();
  Playing_field *get_playing_field();
  Status_line *get_status_line();
  void show_overlay_message(const char *message);
  void hide_overlay_message();
public slots:
  void slot_repaint_playing_field();
private:
  QVBoxLayout *_central_widget_layout;
  QWidget *_central_widget;
  Playing_field *_playing_field;
  Message_overlay *_message_overlay;
  Status_line *_status_line;
};

#endif /* MAIN_WINDOW_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
