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
#include <shape-symbols.hh>
#include <tile-symbols.hh>
#include <shape.hh>
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
  typedef std::unordered_map<const Xml_string *,
                             const Xml_string *,
                             Xml_string::hashing_functor,
                             Xml_string::equal_functor>
  xml_string_to_xml_string_t;
  static const xml_string_to_xml_string_t::size_type BUCKET_COUNT = 5;

  const XMLCh *_node_name_any;
  const XMLCh *_node_name_background;
  const XMLCh *_node_name_columns;
  const XMLCh *_node_name_contents;
  const XMLCh *_node_name_default_background;
  const XMLCh *_node_name_default_foreground;
  const XMLCh *_node_name_field;
  const XMLCh *_node_name_file;
  const XMLCh *_node_name_foreground;
  const XMLCh *_node_name_ignore;
  const XMLCh *_node_name_pixmap;
  const XMLCh *_node_name_rows;
  const XMLCh *_node_name_shape;
  const XMLCh *_node_name_tile;
  const XMLCh *_node_name_tile_shortcut;
  const XMLCh *_attr_name_id;
  const XMLCh *_attr_name_ref;
  Shape_symbols *_shapes;
  Tile_symbols *_tiles;
  QBrush _default_foreground;
  QBrush _default_background;
  Implicit_curve_compiler _implicit_curve_compiler;
  Brush_field *_field;
  static void release(const XMLCh **node_name);
  void reload_brush(const xercesc::DOMElement *elem_brush_ground,
		    QBrush *brush_ground);
  void reload_shapes(const xercesc::DOMElement *elem_config);
  void reload_tiles(const xercesc::DOMElement *elem_config);
  void reload_field(const xercesc::DOMElement *elem_config);
  static const size_t text_content_as_size_t(const xercesc::DOMElement *elem);
  void load_field_ignore_chars(const xercesc::DOMElement *elem_field,
                               std::set<Xml_string> *ignore_chars) const;
  void load_field_tile_shortcuts(const xercesc::DOMElement *elem_field,
                                 xml_string_to_xml_string_t *shortcuts);
  Brush_field *
  load_field_contents(const xercesc::DOMElement *elem_contents,
                      const size_t width, const size_t height,
                      std::set<Xml_string> *ignore_chars,
                      xml_string_to_xml_string_t *shortcuts);
  void load_field(const xercesc::DOMElement *elem_field);
  const Shape *load_shape_definition(const xercesc::DOMElement *elem_shape,
                                     const XMLCh *attr_id) const;
  const Shape *resolve_shape_reference(const xercesc::DOMElement *elem_shape,
                                       const XMLCh *attr_ref) const;
  const Shape *load_shape(const xercesc::DOMElement *elem_shape,
                          const bool require_id);
  const Tile *load_tile(const xercesc::DOMElement *elem_tile);
  const char *load_tile_id(const XMLCh *attr_id);
  const Shape_terms *load_shape_expression(const xercesc::DOMElement *elem_expression,
                                           const bool negated) const;
  Shape_factors *load_shape_term(const xercesc::DOMElement *elem_term) const;
  const Shape_unary_expression *
  load_shape_factor(const xercesc::DOMElement *elem_factor) const;
  const Shape_prime *load_prime(const xercesc::DOMElement *elem_prime,
                                const bool negated) const;
};

#endif /* MAZE_CONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
