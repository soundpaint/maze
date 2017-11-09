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

#ifndef MAZE_CONFIG_BLOCK_HH
#define MAZE_CONFIG_BLOCK_HH

#include <QtGui/QBrush>
#include <shape-expression.hh>

class Maze_config_block
{
public:
  Maze_config_block();
  virtual ~Maze_config_block();
  void set_id(char *id);
  const char *get_id();
  void set_alias(char *alias);
  const char *get_alias();
  void set_foreground(QBrush foreground);
  QBrush get_foreground();
  void set_background(QBrush background);
  QBrush get_background();
  void set_terms(Shape_terms *terms);
  Shape_terms *get_terms();
private:
  char *_id;
  char *_alias;
  QBrush _foreground;
  QBrush _background;
  Shape_terms *_terms;
};

#endif /* MAZE_CONFIG_BLOCK_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
