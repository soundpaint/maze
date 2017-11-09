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

#ifndef MAZE_CONFIG_HH
#define MAZE_CONFIG_HH

#include <xercesc/dom/DOM.hpp>
#include <QtGui/QBrush>
#include <config.hh>
#include <maze-config-blocks-store.hh>
#include <shape-expression.hh>

class Maze_config : Config
{
public:
  Maze_config(const char *path);
  virtual ~Maze_config();
protected:
  virtual void reload(const xercesc::DOMElement *elem_config);
  virtual void print_config();
private:
  XMLCh *_node_name_any, *_node_name_block, *_node_name_id;
  QBrush _background;
  Maze_config_blocks_store *_blocks;
  void reload_brush(const xercesc::DOMElement *elem_config,
		    QBrush *background);
  void reload_blocks(const xercesc::DOMElement *elem_config);
  Maze_config_block *load_block(const xercesc::DOMElement *elem_block);
  void load_block_id(const XMLCh *attr_id,
                     Maze_config_block *block);
  void load_block_alias(const xercesc::DOMElement *elem_alias,
                        Maze_config_block *block);
  void load_block_shape(const xercesc::DOMElement *elem_shape,
                        Maze_config_block *block);
  Shape_terms *load_shape_expression(const xercesc::DOMElement *elem_expression);
  Shape_factors *load_shape_term(const xercesc::DOMElement *elem_term);
  Shape_unary_expression *
  load_shape_factor(const xercesc::DOMElement *elem_factor);
  Shape_prime *load_prime(const xercesc::DOMElement *elem_prime);
};

#endif /* MAZE_CONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
