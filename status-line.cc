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

#include <status-line.hh>
#include <log.hh>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>

Status_line::Status_line(QWidget *parent) : QWidget(parent)
{
  _simulation = 0;
  _pixmap_pause = new QPixmap("pause.png");
  _icon_pause = new QIcon(*_pixmap_pause);
  _pixmap_play = new QPixmap("play.png");
  _icon_play = new QIcon(*_pixmap_play);
  _pixmap_escape = new QPixmap("escape.png");
  _icon_escape = new QIcon(*_pixmap_escape);
  _pixmap_show_velocity = new QPixmap("show_velocity.png");
  _icon_show_velocity = new QIcon(*_pixmap_show_velocity);
  _pixmap_hide_velocity = new QPixmap("hide_velocity.png");
  _icon_hide_velocity = new QIcon(*_pixmap_hide_velocity);
  _pixmap_show_force_field = new QPixmap("show_force_field.png");
  _icon_show_force_field = new QIcon(*_pixmap_show_force_field);
  _pixmap_hide_force_field = new QPixmap("hide_force_field.png");
  _icon_hide_force_field = new QIcon(*_pixmap_hide_force_field);
  _pixmap_show_ball = new QPixmap("show_ball.png");
  _icon_show_ball = new QIcon(*_pixmap_show_ball);
  _pixmap_hide_ball = new QPixmap("hide_ball.png");
  _icon_hide_ball = new QIcon(*_pixmap_hide_ball);
  _label_keys = 0;

  _layout = new QHBoxLayout();
  if (!_layout) {
    Log::fatal("Status_line::Status_line(): not enough memory");
  }
  setLayout(_layout);

  _about_dialog = new About_dialog(this);
  _license_dialog = new License_dialog(this);

  _button_pause = new QPushButton();
  if (!_button_pause) {
    Log::fatal("not enough memory");
  }
  _button_pause->setText(tr("Pause / Play"));
  _button_pause->setIcon(*_icon_pause);
  _button_pause->setIconSize(_pixmap_pause->rect().size());
  _button_pause->setToolTip(tr("pause / play"));
  _layout->addWidget(_button_pause);

  _button_escape = new QPushButton();
  if (!_button_escape) {
    Log::fatal("not enough memory");
  }
  _button_escape->setText(tr("Quit"));
  _button_escape->setIcon(*_icon_escape);
  _button_escape->setIconSize(_pixmap_escape->rect().size());
  _button_escape->setToolTip(tr("quit"));
  _layout->addWidget(_button_escape);

  _dial_speed = new QDial();
  if (!_dial_speed) {
    Log::fatal("not enough memory");
  }
  _dial_speed->setToolTip(tr("control speed of game"));
  _dial_speed->setValue(_dial_speed->maximum() / 2);
  _layout->addWidget(_dial_speed);

  _button_toggle_velocity_visibility = new QPushButton();
  if (!_button_toggle_velocity_visibility) {
    Log::fatal("not enough memory");
  }
  _button_toggle_velocity_visibility->setText(tr("Show / Hide Velocity"));
  _button_toggle_velocity_visibility->setIcon(*_icon_show_velocity);
  _button_toggle_velocity_visibility->
    setIconSize(_pixmap_show_velocity->rect().size());
  _button_toggle_velocity_visibility->
    setToolTip(tr("toggle velocity visibility"));
  _layout->addWidget(_button_toggle_velocity_visibility);

  _button_toggle_force_field_visibility = new QPushButton();
  if (!_button_toggle_force_field_visibility) {
    Log::fatal("not enough memory");
  }
  _button_toggle_force_field_visibility->setText(tr("Show / Hide Force"));
  _button_toggle_force_field_visibility->setIcon(*_icon_show_force_field);
  _button_toggle_force_field_visibility->
    setIconSize(_pixmap_show_force_field->rect().size());
  _button_toggle_force_field_visibility->
    setToolTip(tr("toggle force field visibility"));
  _layout->addWidget(_button_toggle_force_field_visibility);

  _button_toggle_ball_visibility = new QPushButton();
  if (!_button_toggle_ball_visibility) {
    Log::fatal("not enough memory");
  }
  _button_toggle_ball_visibility->setText(tr("Show / Hide Ball"));
  _button_toggle_ball_visibility->setIcon(*_icon_hide_ball);
  _button_toggle_ball_visibility->
    setIconSize(_pixmap_hide_ball->rect().size());
  _button_toggle_ball_visibility->
    setToolTip(tr("toggle ball visibility"));
  _layout->addWidget(_button_toggle_ball_visibility);

  _label_keys = new QLabel(tr("[p]ause/play, [q]uit, [a]bout, [l]icense, "
                              "\"?\"=help."));
  if (!_label_keys) {
    Log::fatal("not enough memory");
  }
  _layout->addWidget(_label_keys);

  _sensors_display = new Sensors_display(this);
  if (!_sensors_display) {
    Log::fatal("not enough memory");
  }
  _layout->addWidget(_sensors_display);

  create_actions();
}

Status_line::~Status_line()
{
  // Q objects will be deleted by Qt, just set them to 0
  _layout = 0;
  _tool_bar = 0;
  _label_keys = 0;
  _pixmap_play = 0;
  _icon_play = 0;
  _pixmap_pause = 0;
  _icon_pause = 0;
  _pixmap_escape = 0;
  _icon_escape = 0;
  _pixmap_show_velocity = 0;
  _icon_show_velocity = 0;
  _pixmap_hide_velocity = 0;
  _icon_hide_velocity = 0;
  _pixmap_show_force_field = 0;
  _icon_show_force_field = 0;
  _pixmap_hide_force_field = 0;
  _icon_hide_force_field = 0;
  _pixmap_show_ball = 0;
  _icon_show_ball = 0;
  _pixmap_hide_ball = 0;
  _icon_hide_ball = 0;
  _about_dialog = 0;
  _license_dialog = 0;
  _button_pause = 0;
  _button_escape = 0;
  _dial_speed = 0;
  _button_toggle_velocity_visibility = 0;
  _button_toggle_force_field_visibility = 0;
  _button_toggle_ball_visibility = 0;
  _simulation = 0;
}

Sensors_display *
Status_line::get_sensors_display()
{
  return _sensors_display;
}

void
Status_line::set_simulation(ISimulation *simulation)
{
  if (!simulation) {
    Log::fatal("Status_line::set_simulation(): simulation is null");
  }
  _simulation = simulation;
  slot_speed_change();
}

void
Status_line::create_actions()
{
  connect(_button_escape,
          SIGNAL(clicked()),
          this,
          SLOT(slot_quit()));
  connect(_button_pause,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_pause()));
  connect(_dial_speed,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(slot_speed_change()));
  connect(_button_toggle_velocity_visibility,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_velocity_visibility()));
  connect(_button_toggle_force_field_visibility,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_force_field_visibility()));
  connect(_button_toggle_ball_visibility,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_ball_visibility()));
}

void
Status_line::keyPressEvent(QKeyEvent* event)
{
  QString text = event->text();
  if (!text.compare("a")) {
    _about_dialog->show();
  } else if (!text.compare("l")) {
    _license_dialog->show();
  } else if (!text.compare("q")) {
    slot_quit();
  } else if (!text.compare("p")) {
    slot_toggle_pause();
  } else if (!text.compare("?")) {
    QMessageBox* box = new QMessageBox();
    box->setWindowTitle(QString(tr("Help")));
    box->setText(QString::fromUtf8("Help"));
    box->show();
  }
};

void
Status_line::slot_quit()
{
  QApplication::instance()->quit();
}

void
Status_line::slot_toggle_pause()
{
  if (_simulation->is_running()) {
    _simulation->pause();
  } else {
    _simulation->resume();
  }
  if (_simulation->is_pausing()) {
    _button_pause->setIcon(*_icon_play);
    _button_pause->setIconSize(_pixmap_play->rect().size());
  } else {
    _button_pause->setIcon(*_icon_pause);
    _button_pause->setIconSize(_pixmap_pause->rect().size());
  }
}

void
Status_line::slot_speed_change()
{
  const double value =
    ((double)_dial_speed->value()) /
    (_dial_speed->maximum() - _dial_speed->minimum());
  _simulation->set_speed(value);
}

void
Status_line::slot_toggle_velocity_visibility()
{
  _simulation->set_velocity_visible(!_simulation->is_velocity_visible());
  if (_simulation->is_velocity_visible()) {
    _button_toggle_velocity_visibility->setIcon(*_icon_hide_velocity);
    _button_toggle_velocity_visibility->
      setIconSize(_pixmap_hide_velocity->rect().size());
  } else {
    _button_toggle_velocity_visibility->setIcon(*_icon_show_velocity);
    _button_toggle_velocity_visibility->
      setIconSize(_pixmap_show_velocity->rect().size());
  }
}

void
Status_line::slot_toggle_force_field_visibility()
{
  _simulation->set_force_field_visible(!_simulation->is_force_field_visible());
  if (_simulation->is_force_field_visible()) {
    _button_toggle_force_field_visibility->setIcon(*_icon_hide_force_field);
    _button_toggle_force_field_visibility->
      setIconSize(_pixmap_hide_force_field->rect().size());
  } else {
    _button_toggle_force_field_visibility->setIcon(*_icon_show_force_field);
    _button_toggle_force_field_visibility->
      setIconSize(_pixmap_show_force_field->rect().size());
  }
}

void
Status_line::slot_toggle_ball_visibility()
{
  _simulation->set_ball_visible(!_simulation->is_ball_visible());
  if (_simulation->is_ball_visible()) {
    _button_toggle_ball_visibility->setIcon(*_icon_hide_ball);
    _button_toggle_ball_visibility->
      setIconSize(_pixmap_hide_ball->rect().size());
  } else {
    _button_toggle_ball_visibility->setIcon(*_icon_show_ball);
    _button_toggle_ball_visibility->
      setIconSize(_pixmap_show_ball->rect().size());
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
