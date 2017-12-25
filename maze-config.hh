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

#include <set>
#include <xercesc/dom/DOM.hpp>
#include <QtGui/QBrush>
#include <config.hh>
#include <xml-string.hh>
#include <tiles-store.hh>
#include <shape-expression.hh>
#include <implicit-curve-compiler.hh>
#include <brush-field.hh>

class Maze_config : Config
{
public:
  Maze_config(const char *path);
  virtual ~Maze_config();
  const Brush_field *get_brush_field() const;
protected:
  virtual void reload(const xercesc::DOMElement *elem_config);
  virtual void print_config();
private:
  XMLCh *_node_name_any;
  XMLCh *_node_name_ignore;
  XMLCh *_node_name_field;
  XMLCh *_node_name_tile;
  XMLCh *_node_name_id;
  QBrush _background;
  Implicit_curve_compiler _implicit_curve_compiler;
  Tiles_store *_tiles;
  Brush_field *_field;
  void reload_brush(const xercesc::DOMElement *elem_background,
		    QBrush *background);
  void reload_tiles(const xercesc::DOMElement *elem_config);
  void reload_field(const xercesc::DOMElement *elem_config);
  static const size_t text_content_as_size_t(const xercesc::DOMElement *elem);
  void load_field_ignore_chars(const xercesc::DOMElement *elem_field,
                               std::set<Xml_string> *chars) const;
  void load_field(const xercesc::DOMElement *elem_field);
  Tile *load_tile(const xercesc::DOMElement *elem_tile);
  const char *load_tile_id(const XMLCh *attr_id);
  const Xml_string *load_tile_alias_char(const xercesc::DOMElement *elem_alias_char);
  Shape_terms *load_tile_shape(const xercesc::DOMElement *elem_shape);
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
