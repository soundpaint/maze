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
#include <playing-field.hh>
#include <ball.hh>
#include <log.hh>

#define BACKGROUND_MODE_NORMAL 0
#define BACKGROUND_MODE_FORCES 1
#define BACKGROUND_MODE_REFLECTIONS 2

#define BACKGROUND_MODE BACKGROUND_MODE_NORMAL

Playing_field::Playing_field(Brush_field *brush_field,
                             Balls *balls,
                             QWidget *parent) :
  QWidget(parent),
  _balls(balls),
  _brush_field(brush_field)
{
  _velocity_visible = false;
  _force_field_visible = false;
  _ball_visible = true;

  _field_geometry_listeners = new std::vector<IField_geometry_listener *>();
  if (!_field_geometry_listeners) {
    Log::fatal("not enough memory");
  }

  if (!balls) {
    Log::fatal("Playing_field::Playing_field(): balls is null");
  }

  if (!brush_field) {
    Log::fatal("Playing_field::Playing_field(): brush_field is null");
  }

  _force_field = new Force_field();
  if (!_force_field) {
    Log::fatal("Playing_field::Playing_field(): not enough memory");
  }

  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  _background = 0;
}

Playing_field::~Playing_field()
{
  _velocity_visible = false;
  _force_field_visible = false;
  _ball_visible = false;

  delete _field_geometry_listeners;
  _field_geometry_listeners = 0;

  delete _balls;
  _balls = 0;

  delete _brush_field;
  _brush_field = 0;

  delete _force_field;
  _force_field = 0;

  // Q objects will be deleted by Qt, just set them to 0
  _background = 0;
}

const uint16_t
Playing_field::get_width() const
{
  return width();
}

const uint16_t
Playing_field::get_height() const
{
  return height();
}

void
Playing_field::add_field_geometry_listener(IField_geometry_listener *listener)
{
  _field_geometry_listeners->push_back(listener);
}

void
Playing_field::create_background_normal(const uint16_t width,
                                        const uint16_t height,
                                        QPainter *painter)
{
  for (uint16_t x = 0; x < width; x++) {
    const double field_x = ((double)x) / width;
    for (uint16_t y = 0; y < height; y++) {
      const double field_y = ((double)y) / height;
      const QBrush *brush = _brush_field->get_brush(field_x, field_y);
      painter->fillRect(x, y, 1, 1, *brush);
    }
  }
}

void
Playing_field::create_background_forces(const uint16_t width,
                                        const uint16_t height,
                                        QPainter *painter)
{
  for (uint16_t x = 0; x < width; x++) {
    const double field_x = ((double)x) / width;
    for (uint16_t y = 0; y < height; y++) {
      const double field_y = ((double)y) / height;
      QColor color;
      const Ball *ball = _balls->at(0);
      if (!ball->is_reflection(x, y)) {
        color = QColor(0, 0, 0);
      } else if (_brush_field->get_potential(field_x, field_y) > 0.5) {
        color = QColor(255, 255, 255);
      } else {
        int16_t theta = (int16_t)(ball->get_theta(x, y) / M_PI * 180.0);
        while (theta < 0) {
          theta += 360;
        }
        while (theta >= 360) {
          theta -= 360;
        }
        color = QColor::fromHsv(theta, 255, 255);
      }
      const QBrush color_brush = QBrush(color);
      painter->fillRect(x, y, 1, 1, color_brush);
    }
  }
}

void
Playing_field::create_background_reflections(const uint16_t width,
                                             const uint16_t height,
                                             QPainter *painter)
{
  for (uint16_t x = 0; x < width; x++) {
    //const double field_x = ((double)x) / width;
    for (uint16_t y = 0; y < height; y++) {
      //const double field_y = ((double)y) / height;
      QColor color;
      const Ball *ball = _balls->at(0);
      if (!ball->is_reflection(x, y)) {
        if (!ball->is_exclusion_zone(x, y)) {
          color = QColor(0, 0, 0);
        } else {
          color = QColor(255, 0, 0);
        }
      } else {
        if (!ball->is_exclusion_zone(x, y)) {
          color = QColor(0, 255, 0);
        } else {
          color = QColor(255, 255, 255);
        }
      }
      const QBrush color_brush = QBrush(color);
      painter->fillRect(x, y, 1, 1, color_brush);
    }
  }
}

QImage *
Playing_field::create_background(const uint16_t width,
                                 const uint16_t height)
{
  if (width <= 0) {
    Log::fatal("Playing_field::create_background(): width <= 0");
  }
  if (height <= 0) {
    Log::fatal("Playing_field::create_background(): height <= 0");
  }

  {
    std::stringstream msg;
    msg << "[create_background]";
    msg << " width=" << width;
    msg << ", height=" << height;
    Log::info(msg.str());
  }

  QImage *image = new QImage(width, height, QImage::Format_RGB32);
  if (!image) {
    Log::fatal("Playing_field::create_background(): "
               "not enough memory");
  }
  QPainter painter(image);
  if (BACKGROUND_MODE == BACKGROUND_MODE_NORMAL) {
    create_background_normal(width, height, &painter);
  } else if (BACKGROUND_MODE == BACKGROUND_MODE_FORCES) {
    create_background_forces(width, height, &painter);
  } else { // (BACKGROUND_MODE == BACKGROUND_MODE_REFLECTIONS)
    create_background_reflections(width, height, &painter);
  }
  return image;
}

void
Playing_field::check_update_geometry()
{
  const uint16_t current_width = width();
  const uint16_t current_height = height();
  bool need_update = false;
  if (!_background) {
    need_update = true;
  } else {
    need_update =
      (current_width != _background->width()) ||
      (current_height != _background->height());
  }
  if (need_update) {
    _brush_field->geometry_changed(current_width, current_height);
    for (IField_geometry_listener *listener : *_field_geometry_listeners) {
      listener->geometry_changed(current_width, current_height);
    }

    /*
     * Unfortunately, we can not add this Playing-field object instance to
     * the vector of listeners, since a class can not inherit from both,
     * a QWidget and an additional interface like the listener interface.
     * Therefore, we must -- in addition to the above for loop -- call
     * method geometry_changed() explicitly on this class.
     */
    geometry_changed(current_width, current_height);
  }
}

void
Playing_field::geometry_changed(const uint16_t width, const uint16_t height)
{
  {
    std::stringstream msg;
    msg << "[playing_field] new geometry: " <<
      "width=" << width << ", height=" << height;
    Log::debug(msg.str());
  }
  Log::debug("loading force field");
  _force_field->load_field(_brush_field, width, height);
  Log::debug("compute forces field onto ball");
  for (uint8_t i = 0; i < _balls->get_count(); i++) {
    Ball *ball = _balls->at(i);
    ball->precompute_forces(_force_field);
  }
  Log::debug("(re-)create background");
  if (_background) {
    delete _background;
    _background = 0;
  }
  _background = create_background(width, height);
  if (!_background) {
    Log::fatal("Playing_field::paintEvent(): not enough memory");
  }
}

void
Playing_field::draw_balls(QPainter *painter, const QRect rect)
{
  const uint16_t current_width = width();
  const uint16_t current_height = height();
  for (uint8_t i = 0; i < _balls->get_count(); i++) {
    const Ball *ball = _balls->at(i);
    const uint16_t pixmap_origin_x = ball->get_pixmap_origin_x();
    const uint16_t pixmap_origin_y = ball->get_pixmap_origin_y();
    const double px = ball->get_position()->get_x();
    const double py = ball->get_position()->get_y();
    const uint16_t x = (uint16_t)(current_width * px + 0.5) - pixmap_origin_x;
    const uint16_t y = (uint16_t)(current_height * py + 0.5) - pixmap_origin_y;
    painter->setPen(Qt::black);
    const QPixmap *pixmap = ball->get_pixmap();
    painter->drawPixmap(x, y, *pixmap);
  }
}

void
Playing_field::draw_velocities(QPainter *painter, const QRect rect)
{
  const uint16_t current_width = width();
  const uint16_t current_height = height();
  for (uint8_t i = 0; i < _balls->get_count(); i++) {
    const Ball *ball = _balls->at(i);
    painter->setPen(Qt::black);
    const double px = current_width * ball->get_position()->get_x();
    const double py = current_height * ball->get_position()->get_y();
    const double rx = ball->get_velocity()->get_x();
    const double ry = ball->get_velocity()->get_y();
    const double v_norm = sqrt(rx * rx + ry * ry);
    double normed_rx, normed_ry;
    if (v_norm > 0.0) {
      normed_rx = 8.0 * rx / v_norm;
      normed_ry = 8.0 * ry / v_norm;
    } else {
      normed_rx = 0.0;
      normed_ry = 0.0;
    }
    painter->drawLine((int)(px), (int)(py),
                      (int)(px + normed_rx), (int)(py + normed_ry));
  }
}

void
Playing_field::paintEvent(QPaintEvent *event)
{
  check_update_geometry();
  const QRect rect = event->rect();
  QPainter painter(this);
  if (_background) {
    painter.drawImage(rect, *_background, rect);
  }
  if (_ball_visible) {
    draw_balls(&painter, rect);
  }
  if (_velocity_visible) {
    draw_velocities(&painter, rect);
  }
  painter.end();
}

void
Playing_field::invalidate_rect(const double px, const double py,
                               const uint16_t pixmap_width,
                               const uint16_t pixmap_height,
                               const uint16_t pixmap_origin_x,
                               const uint16_t pixmap_origin_y)
{
  const uint16_t current_width = width();
  const uint16_t current_height = height();
  const uint16_t x = (uint16_t)(current_width * px + 0.5) - pixmap_origin_x;
  const uint16_t y = (uint16_t)(current_height * py + 0.5) - pixmap_origin_y;
  const QRect paintRect(x, y, pixmap_width, pixmap_height);
  update(paintRect);
}

const bool
Playing_field::matches_goal(const double px, const double py) const
{
  return _brush_field->matches_goal(px, py);
}

const bool
Playing_field::is_exclusion_zone(const uint16_t x, const uint16_t y) const
{
  return _force_field->is_exclusion_zone(x, y);
}

const bool
Playing_field::is_velocity_visible() const
{
  return _velocity_visible;
}

void
Playing_field::set_velocity_visible(const bool velocity_visible)
{
  _velocity_visible = velocity_visible;
}

const bool
Playing_field::is_force_field_visible() const
{
  return _force_field_visible;
}

void
Playing_field::set_force_field_visible(const bool force_field_visible)
{
  _force_field_visible = force_field_visible;
}

const bool
Playing_field::is_ball_visible() const
{
  return _ball_visible;
}

void
Playing_field::set_ball_visible(const bool ball_visible)
{
  _ball_visible = ball_visible;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
