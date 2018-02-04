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

#ifndef STATUS_LINE_HH
#define STATUS_LINE_HH

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QDial>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <playing-field.hh>
#include <about-dialog.hh>
#include <license-dialog.hh>
#include <sensors-display.hh>
#include <isimulation.hh>
#include <balls.hh>

class Status_line : public QWidget
{
  Q_OBJECT
public:
  explicit Status_line(QWidget *parent);
  virtual ~Status_line();
  void set_simulation(ISimulation *simulation);
  Sensors_display *get_sensors_display();
public slots:
  void slot_quit();
  void slot_toggle_pause();
  void slot_speed_change();
  void slot_toggle_velocity_visibility();
  void slot_toggle_force_field_visibility();
  void slot_toggle_ball_visibility();
private:
  ISimulation *_simulation;
  QHBoxLayout *_layout;
  QToolBar *_tool_bar;
  QPushButton *_button_escape;
  QPushButton *_button_pause;
  QDial *_dial_speed;
  QPushButton *_button_toggle_velocity_visibility;
  QPushButton *_button_toggle_force_field_visibility;
  QPushButton *_button_toggle_ball_visibility;
  QPixmap *_pixmap_pause;
  QPixmap *_pixmap_play;
  QPixmap *_pixmap_escape;
  QPixmap *_pixmap_show_velocity;
  QPixmap *_pixmap_hide_velocity;
  QPixmap *_pixmap_show_force_field;
  QPixmap *_pixmap_hide_force_field;
  QPixmap *_pixmap_show_ball;
  QPixmap *_pixmap_hide_ball;
  QIcon *_icon_pause;
  QIcon *_icon_play;
  QIcon *_icon_escape;
  QIcon *_icon_show_velocity;
  QIcon *_icon_hide_velocity;
  QIcon *_icon_show_force_field;
  QIcon *_icon_hide_force_field;
  QIcon *_icon_show_ball;
  QIcon *_icon_hide_ball;
  QLabel *_label_keys;
  Sensors_display *_sensors_display;
  About_dialog *_about_dialog;
  License_dialog *_license_dialog;
  void create_actions();
  void keyPressEvent(QKeyEvent* e);
};

#endif /* STATUS_LINE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
