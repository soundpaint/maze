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

#ifndef PLAYING_FIELD_HH
#define PLAYING_FIELD_HH

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
#include <ifield-geometry-listener.hh>
#include <iplaying-field.hh>
#include <balls.hh>
#include <brush-field.hh>
#include <force-field.hh>

class Playing_field :
  public QWidget, public IPlaying_field, public IField_geometry_listener
{
  Q_OBJECT
public:
  explicit Playing_field(Brush_field *brush_field,
                         Balls *balls,
                         QWidget *parent = 0);
  virtual ~Playing_field();
  virtual const uint16_t get_width() const;
  virtual const uint16_t get_height() const;
  void invalidate_rect(const double px, const double py,
                       const uint16_t pixmap_width,
                       const uint16_t pixmap_height,
                       const uint16_t pixmap_origin_x,
                       const uint16_t pixmap_origin_y);
  void invalidate_rect(const uint16_t px, const uint16_t py,
                       const uint16_t width, const uint16_t height);
  const bool matches_goal(const double px, const double py) const;
  const bool is_exclusion_zone(const uint16_t x, const uint16_t y) const;
  const bool is_velocity_visible() const;
  void set_velocity_visible(const bool velocity_visible);
  const bool is_force_field_visible() const;
  void set_force_field_visible(const bool force_field_visible);
  const bool is_ball_visible() const;
  void set_ball_visible(const bool ball_visible);
  void add_field_geometry_listener(IField_geometry_listener *listener);
  virtual void geometry_changed(const uint16_t width, const uint16_t height);
protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
  const Balls *_balls;
  Brush_field *_brush_field;
  Force_field *_force_field;
  QImage *_background;
  bool _velocity_visible;
  bool _force_field_visible;
  bool _ball_visible;
  std::vector<IField_geometry_listener *> *_field_geometry_listeners;
  void check_update_geometry();
  void create_background_normal(const uint16_t width,
                                const uint16_t height,
                                QPainter *painter);
  void create_background_forces(const uint16_t width,
                                const uint16_t height,
                                QPainter *painter);
  void create_background_reflections(const uint16_t width,
                                     const uint16_t height,
                                     QPainter *painter);
  QImage *create_background(const uint16_t width,
                            const uint16_t height);
  void draw_balls(QPainter *painter, const QRect rect);
  void draw_velocities(QPainter *painter, const QRect rect);
};

#endif /* PLAYING_FIELD_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
