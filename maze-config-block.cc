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

#include <maze-config-block.hh>
#include <stdlib.h>
#include <log.hh>

Maze_config_block::Maze_config_block(const Xml_string *id,
                                     const Xml_string *alias_char,
                                     const QBrush foreground,
                                     const QBrush background,
                                     const double foreground_potential,
                                     const double background_potential,
                                     const Shape_terms *shape_expression) :
  _id(id),
  _alias_char(alias_char),
  _foreground(QBrush(foreground)),
  _background(QBrush(background)),
  _foreground_potential(foreground_potential),
  _background_potential(background_potential),
  _shape_expression(shape_expression)
{
  if (!id) {
    Log::fatal("id is null");
  }
  if (!shape_expression) {
    Log::fatal("shape_expression is null");
  }
}

Maze_config_block::~Maze_config_block()
{
  //free(_id); // TODO
  _id = 0;
  delete _shape_expression;
  _shape_expression = 0;
}

const std::string
Maze_config_block::to_string() const
{
  std::stringstream str;
  str << "Maze_config_block{";
  str << "id=" << _id;
  str << ", alias_char=" << _alias_char;
  str << ", foreground=" << &_foreground;
  str << ", background=" << &_background;
  str << ", foreground_potential=" << _foreground_potential;
  str << ", background_potential=" << _background_potential;
  str << ", shape_expression=" << _shape_expression;
  str <<"}";
  return std::string(str.str());
}

const double
Maze_config_block::get_potential(const double x, const double y) const
{
  if (_shape_expression->is_inside(x, y)) {
    return _foreground_potential;
  } else {
    return _background_potential;
  }
}

const QBrush *
Maze_config_block::get_brush(const double x, const double y) const
{
  const QBrush *brush;
  if (_shape_expression->is_inside(x, y)) {
    brush = &_foreground;
  } else {
    brush = &_background;
  }
return brush;
}

const double
Maze_config_block::get_avg_tan(const double block_offset_x,
                               const double block_offset_y,
                               const double dx,
                               const double dy) const
{
  // TODO
  return 0.0;
}

const Xml_string *
Maze_config_block::get_id() const
{
  return _id;
}

const Xml_string *
Maze_config_block::get_alias_char() const
{
  return _alias_char;
}

const QBrush
Maze_config_block::get_foreground() const
{
  return _foreground;
}

const QBrush
Maze_config_block::get_background() const
{
  return _background;
}

const Shape_terms *
Maze_config_block::get_terms() const
{
  return _shape_expression;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
