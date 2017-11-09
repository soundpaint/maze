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

Maze_config_block::Maze_config_block()
{
  _terms = 0;
}

Maze_config_block::~Maze_config_block()
{
  free(_id);
  _id = 0;
  free(_alias);
  _alias = 0;
  delete _terms;
  _terms = 0;
}

void
Maze_config_block::set_id(char *id)
{
  _id = id;
}

const char *
Maze_config_block::get_id()
{
  return _id;
}

void
Maze_config_block::set_alias(char *alias)
{
  _alias = alias;
}

const char *
Maze_config_block::get_alias()
{
  return _alias;
}

void
Maze_config_block::set_foreground(QBrush foreground)
{
  _foreground = foreground;
}

QBrush
Maze_config_block::get_foreground()
{
  return _foreground;
}

void
Maze_config_block::set_background(QBrush background)
{
  _background = background;
}

QBrush
Maze_config_block::get_background()
{
  return _background;
}

void
Maze_config_block::set_terms(Shape_terms *terms)
{
  _terms = terms;
}

Shape_terms *
Maze_config_block::get_terms()
{
  return _terms;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
