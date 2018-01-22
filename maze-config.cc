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

#include <maze-config.hh>
#include <cerrno>
#include <unistd.h>
#include <inttypes.h>
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QScreen>
#include <log.hh>
#include <fractals-brush-factory.hh>
#include <pixmap-brush-factory.hh>
#include <solid-brush-factory.hh>
#include <symbol-table.tcc>

#define CONFIG_SCHEMA_LOCATION "http://soundpaint.org/schema/maze-0.1/config.xsd"

Maze_config::Maze_config(const char *path) :
  Config(path),
  _node_name_any(xercesc::XMLString::transcode("*")),
  _node_name_align(xercesc::XMLString::transcode("align")),
  _node_name_arg_c(xercesc::XMLString::transcode("arg-c")),
  _node_name_arg_n(xercesc::XMLString::transcode("arg-n")),
  _node_name_background(xercesc::XMLString::transcode("background")),
  _node_name_ball(xercesc::XMLString::transcode("ball")),
  _node_name_brush(xercesc::XMLString::transcode("brush")),
  _node_name_column(xercesc::XMLString::transcode("column")),
  _node_name_columns(xercesc::XMLString::transcode("columns")),
  _node_name_contents(xercesc::XMLString::transcode("contents")),
  _node_name_default_background(xercesc::XMLString::transcode("default-background")),
  _node_name_default_foreground(xercesc::XMLString::transcode("default-foreground")),
  _node_name_field(xercesc::XMLString::transcode("field")),
  _node_name_file(xercesc::XMLString::transcode("file")),
  _node_name_foreground(xercesc::XMLString::transcode("foreground")),
  _node_name_fractal(xercesc::XMLString::transcode("fractal")),
  _node_name_ignore(xercesc::XMLString::transcode("ignore")),
  _node_name_imag(xercesc::XMLString::transcode("imag")),
  _node_name_julia(xercesc::XMLString::transcode("julia")),
  _node_name_mandelbrot(xercesc::XMLString::transcode("mandelbrot")),
  _node_name_mass(xercesc::XMLString::transcode("mass")),
  _node_name_max_iterations(xercesc::XMLString::transcode("max-iterations")),
  _node_name_position(xercesc::XMLString::transcode("position")),
  _node_name_real(xercesc::XMLString::transcode("real")),
  _node_name_row(xercesc::XMLString::transcode("row")),
  _node_name_rows(xercesc::XMLString::transcode("rows")),
  _node_name_shape(xercesc::XMLString::transcode("shape")),
  _node_name_solid(xercesc::XMLString::transcode("solid")),
  _node_name_tile(xercesc::XMLString::transcode("tile")),
  _node_name_tile_shortcut(xercesc::XMLString::transcode("tile-shortcut")),
  _node_name_velocity(xercesc::XMLString::transcode("velocity")),
  _node_name_x(xercesc::XMLString::transcode("x")),
  _node_name_x_offset(xercesc::XMLString::transcode("x-offset")),
  _node_name_x_scale(xercesc::XMLString::transcode("x-scale")),
  _node_name_y(xercesc::XMLString::transcode("y")),
  _node_name_y_offset(xercesc::XMLString::transcode("y-offset")),
  _node_name_y_scale(xercesc::XMLString::transcode("y-scale")),
  _attr_name_id(xercesc::XMLString::transcode("id")),
  _attr_name_ref(xercesc::XMLString::transcode("ref")),
  _brush_factories(new Symbol_table<IBrush_factory *>(&SYMBOL_TABLE_ID_BRUSH_FACTORIES)),
  _shapes(new Symbol_table<Shape *>(&SYMBOL_TABLE_ID_SHAPES)),
  _tiles(new Symbol_table<Tile *>(&SYMBOL_TABLE_ID_TILES))
{
  Config::reload();
}

Maze_config::~Maze_config()
{
  delete _shapes;
  _shapes = 0;
  delete _tiles;
  _tiles = 0;
  delete _brush_factories;
  _brush_factories = 0;
  delete _field;
  _field = 0;

  release(&_node_name_any);
  release(&_node_name_align);
  release(&_node_name_arg_c);
  release(&_node_name_arg_n);
  release(&_node_name_background);
  release(&_node_name_ball);
  release(&_node_name_brush);
  release(&_node_name_column);
  release(&_node_name_columns);
  release(&_node_name_contents);
  release(&_node_name_default_background);
  release(&_node_name_default_foreground);
  release(&_node_name_field);
  release(&_node_name_file);
  release(&_node_name_foreground);
  release(&_node_name_fractal);
  release(&_node_name_ignore);
  release(&_node_name_imag);
  release(&_node_name_julia);
  release(&_node_name_mandelbrot);
  release(&_node_name_mass);
  release(&_node_name_max_iterations);
  release(&_node_name_position);
  release(&_node_name_real);
  release(&_node_name_row);
  release(&_node_name_rows);
  release(&_node_name_shape);
  release(&_node_name_solid);
  release(&_node_name_tile);
  release(&_node_name_tile_shortcut);
  release(&_node_name_velocity);
  release(&_node_name_x);
  release(&_node_name_x_offset);
  release(&_node_name_x_scale);
  release(&_node_name_y);
  release(&_node_name_y_offset);
  release(&_node_name_y_scale);
  release(&_attr_name_id);
  release(&_attr_name_ref);
}

const std::string
Maze_config::SYMBOL_TABLE_ID_BRUSH_FACTORIES("brush-factory");

const std::string
Maze_config::SYMBOL_TABLE_ID_SHAPES("shapes");

const std::string
Maze_config::SYMBOL_TABLE_ID_TILES("tiles");

void
Maze_config::release(const XMLCh **node_name)
{
  XMLCh **__node_name = (XMLCh **)node_name;
  xercesc::XMLString::release(__node_name);
  *node_name = 0;
}

void
Maze_config::reload(const xercesc::DOMElement *elem_config)
{
  const XMLCh *node_name = elem_config->getNodeName();
  char *node_name_as_c_star = xercesc::XMLString::transcode(node_name);
  if (strcmp(node_name_as_c_star, "config")) {
    fatal(node_name_as_c_star);
  }
  xercesc::XMLString::release(&node_name_as_c_star);
  node_name_as_c_star = 0;

  reload_brush_factories(elem_config);

  const xercesc::DOMElement *elem_default_background =
    get_single_child_element(elem_config, _node_name_default_background, false);
  if (elem_default_background) {
    const xercesc::DOMElement *elem_brush =
      get_single_child_element(elem_default_background, _node_name_brush);
    if (!elem_brush) {
      fatal("missing brush element");
    }
    _default_background_brush_factory =
      load_brush(elem_brush, false);
  } else {
    _default_background_brush_factory = new Solid_brush_factory(0, Qt::black);
  }

  const xercesc::DOMElement *elem_default_foreground =
    get_single_child_element(elem_config, _node_name_default_foreground, false);
  if (elem_default_foreground) {
    const xercesc::DOMElement *elem_brush =
      get_single_child_element(elem_default_foreground, _node_name_brush);
    if (!elem_brush) {
      fatal("missing brush element");
    }
    _default_foreground_brush_factory =
      load_brush(elem_brush, false);
  } else {
    _default_background_brush_factory = new Solid_brush_factory(0, Qt::white);
  }

  reload_shapes(elem_config);
  reload_tiles(elem_config);
  reload_field(elem_config);
}

const Julia_set *
Maze_config::load_fractal_set_julia(const xercesc::DOMElement *elem_julia) const
{
  uint16_t arg_n;
  const xercesc::DOMElement *elem_arg_n =
    get_single_child_element(elem_julia, _node_name_arg_n, false);
  if (elem_arg_n) {
    const XMLCh *node_value_arg_n = elem_arg_n->getTextContent();
    arg_n = parse_decimal_uint16(node_value_arg_n);
  } else {
    arg_n = 2;
  }

  const xercesc::DOMElement *elem_arg_c =
    get_single_child_element(elem_julia, _node_name_arg_c, true);
  const xercesc::DOMElement *elem_real =
    get_single_child_element(elem_arg_c, _node_name_real, true);
  const XMLCh *node_value_real = elem_real->getTextContent();
  const double real = parse_double(node_value_real);
  const xercesc::DOMElement *elem_imag =
    get_single_child_element(elem_arg_c, _node_name_imag, true);
  const XMLCh *node_value_imag = elem_imag->getTextContent();
  const double imag = parse_double(node_value_imag);

  const IFractal_set::complex_t arg_c = {real, imag};
  const Julia_set *julia_set = new Julia_set(arg_n, arg_c);
  if (!julia_set) {
    fatal("not enough memory");
  }
  return julia_set;
}

const Mandelbrot_set *
Maze_config::load_fractal_set_mandelbrot(const xercesc::DOMElement *elem_mandelbrot) const
{
  const Mandelbrot_set *mandelbrot_set = new Mandelbrot_set();
  if (!mandelbrot_set) {
    fatal("not enough memory");
  }
  return mandelbrot_set;
}

IBrush_factory *
Maze_config::load_brush_fractal(const Xml_string *id,
                                const xercesc::DOMElement *elem_fractal) const
{
  const xercesc::DOMElement *elem_x_offset =
    get_single_child_element(elem_fractal, _node_name_x_offset, true);
  const XMLCh *node_value_x_offset = elem_x_offset->getTextContent();
  const double x_offset = parse_double(node_value_x_offset);
  const xercesc::DOMElement *elem_y_offset =
    get_single_child_element(elem_fractal, _node_name_y_offset, true);
  const XMLCh *node_value_y_offset = elem_y_offset->getTextContent();
  const double y_offset = parse_double(node_value_y_offset);

  const xercesc::DOMElement *elem_x_scale =
    get_single_child_element(elem_fractal, _node_name_x_scale, true);
  const XMLCh *node_value_x_scale = elem_x_scale->getTextContent();
  const double x_scale = parse_double(node_value_x_scale);
  const xercesc::DOMElement *elem_y_scale =
    get_single_child_element(elem_fractal, _node_name_y_scale, true);
  const XMLCh *node_value_y_scale = elem_y_scale->getTextContent();
  const double y_scale = parse_double(node_value_y_scale);

  uint16_t max_iterations;
  const xercesc::DOMElement *elem_max_iterations =
    get_single_child_element(elem_fractal, _node_name_max_iterations, false);
  if (elem_max_iterations) {
    const XMLCh *node_value_max_iterations = elem_max_iterations->getTextContent();
    max_iterations = parse_decimal_uint16(node_value_max_iterations);
  } else {
    max_iterations = 256;
  }

  const xercesc::DOMElement *elem_julia =
    get_single_child_element(elem_fractal, _node_name_julia, false);
  const xercesc::DOMElement *elem_mandelbrot =
    get_single_child_element(elem_fractal, _node_name_mandelbrot, false);
  const uint16_t elems_count =
    (elem_julia ? 1 : 0) + (elem_mandelbrot ? 1 : 0);
  if (elems_count != 1) {
    fatal("fractal definition must contain exactly one of either julia or "
          "mandelbrot definition");
  }

  const IFractal_set *fractal_set = elem_julia ?
    (const IFractal_set *)load_fractal_set_julia(elem_julia) :
    (const IFractal_set *)load_fractal_set_mandelbrot(elem_mandelbrot);

  IBrush_factory *factory = new Fractals_brush_factory(id,
                                                       fractal_set,
                                                       max_iterations,
                                                       x_offset, y_offset,
                                                       x_scale, y_scale);
  if (!factory) {
    fatal("not enough memory");
  }
  return factory;
}

IBrush_factory *
Maze_config::load_brush_file(const Xml_string *id,
                             const xercesc::DOMElement *elem_file) const
{
  const XMLCh *node_value_file = elem_file->getTextContent();
  char *str_file_path = xercesc::XMLString::transcode(node_value_file);
  IBrush_factory *factory = new Pixmap_brush_factory(id, str_file_path);
  if (!factory) {
    fatal("not enough memory");
  }
  xercesc::XMLString::release(&str_file_path);
  return factory;
}

IBrush_factory *
Maze_config::load_brush_solid(const Xml_string *id,
                              const xercesc::DOMElement *elem_solid) const
{
  const XMLCh *node_value_solid = elem_solid->getTextContent();
  char *str_solid_color = xercesc::XMLString::transcode(node_value_solid);
  QColor solid_color(str_solid_color);
  if (!solid_color.isValid()) {
    std::stringstream msg;
    msg << "invalid color specification string: " << str_solid_color;
    xercesc::XMLString::release(&str_solid_color);
    fatal(msg.str());
  }
  xercesc::XMLString::release(&str_solid_color);
  IBrush_factory *factory = new Solid_brush_factory(id, solid_color);
  if (!factory) {
    fatal("not enough memory");
  }
  return factory;
}

IBrush_factory *
Maze_config::load_brush_definition(const xercesc::DOMElement *elem_brush,
                                   const XMLCh *attr_id) const
{
  // symbol definition
  Xml_string *str_id;
  if (attr_id) {
    str_id = new Xml_string(attr_id);
    if (!str_id) {
      fatal("not enough memory");
    }
  } else {
    str_id = 0;
  }

  const xercesc::DOMElement *elem_solid =
    get_single_child_element(elem_brush, _node_name_solid, false);
  const xercesc::DOMElement *elem_file =
    get_single_child_element(elem_brush, _node_name_file, false);
  const xercesc::DOMElement *elem_fractal =
    get_single_child_element(elem_brush, _node_name_fractal, false);
  const uint16_t elems_count =
    (elem_solid ? 1 : 0) + (elem_file ? 1 : 0) + (elem_fractal ? 1 : 0);
  if (elems_count != 1) {
    fatal("brush definition must contain exactly one of either solid or "
          "file or fractal definition");
  }

  IBrush_factory *factory =
    elem_solid ? load_brush_solid(str_id, elem_solid) :
    elem_file ? load_brush_file(str_id, elem_file) :
    load_brush_fractal(str_id, elem_fractal);
  return factory;
}

IBrush_factory *
Maze_config::resolve_brush_reference(const xercesc::DOMElement *elem_brush,
                                     const XMLCh *attr_ref) const
{
  // symbol reference
  Xml_string str_ref(attr_ref);
  IBrush_factory *brush_factory = _brush_factories->lookup(&str_ref);
  if (!brush_factory) {
    std::stringstream msg;
    char *str_ref_as_c_star = str_ref.transcode();
    msg << "can not resolve brush '" << str_ref_as_c_star << "'";
    str_ref.release(&str_ref_as_c_star);
    fatal(msg.str());
  }
  return brush_factory;
}

IBrush_factory *
Maze_config::load_brush(const xercesc::DOMElement *elem_brush,
                        const bool require_definition) const
{
  debug("'brush('");
  if (!elem_brush) {
    fatal("unexpected null element");
  }

  const XMLCh *attr_id = elem_brush->getAttribute(_attr_name_id);

  if (require_definition) {
    if (xercesc::XMLString::stringLen(attr_id) <= 0) {
      fatal("missing or empty 'id' attribute on brush definition");
    }
  }

  const XMLCh *attr_ref = elem_brush->getAttribute(_attr_name_ref);
  IBrush_factory *brush_factory =
    (xercesc::XMLString::stringLen(attr_ref) > 0) ?
    resolve_brush_reference(elem_brush, attr_ref) :
    load_brush_definition(elem_brush, attr_id);
  debug("')'");
  return brush_factory;
}

void
Maze_config::reload_field(const xercesc::DOMElement *elem_config)
{
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_config, _node_name_field);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    if (length != 1) {
      fatal("expected exactly one 'field' element in config file");
    }
    xercesc::DOMNode *node = node_list->item(0);
    xercesc::DOMElement *elem_field =
      dynamic_cast<xercesc::DOMElement *>(node);
    if (!elem_field) {
      fatal("unexpected null element");
    }
    load_field(elem_field);
  }
}

const size_t
Maze_config::text_content_as_size_t(const xercesc::DOMElement *elem)
{
  const XMLCh *node_value = elem->getTextContent();
  char *node_value_as_c_star =
    xercesc::XMLString::transcode(node_value);
  char *endptr;
  const long int value = strtol(node_value_as_c_star, &endptr, 10);
  if (endptr != node_value_as_c_star + strlen(node_value_as_c_star)) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': unexpected characters following integer value at position " <<
      endptr - node_value_as_c_star;
    fatal(msg.str());
  }
  if (errno == ERANGE) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': integer value out of range ";
    fatal(msg.str());
  }
  if (errno == EINVAL) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': either no digits seen or " <<
      "programming error: strtol base out of range";
    fatal(msg.str());
  }
  xercesc::XMLString::release(&node_value_as_c_star);
  node_value_as_c_star = 0;
  return value;
}

const double
Maze_config::text_content_as_double(const xercesc::DOMElement *elem)
{
  const XMLCh *node_value = elem->getTextContent();
  char *node_value_as_c_star =
    xercesc::XMLString::transcode(node_value);
  char *endptr;
  const double value = strtod(node_value_as_c_star, &endptr);
  if (endptr != node_value_as_c_star + strlen(node_value_as_c_star)) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': unexpected characters following double value at position " <<
      endptr - node_value_as_c_star;
    fatal(msg.str());
  }
  if (errno == ERANGE) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': double value out of range ";
    fatal(msg.str());
  }
  if (errno == EINVAL) {
    std::stringstream msg;
    msg << "in text value of node '" << elem->getNodeName() <<
      "': either no digits seen or " <<
      "programming error: strtod base out of range";
    fatal(msg.str());
  }
  xercesc::XMLString::release(&node_value_as_c_star);
  node_value_as_c_star = 0;
  return value;
}

void
Maze_config::load_field_ignore_chars(const xercesc::DOMElement *elem_field,
                                     std::set<Xml_string> *chars) const
{
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_field, _node_name_ignore);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_ignore =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_ignore) {
        fatal("unexpected null element");
      }
      const XMLCh *node_value_ignore = elem_ignore->getTextContent();
      const Xml_string str_ignore(node_value_ignore);
      char *node_value_ignore_as_c_star = str_ignore.transcode();
      if (str_ignore.length() != 1) {
        std::stringstream msg;
        msg << "in text value of node '" << elem_ignore->getNodeName() <<
          "': ignore value must have exactly 1 character, but has " <<
          str_ignore.length() << " characters: '" <<
          node_value_ignore_as_c_star << "'";
        xercesc::XMLString::release(&node_value_ignore_as_c_star);
        fatal(msg.str());
      }
      chars->insert(str_ignore);
      xercesc::XMLString::release(&node_value_ignore_as_c_star);
    }
    std::stringstream msg;
    msg << "found brush field ignore characters:" << std::endl;
    for (const Xml_string &str_ch : *chars) {
      char *str_ch_as_c_star = str_ch.transcode();
      msg << "ignore character '" << str_ch_as_c_star << "'" << std::endl;
      str_ch.release(&str_ch_as_c_star);
    }
    debug(msg.str());
  } else {
    // no ignores => nothing to parse
  }
}

void
Maze_config::load_field_tile_shortcut(const xercesc::DOMElement *elem_tile_shortcut,
                                      xml_string_to_xml_string_t *shortcuts) const
{
  const XMLCh *attr_id = elem_tile_shortcut->getAttribute(_attr_name_id);
  if (xercesc::XMLString::stringLen(attr_id) <= 0) {
    fatal("missing or empty 'id' attribute on tile shortcut");
  }
  const Xml_string *str_id = new Xml_string(attr_id);
  if (!str_id) {
    fatal("not enough memory");
  }
  char *attr_value_id_as_c_star = str_id->transcode();
  if (str_id->length() != 1) {
    std::stringstream msg;
    msg << "in tile-shortcut node '" << elem_tile_shortcut->getNodeName() <<
      "': 'id' attribute value must have exactly 1 character, but has " <<
      str_id->length() << " characters: '" <<
      attr_value_id_as_c_star << "'";
    xercesc::XMLString::release(&attr_value_id_as_c_star);
    fatal(msg.str());
  }
  {
    std::stringstream msg;
    msg << "found shortcut: '" << attr_value_id_as_c_star << "'";
    debug(msg.str());
  }

  if (shortcuts->count(str_id) > 0) {
    std::stringstream msg;
    msg << "in node '" << elem_tile_shortcut->getNodeName() <<
      "': multiple declarations of shortcut with id='" <<
      attr_value_id_as_c_star << "'";
    xercesc::XMLString::release(&attr_value_id_as_c_star);
    fatal(msg.str());
  }
  xercesc::XMLString::release(&attr_value_id_as_c_star);

  const XMLCh *attr_ref = elem_tile_shortcut->getAttribute(_attr_name_ref);
  if (xercesc::XMLString::stringLen(attr_ref) <= 0) {
    fatal("missing or empty 'ref' attribute on tile shortcut");
  }
  const Xml_string *str_ref = new Xml_string(attr_ref);
  if (!str_ref) {
    fatal("not enough memory");
  }
  (*shortcuts)[str_id] = str_ref;
}

void
Maze_config::load_field_tile_shortcuts(const xercesc::DOMElement *elem_field,
                                       xml_string_to_xml_string_t *shortcuts) const
{
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_field, _node_name_tile_shortcut);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_tile_shortcut =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_tile_shortcut) {
        fatal("unexpected null element");
      }
      load_field_tile_shortcut(elem_tile_shortcut, shortcuts);
    }
    {
      std::stringstream msg;
      msg << "found tile shortcuts:" << std::endl;
      for (const auto& entry : *shortcuts) {
        const Xml_string *id = entry.first;
        const Xml_string *ref = entry.second;
        char *str_id = id->transcode();
        char *str_ref = ref->transcode();
        msg << "'" << str_id << "' => '" << str_ref << "'" << std::endl;
        id->release(&str_id);
        ref->release(&str_id);
      }
      debug(msg.str());
    }
  } else {
    Log::warn("no tile shortcut found");
  }
}

Brush_field *
Maze_config::load_field_contents(const xercesc::DOMElement *elem_contents,
                                 const size_t columns, const size_t rows,
                                 std::set<Xml_string> *ignore_chars,
                                 xml_string_to_xml_string_t *shortcuts,
                                 std::vector<const Ball_init_data *> *balls)
{
  const XMLCh *node_value_contents = elem_contents->getTextContent();
  char *node_value_contents_as_c_star =
    xercesc::XMLString::transcode(node_value_contents);

  std::vector<Tile *> field;
  int elems = 0;
  for (XMLSize_t i = 0;
       i < xercesc::XMLString::stringLen(node_value_contents); i++) {
    XMLCh alias_id = node_value_contents[i];
    XMLCh alias_id_star[2] = { alias_id, 0 };

    const Xml_string str_alias_id(alias_id_star);
    const auto search = ignore_chars->find(str_alias_id);
    if (search == ignore_chars->end()) {
      if (shortcuts->count(&str_alias_id) <= 0) {
        std::stringstream msg;
        char *alias_id_as_c_star = str_alias_id.transcode();
        msg << "in field contents: unresolved alias character: '" <<
          alias_id_as_c_star << "' at position " << i << "; contents:" <<
          std::endl << node_value_contents_as_c_star << std::endl;
        str_alias_id.release(&alias_id_as_c_star);
        fatal(msg.str());
      }
      const Xml_string *str_tile_id = (*shortcuts)[&str_alias_id];
      if (_tiles->exists(str_tile_id) <= 0) {
        std::stringstream msg;
        char *tile_id_as_c_star = str_tile_id->transcode();
        msg << "in field contents: unresolved tile id: '" <<
          tile_id_as_c_star << "' at position " << i << "; contents:" <<
          std::endl << node_value_contents_as_c_star << std::endl;
        str_tile_id->release(&tile_id_as_c_star);
        fatal(msg.str());
      }
      Tile *tile = _tiles->lookup(str_tile_id);
      field.push_back(tile);
    } else {
      // alias_id is in set of characters to be ignored
    }
  }
  if (field.size() != (columns * rows)) {
    std::stringstream msg;
    msg << "field dimensions mismatch: expected " << columns << "×" << rows <<
      "=" << (columns * rows) << " elements, but found " << field.size() <<
      " unignorable fields in this contents: '" <<
      node_value_contents_as_c_star << "'";
    fatal(msg.str());
  }

  xercesc::XMLString::release(&node_value_contents_as_c_star);
  node_value_contents_as_c_star = 0;

  Brush_field *brush_field = new Brush_field(columns, rows, field, *balls);
  if (!brush_field) {
    fatal("not enough memory");
  }
  return brush_field;
}

const double
Maze_config::load_field_ball_align(const xercesc::DOMElement *elem_align,
                                   const XMLCh *_node_name_axis,
                                   const double default_align) const
{
  if (elem_align) {
    const xercesc::DOMElement *elem_align_axis =
      get_single_child_element(elem_align, _node_name_axis, false);
    double align;
    if (elem_align_axis) {
      align = text_content_as_double(elem_align_axis);
    } else {
      align = default_align;
    }
    if (align < 0.0) {
      std::stringstream msg;
      msg << "field ball align " << elem_align_axis->getNodeName() <<
        ": align value must be positive";
      fatal(msg.str());
    }
    if (align > 1.0) {
      std::stringstream msg;
      msg << "field ball align " << elem_align_axis->getNodeName() <<
        ": align value must be not greater than 1.0";
      fatal(msg.str());
    }
    return align;
  } else {
    return default_align;
  }
}

void
Maze_config::load_field_ball(const xercesc::DOMElement *elem_ball,
                             std::vector<const Ball_init_data *> *balls) const
{
  const xercesc::DOMElement *elem_position =
    get_single_child_element(elem_ball, _node_name_position, true);

  const xercesc::DOMElement *elem_column =
    get_single_child_element(elem_position, _node_name_column, true);
  const uint16_t column = text_content_as_size_t(elem_column);

  const xercesc::DOMElement *elem_row =
    get_single_child_element(elem_position, _node_name_row, true);
  const uint16_t row = text_content_as_size_t(elem_row);

  const xercesc::DOMElement *elem_align =
    get_single_child_element(elem_ball, _node_name_align, false);
  const double default_align_x = 0.5;
  const double align_x = load_field_ball_align(elem_align, _node_name_x,
                                               default_align_x);
  const double default_align_y = 0.5;
  const double align_y = load_field_ball_align(elem_align, _node_name_y,
                                               default_align_y);

  const xercesc::DOMElement *elem_velocity =
    get_single_child_element(elem_ball, _node_name_velocity, true);

  const xercesc::DOMElement *elem_velocity_x =
    get_single_child_element(elem_velocity, _node_name_x, true);
  const double velocity_x = text_content_as_double(elem_velocity_x);

  const xercesc::DOMElement *elem_velocity_y =
    get_single_child_element(elem_velocity, _node_name_y, true);
  const double velocity_y = text_content_as_double(elem_velocity_y);

  const xercesc::DOMElement *elem_mass =
    get_single_child_element(elem_ball, _node_name_mass, true);
  const double mass = text_content_as_double(elem_mass);

  const Ball_init_data *ball = new Ball_init_data(column, row,
                                                  align_x, align_y,
                                                  velocity_x, velocity_y,
                                                  mass);
  if (!ball) {
    fatal("not enough memory");
  }
  balls->push_back(ball);
}

void
Maze_config::load_field_balls(const xercesc::DOMElement *elem_field,
                              std::vector<const Ball_init_data *> *balls) const
{
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_field, _node_name_ball);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_ball =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_ball) {
        fatal("unexpected null element");
      }
      load_field_ball(elem_ball, balls);
    }
    {
      std::stringstream msg;
      msg << "found " << balls->size() << " ball(s):" << std::endl;
      for (const Ball_init_data *ball : *balls) {
        const uint16_t row = ball->get_row();
        const uint16_t column = ball->get_column();
        const double velocity_x = ball->get_velocity_x();
        const double velocity_y = ball->get_velocity_y();
        const double mass = ball->get_mass();
        msg << "ball{pos=(row=" << row << ", col=" << column << ")), " <<
          "velocity=(x=" << velocity_x << ", y=" << velocity_y << ")), " <<
          "mass=" << mass << "}" << std::endl;
      }
      debug(msg.str());
    }
  } else {
    Log::warn("no ball found");
  }
}

void
Maze_config::load_field(const xercesc::DOMElement *elem_field)
{
  debug("'field('");

  std::vector<const Ball_init_data *> balls;
  load_field_balls(elem_field, &balls);

  const xercesc::DOMElement *elem_columns =
    get_single_child_element(elem_field, _node_name_columns, true);
  const size_t columns = text_content_as_size_t(elem_columns);

  const xercesc::DOMElement *elem_rows =
    get_single_child_element(elem_field, _node_name_rows, true);
  const size_t rows = text_content_as_size_t(elem_rows);

  std::set<Xml_string> ignore_chars;
  load_field_ignore_chars(elem_field, &ignore_chars);

  xml_string_to_xml_string_t shortcuts(BUCKET_COUNT,
                                       Xml_string::hashing_functor(),
                                       Xml_string::equal_functor());
  load_field_tile_shortcuts(elem_field, &shortcuts);

  {
    std::stringstream msg;
    msg << "building playing field [" << columns << "×" << rows << "]";
    debug(msg.str());
  }

  const xercesc::DOMElement *elem_contents =
    get_single_child_element(elem_field, _node_name_contents, true);
  _field =
    load_field_contents(elem_contents, columns, rows,
                        &ignore_chars, &shortcuts, &balls);
  debug("')'");
}

Brush_field *
Maze_config::get_brush_field() const
{
  return _field;
}

void
Maze_config::reload_shapes(const xercesc::DOMElement *elem_config)
{
  debug("'shapes('");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_config, _node_name_shape);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_shape =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_shape) {
        fatal("unexpected null element");
      }
      Shape *shape = load_shape(elem_shape, true);
      const Xml_string *id = shape->get_id();
      _shapes->add(id, shape);
    }
  } else {
    // no shapes => nothing to parse
  }
  debug("')'");
}

Shape *
Maze_config::load_shape_definition(const xercesc::DOMElement *elem_shape,
                                   const XMLCh *attr_id) const
{
  // symbol definition
  Xml_string *str_id;
  if (attr_id) {
    str_id = new Xml_string(attr_id);
    if (!str_id) {
      fatal("not enough memory");
    }
  } else {
    str_id = 0;
  }

  if (elem_shape->getChildElementCount() != 1) {
    fatal("'shape' element for shape definition must contain exactly "
          "one child element");
  }
  const xercesc::DOMElement *elem_expression =
    elem_shape->getFirstElementChild();

  const Shape_terms *shape_terms =
    load_shape_expression(elem_expression, false);
  Shape *shape = new Shape(str_id, shape_terms);
  if (!shape) {
    fatal("not enough memory");
  }
  return shape;
}

Shape *
Maze_config::resolve_shape_reference(const xercesc::DOMElement *elem_shape,
                                     const XMLCh *attr_ref) const
{
  // symbol reference
  Xml_string str_ref(attr_ref);
  Shape *shape = _shapes->lookup(&str_ref);
  if (!shape) {
    std::stringstream msg;
    char *str_ref_as_c_star = str_ref.transcode();
    msg << "can not resolve shape '" << str_ref_as_c_star << "'";
    str_ref.release(&str_ref_as_c_star);
    fatal(msg.str());
  }
  return shape;
}

Shape *
Maze_config::load_shape(const xercesc::DOMElement *elem_shape,
                        const bool require_definition)
{
  debug("'shape('");
  if (!elem_shape) {
    fatal("missing shape element");
  }

  const XMLCh *attr_id = elem_shape->getAttribute(_attr_name_id);

  if (require_definition) {
    if (xercesc::XMLString::stringLen(attr_id) <= 0) {
      fatal("missing or empty 'id' attribute on shape definition");
    }
  }

  const XMLCh *attr_ref = elem_shape->getAttribute(_attr_name_ref);
  Shape *shape =
    (xercesc::XMLString::stringLen(attr_ref) > 0) ?
    resolve_shape_reference(elem_shape, attr_ref) :
    load_shape_definition(elem_shape, attr_id);
  debug("')'");
  return shape;
}

void
Maze_config::reload_brush_factories(const xercesc::DOMElement *elem_config)
{
  debug("'brushes('");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_config, _node_name_brush);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_brush =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_brush) {
        fatal("unexpected null element");
      }
      IBrush_factory *brush_factory = load_brush(elem_brush, true);
      const Xml_string *id = brush_factory->get_id();
      _brush_factories->add(id, brush_factory);
    }
  } else {
    // no brush factories => nothing to parse
  }
  debug("')'");
}

void
Maze_config::reload_tiles(const xercesc::DOMElement *elem_config)
{
  debug("'tiles('");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_config, _node_name_tile);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_tile =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_tile) {
        fatal("unexpected null element");
      }
      Tile *tile = load_tile(elem_tile);
      const Xml_string *id = tile->get_id();
      _tiles->add(id, tile);
    }
  } else {
    // no tiles => nothing to parse
  }
  debug("')'");
}

Tile *
Maze_config::load_tile(const xercesc::DOMElement *elem_tile)
{
  debug("'tile('");
  if (!elem_tile) {
    fatal("unexpected null element");
  }

  const XMLCh *attr_id = elem_tile->getAttribute(_attr_name_id);
  if (xercesc::XMLString::stringLen(attr_id) <= 0) {
    fatal("missing or empty 'id' attribute on tile");
  }
  const Xml_string *str_id = new Xml_string(attr_id);
  if (!str_id) {
    fatal("not enough memory");
  }

  /*
  const char *id = attr_id ? load_tile_id(attr_id) : 0;
  {
    std::stringstream msg;
    msg << "id='" << id << "'";
    debug(msg.str());
  }
  */

  IBrush_factory *foreground_brush_factory;
  const xercesc::DOMElement *elem_foreground =
    get_single_child_element(elem_tile, _node_name_foreground);
  if (elem_foreground) {
    const xercesc::DOMElement *elem_brush =
      get_single_child_element(elem_foreground, _node_name_brush);
    if (!elem_brush) {
      fatal("missing brush element");
    }
    foreground_brush_factory = load_brush(elem_brush, false);
  } else {
    std::stringstream msg;
    msg << "no foreground defined for tile '" << str_id <<
      "': falling back to default foreground";
    Log::info(msg.str());
    foreground_brush_factory = _default_foreground_brush_factory;
  }

  IBrush_factory *background_brush_factory;
  const xercesc::DOMElement *elem_background =
    get_single_child_element(elem_tile, _node_name_background);
  if (elem_background) {
    const xercesc::DOMElement *elem_brush =
      get_single_child_element(elem_background, _node_name_brush);
    if (!elem_brush) {
      fatal("missing brush element");
    }
    background_brush_factory = load_brush(elem_brush, false);
  } else {
    std::stringstream msg;
    msg << "no background defined for tile '" << str_id <<
      "': falling back to default background";
    Log::info(msg.str());
    background_brush_factory = _default_background_brush_factory;
  }

  const xercesc::DOMElement *elem_shape =
    get_single_child_element(elem_tile, _node_name_shape, true);
  if (!elem_shape) {
    fatal("for now, tile definition must contain shape definition");
  }
  Shape *shape = load_shape(elem_shape, false);

  const double foreground_potential = 0.0; // TODO
  const double background_potential = 0.0; // TODO

  // FIXME: Memory leak: Currently, nobody feels responsible
  // for deleting foreground_brush_factory and
  // background_brush_factory when no more used.
  Tile *tile =
    new Tile(str_id,
             foreground_brush_factory,
             background_brush_factory,
             foreground_potential,
             background_potential,
             shape);
  if (!tile) {
    fatal("not enough memory");
  }

  debug("')'");

  return tile;
}

const char *
Maze_config::load_tile_id(const XMLCh *attr_id)
{
  debug("'id('");
  if (!attr_id) {
    fatal("unexpected null attribute");
  }
  char *attr_id_as_c_star = xercesc::XMLString::transcode(attr_id);
  char *id = strdup(attr_id_as_c_star);
  xercesc::XMLString::release(&attr_id_as_c_star);
  attr_id_as_c_star = 0;
  if (!id) {
    fatal("not enough memory");
  }
  debug(id);
  debug("')'");
  return id;
}

const Shape_terms *
Maze_config::load_shape_expression(const xercesc::DOMElement *elem_expression,
                                   const bool negated) const
{
  // TODO: For optimzation (fast evaluation of subsequent shape inside
  // / outside tests), convert shape expression into minimal form.
  debug("'shape('");
  if (!elem_expression) {
    fatal("unexpected null element");
  }
  Shape_terms *expression = new Shape_terms();
  if (!expression) {
    fatal("not enough memory");
  }
  const XMLCh *node_name = elem_expression->getNodeName();
  char *node_name_as_c_star = xercesc::XMLString::transcode(node_name);
  Shape_factors *term;
  if (!strcmp(node_name_as_c_star, "or")) {
    debug("'or('");
    const xercesc::DOMNodeList *node_list =
      get_children_by_tag_name(elem_expression, _node_name_any);
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_term =
        dynamic_cast<xercesc::DOMElement *>(node);
      term = load_shape_term(elem_term);
      expression->add_term(term);
    }
    debug("')'");
  } else {
    term = load_shape_term(elem_expression);
    expression->add_term(term);
  }
  xercesc::XMLString::release(&node_name_as_c_star);
  node_name_as_c_star = 0;
  debug("')'");
  if (negated)
    expression->set_negated(true);
  return expression;
}

Shape_factors *
Maze_config::load_shape_term(const xercesc::DOMElement *elem_term) const
{
  if (!elem_term) {
    fatal("unexpected null element");
  }
  Shape_factors *term = new Shape_factors();
  if (!term) {
    fatal("not enough memory");
  }
  const XMLCh *node_name = elem_term->getNodeName();
  char *node_name_as_c_star = xercesc::XMLString::transcode(node_name);
  const Shape_unary_expression *factor;
  if (!strcmp(node_name_as_c_star, "and")) {
    debug("'and('");
    const xercesc::DOMNodeList *node_list =
      get_children_by_tag_name(elem_term, _node_name_any);
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_factor =
        dynamic_cast<xercesc::DOMElement *>(node);
      factor = load_shape_factor(elem_factor);
      term->add_factor(factor);
    }
    debug("')'");
  } else {
    factor = load_shape_factor(elem_term);
    term->add_factor(factor);
  }
  xercesc::XMLString::release(&node_name_as_c_star);
  node_name_as_c_star = 0;
  return term;
}

const Shape_unary_expression *
Maze_config::load_shape_factor(const xercesc::DOMElement *elem_factor) const
{
  if (!elem_factor) {
    fatal("unexpected null element");
  }
  bool negated;
  const XMLCh *node_name_unary_expression;
  char *node_name_unary_expression_as_c_star;
  const XMLCh *node_name_factor = elem_factor->getNodeName();
  char *node_name_factor_as_c_star =
    xercesc::XMLString::transcode(node_name_factor);
  const xercesc::DOMElement *elem_unary_expression;
  if (!strcmp(node_name_factor_as_c_star, "not")) {
    if (elem_factor->getChildElementCount() != 1) {
      fatal("'not' element must contain exactly one child element");
    }
    elem_unary_expression = elem_factor->getFirstElementChild();
    negated = true;
    node_name_unary_expression = elem_unary_expression->getNodeName();
    node_name_unary_expression_as_c_star =
      xercesc::XMLString::transcode(node_name_unary_expression);
    xercesc::XMLString::release(&node_name_factor_as_c_star);
  } else {
    elem_unary_expression = elem_factor;
    negated = false;
    node_name_unary_expression = node_name_factor;
    node_name_unary_expression_as_c_star = node_name_factor_as_c_star;
  }
  node_name_factor_as_c_star = 0;

  if (negated) {
    debug("'not('");
  }
  const Shape_unary_expression *unary_expression =
  !strcmp(node_name_unary_expression_as_c_star, "implicit-curve") ?
    (const Shape_unary_expression *)load_prime(elem_unary_expression, negated) :
    (const Shape_unary_expression *)load_shape_expression(elem_unary_expression, negated);
  if (negated) {
    debug("')'");
  }
  xercesc::XMLString::release(&node_name_unary_expression_as_c_star);
  node_name_unary_expression_as_c_star = 0;

  return unary_expression;
}

const Shape_prime *
Maze_config::load_prime(const xercesc::DOMElement *elem_prime,
                        const bool negated) const
{
  debug("'implicit_curve('");
  if (!elem_prime) {
    fatal("unexpected null element");
  }
  const XMLCh *node_value_prime = elem_prime->getTextContent();
  char *str_prime = xercesc::XMLString::transcode(node_value_prime);
  if (!str_prime) {
    fatal("not enough memory");
  }
  const Implicit_curve *implicit_curve =
    _implicit_curve_compiler.compile(str_prime);
  if (!implicit_curve) {
    fatal("not enough memory");
  }
  xercesc::XMLString::release(&str_prime);
  {
    std::stringstream msg;
    msg << "normalized implicit curve: ";
    msg << implicit_curve->to_string();
    debug(msg.str());
  }

  const Shape_prime *prime = new Shape_prime(implicit_curve, negated);
  if (!prime) {
    fatal("not enough memory");
  }
  debug("')'");
  return prime;
}

void
Maze_config::print_config()
{
  std::stringstream msg;
  msg << std::endl << "######## CONFIG ########" << std::endl;
  // TODO
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
