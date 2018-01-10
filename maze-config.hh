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

#ifndef MAZE_CONFIG_HH
#define MAZE_CONFIG_HH

#include <set>
#include <xercesc/dom/DOM.hpp>
#include <QtGui/QBrush>
#include <ball-init-data.hh>
#include <brush-field.hh>
#include <brush-factory-symbols.hh>
#include <config.hh>
#include <implicit-curve-compiler.hh>
#include <shape-symbols.hh>
#include <tile-symbols.hh>

class Maze_config : Config
{
public:
  Maze_config(const char *path);
  virtual ~Maze_config();
  Brush_field *get_brush_field() const;
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
  const XMLCh *_node_name_align;
  const XMLCh *_node_name_background;
  const XMLCh *_node_name_ball;
  const XMLCh *_node_name_brush;
  const XMLCh *_node_name_column;
  const XMLCh *_node_name_columns;
  const XMLCh *_node_name_contents;
  const XMLCh *_node_name_default_background;
  const XMLCh *_node_name_default_foreground;
  const XMLCh *_node_name_field;
  const XMLCh *_node_name_file;
  const XMLCh *_node_name_foreground;
  const XMLCh *_node_name_fractal;
  const XMLCh *_node_name_ignore;
  const XMLCh *_node_name_mass;
  const XMLCh *_node_name_position;
  const XMLCh *_node_name_row;
  const XMLCh *_node_name_rows;
  const XMLCh *_node_name_shape;
  const XMLCh *_node_name_solid;
  const XMLCh *_node_name_tile;
  const XMLCh *_node_name_tile_shortcut;
  const XMLCh *_node_name_velocity;
  const XMLCh *_node_name_x;
  const XMLCh *_node_name_x_offset;
  const XMLCh *_node_name_x_scale;
  const XMLCh *_node_name_y;
  const XMLCh *_node_name_y_offset;
  const XMLCh *_node_name_y_scale;
  const XMLCh *_attr_name_id;
  const XMLCh *_attr_name_ref;
  Brush_factory_symbols *_brush_factories;
  Shape_symbols *_shapes;
  Tile_symbols *_tiles;
  IBrush_factory *_default_foreground_brush_factory;
  IBrush_factory *_default_background_brush_factory;
  Implicit_curve_compiler _implicit_curve_compiler;
  Brush_field *_field;
  static void release(const XMLCh **node_name);
  void reload_brush_factories(const xercesc::DOMElement *elem_config);
  void reload_shapes(const xercesc::DOMElement *elem_config);
  void reload_tiles(const xercesc::DOMElement *elem_config);
  void reload_field(const xercesc::DOMElement *elem_config);
  static const size_t text_content_as_size_t(const xercesc::DOMElement *elem);
  static const double text_content_as_double(const xercesc::DOMElement *elem);
  IBrush_factory *load_brush_fractal(const Xml_string *id,
                                     const xercesc::DOMElement *elem_fractal) const;
  IBrush_factory *load_brush_file(const Xml_string *id,
                                  const xercesc::DOMElement *elem_file) const;
  IBrush_factory *load_brush_solid(const Xml_string *id,
                                   const xercesc::DOMElement *elem_solid) const;
  IBrush_factory *load_brush_definition(const xercesc::DOMElement *elem_brush,
                                        const XMLCh *attr_id) const;
  IBrush_factory *resolve_brush_reference(const xercesc::DOMElement *elem_brush,
                                          const XMLCh *attr_ref) const;
  IBrush_factory *load_brush(const xercesc::DOMElement *elem_brush_ground,
                             const bool require_definition) const;
  void load_field_ignore_chars(const xercesc::DOMElement *elem_field,
                               std::set<Xml_string> *ignore_chars) const;
  void load_field_tile_shortcut(const xercesc::DOMElement *elem_tile_shortcut,
                                xml_string_to_xml_string_t *shortcuts) const;
  void load_field_tile_shortcuts(const xercesc::DOMElement *elem_field,
                                 xml_string_to_xml_string_t *shortcuts) const;
  Brush_field *
  load_field_contents(const xercesc::DOMElement *elem_contents,
                      const size_t columns, const size_t rows,
                      std::set<Xml_string> *ignore_chars,
                      xml_string_to_xml_string_t *shortcuts,
                      std::vector<const Ball_init_data *> *balls);
  const double load_field_ball_align(const xercesc::DOMElement *elem_align,
                                     const XMLCh *_node_name_axis,
                                     const double default_align) const;
  void load_field_ball(const xercesc::DOMElement *elem_ball,
                       std::vector<const Ball_init_data *> *balls) const;
  void load_field_balls(const xercesc::DOMElement *elem_field,
                        std::vector<const Ball_init_data *> *balls) const;
  void load_field(const xercesc::DOMElement *elem_field);
  const Shape *load_shape_definition(const xercesc::DOMElement *elem_shape,
                                     const XMLCh *attr_id) const;
  const Shape *resolve_shape_reference(const xercesc::DOMElement *elem_shape,
                                       const XMLCh *attr_ref) const;
  const Shape *load_shape(const xercesc::DOMElement *elem_shape,
                          const bool require_id);
  Tile *load_tile(const xercesc::DOMElement *elem_tile);
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
