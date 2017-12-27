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

#include <maze-config.hh>
#include <cerrno>
#include <unistd.h>
#include <inttypes.h>
#include <log.hh>

#define CONFIG_SCHEMA_LOCATION "http://soundpaint.org/schema/maze-0.1/config.xsd"

Maze_config::Maze_config(const char *path) :
  Config(path),
  _node_name_any(xercesc::XMLString::transcode("*")),
  _node_name_id(xercesc::XMLString::transcode("id")),
  _node_name_ignore(xercesc::XMLString::transcode("ignore")),
  _node_name_field(xercesc::XMLString::transcode("field")),
  _node_name_ref(xercesc::XMLString::transcode("ref")),
  _node_name_tile(xercesc::XMLString::transcode("tile")),
  _node_name_tile_shortcut(xercesc::XMLString::transcode("tile-shortcut")),
  _tiles(new Tile_symbols())
{
  Config::reload();
}

Maze_config::~Maze_config()
{
  // Q objects will be deleted by Qt, just set them to 0

  delete _tiles;
  _tiles = 0;
  delete _field;
  _field = 0;

  XMLCh *node_name_any = (XMLCh *)_node_name_any;
  xercesc::XMLString::release(&node_name_any);
  _node_name_any = 0;

  XMLCh *node_name_id = (XMLCh *)_node_name_id;
  xercesc::XMLString::release(&node_name_id);
  _node_name_id = 0;

  XMLCh *node_name_ignore = (XMLCh *)_node_name_ignore;
  xercesc::XMLString::release(&node_name_ignore);
  _node_name_ignore = 0;

  XMLCh *node_name_field = (XMLCh *)_node_name_field;
  xercesc::XMLString::release(&node_name_field);
  _node_name_field = 0;

  XMLCh *node_name_ref = (XMLCh *)_node_name_ref;
  xercesc::XMLString::release(&node_name_ref);
  _node_name_ref = 0;

  XMLCh *node_name_tile = (XMLCh *)_node_name_tile;
  xercesc::XMLString::release(&node_name_tile);
  _node_name_tile = 0;

  XMLCh *node_name_tile_shortcut = (XMLCh *)_node_name_tile_shortcut;
  xercesc::XMLString::release(&node_name_tile_shortcut);
  _node_name_tile_shortcut = 0;
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

  const xercesc::DOMElement *elem_background =
    get_single_child_element(elem_config, "background", true);
  reload_brush(elem_background, &_background);

  reload_tiles(elem_config);
  reload_field(elem_config);
  //fatal("config parsed"); // DEBUG
}

void
Maze_config::reload_brush(const xercesc::DOMElement *elem_background,
			  QBrush *background)
{
  if (!elem_background) {
    fatal("elem_background is null");
  }
  const xercesc::DOMElement *elem_pixmap =
    get_single_child_element(elem_background, "pixmap");
  if (!elem_pixmap) {
    fatal("for now, background definition must contain pixmap definition");
  }
  const xercesc::DOMElement *elem_file =
    get_single_child_element(elem_pixmap, "file");
  if (!elem_file) {
    fatal("for now, pixmap definition must contain file definition");
  }
  const XMLCh *node_value_file = elem_file->getTextContent();
  char *str_file_path = xercesc::XMLString::transcode(node_value_file);
  QPixmap *pixmap = new QPixmap(str_file_path);
  if (!pixmap) {
    fatal ("not enough memory");
  }
  *background = QBrush(*pixmap);
  xercesc::XMLString::release(&str_file_path);
}

void
Maze_config::reload_field(const xercesc::DOMElement *elem_config)
{
  const xercesc::DOMNodeList *node_list =
    elem_config->getElementsByTagName(_node_name_field);
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

void
Maze_config::load_field_ignore_chars(const xercesc::DOMElement *elem_field,
                                     std::set<Xml_string> *chars) const
{
  const xercesc::DOMNodeList *node_list =
    elem_field->getElementsByTagName(_node_name_ignore);
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
Maze_config::load_field_tile_shortcuts(const xercesc::DOMElement *elem_field,
                                       xml_string_to_xml_string_t *shortcuts)
{
  const xercesc::DOMNodeList *node_list =
    elem_field->getElementsByTagName(_node_name_tile_shortcut);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_tile_shortcut =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_tile_shortcut) {
	fatal("unexpected null element");
      }

      const XMLCh *attr_id = elem_tile_shortcut->getAttribute(_node_name_id);
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

      const XMLCh *attr_ref = elem_tile_shortcut->getAttribute(_node_name_ref);
      const Xml_string *str_ref = new Xml_string(attr_ref);
      if (!str_ref) {
        fatal("not enough memory");
      }
      (*shortcuts)[str_id] = str_ref;
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
                                 const size_t width, const size_t height,
                                 std::set<Xml_string> *ignore_chars,
                                 xml_string_to_xml_string_t *shortcuts)
{
  const XMLCh *node_value_contents = elem_contents->getTextContent();
  char *node_value_contents_as_c_star =
    xercesc::XMLString::transcode(node_value_contents);

  std::vector<const Tile *> field;
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
      const Tile *tile = _tiles->lookup(str_tile_id);
      field.push_back(tile);
    } else {
      // alias_id is in set of characters to be ignored
    }
  }
  if (field.size() != (width * height)) {
    std::stringstream msg;
    msg << "field dimensions mismatch: expected " << width << "×" << height <<
      "=" << (width * height) << " elements, but found " << field.size() <<
      " unignorable fields in this contents: '" <<
      node_value_contents_as_c_star << "'";
    fatal(msg.str());
  }

  xercesc::XMLString::release(&node_value_contents_as_c_star);
  node_value_contents_as_c_star = 0;

  Brush_field *brush_field = new Brush_field(width, height, field);
  if (!brush_field) {
    fatal("not enough memory");
  }
  return brush_field;
}

void
Maze_config::load_field(const xercesc::DOMElement *elem_field)
{
  debug("'field('");
  const xercesc::DOMElement *elem_columns =
    get_single_child_element(elem_field, "columns", true);
  const size_t width = text_content_as_size_t(elem_columns);

  const xercesc::DOMElement *elem_rows =
    get_single_child_element(elem_field, "rows", true);
  const size_t height = text_content_as_size_t(elem_rows);

  std::set<Xml_string> ignore_chars;
  load_field_ignore_chars(elem_field, &ignore_chars);

  xml_string_to_xml_string_t shortcuts(BUCKET_COUNT,
                                       Xml_string::hashing_functor(),
                                       Xml_string::equal_functor());
  load_field_tile_shortcuts(elem_field, &shortcuts);

  {
    std::stringstream msg;
    msg << "building playing field [" << width << "×" << height << "]";
    debug(msg.str());
  }

  const xercesc::DOMElement *elem_contents =
    get_single_child_element(elem_field, "contents", true);
  _field =
    load_field_contents(elem_contents, width, height,
                        &ignore_chars, &shortcuts);

  debug("')'");
}

const Brush_field *
Maze_config::get_brush_field() const
{
  return _field;
}

void
Maze_config::reload_tiles(const xercesc::DOMElement *elem_config)
{
  debug("'tiles('");
  const xercesc::DOMNodeList *node_list =
    elem_config->getElementsByTagName(_node_name_tile);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_tile =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_tile) {
	fatal("unexpected null element");
      }
      const Tile *tile = load_tile(elem_tile);
      const Xml_string *id = tile->get_id();
      _tiles->add(id, tile);
    }
  } else {
    // no tiles => nothing to parse
  }
  debug("')'");
}

const Tile *
Maze_config::load_tile(const xercesc::DOMElement *elem_tile)
{
  debug("'tile('");
  if (!elem_tile) {
    fatal("unexpected null element");
  }

  const XMLCh *attr_id = elem_tile->getAttribute(_node_name_id);
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

  QBrush foreground;
  const xercesc::DOMElement *elem_foreground =
    get_single_child_element(elem_tile, "foreground");
  if (elem_foreground) {
    reload_brush(elem_foreground, &foreground);
  } else {
    std::stringstream msg;
    msg << "foreground on tile '" << str_id <<
      "': falling back to global foreground";
    fatal(msg.str());
  }

  QBrush background;
  const xercesc::DOMElement *elem_background =
    get_single_child_element(elem_tile, "background");
  if (elem_background) {
    reload_brush(elem_background, &background);
  } else {
    std::stringstream msg;
    msg << "background on tile '" << str_id <<
      "': falling back to global background";
    fatal(msg.str());
    //background = _background; // may not yet be initialized
  }

  const xercesc::DOMElement *elem_shape =
    get_single_child_element(elem_tile, "shape", true);
  if (!elem_shape) {
    fatal("for now, tile definition must contain shape definition");
  }
  Shape_terms *terms = load_tile_shape(elem_shape);

  const double foreground_potential = 0.0; // TODO
  const double background_potential = 0.0; // TODO

  Tile *tile =
    new Tile(str_id,
             foreground, background,
             foreground_potential, background_potential,
             terms);
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

Shape_terms *
Maze_config::load_tile_shape(const xercesc::DOMElement *elem_shape)
{
  debug("'tile-shape('");
  if (!elem_shape) {
    fatal("unexpected null element");
  }
  if (elem_shape->getChildElementCount() != 1) {
    fatal("'shape' element must contain exactly one child element");
  }
  const xercesc::DOMElement *elem_expression =
    elem_shape->getFirstElementChild();
  Shape_terms *terms = load_shape_expression(elem_expression);
  debug("')'");
  return terms;
}

Shape_terms *
Maze_config::load_shape_expression(const xercesc::DOMElement *elem_expression)
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
      elem_expression->getElementsByTagName(_node_name_any);
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
  return expression;
}

Shape_factors *
Maze_config::load_shape_term(const xercesc::DOMElement *elem_term)
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
      elem_term->getElementsByTagName(_node_name_any);
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

Shape_unary_expression *
Maze_config::load_shape_factor(const xercesc::DOMElement *elem_factor)
{
  if (!elem_factor) {
    fatal("unexpected null element");
  }
  bool is_negated;
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
    is_negated = true;
    node_name_unary_expression = elem_unary_expression->getNodeName();
    node_name_unary_expression_as_c_star =
      xercesc::XMLString::transcode(node_name_unary_expression);
    xercesc::XMLString::release(&node_name_factor_as_c_star);
  } else {
    elem_unary_expression = elem_factor;
    is_negated = false;
    node_name_unary_expression = node_name_factor;
    node_name_unary_expression_as_c_star = node_name_factor_as_c_star;
  }
  node_name_factor_as_c_star = 0;

  if (is_negated) {
    debug("'not('");
  }
  Shape_unary_expression *unary_expression;
  if (!strcmp(node_name_unary_expression_as_c_star, "implicit-curve")) {
    unary_expression = load_prime(elem_unary_expression);
  } else {
    unary_expression = load_shape_expression(elem_unary_expression);
  }
  unary_expression->set_negated(is_negated);
  if (is_negated) {
    debug("')'");
  }
  xercesc::XMLString::release(&node_name_unary_expression_as_c_star);
  node_name_unary_expression_as_c_star = 0;

  return unary_expression;
}

Shape_prime *
Maze_config::load_prime(const xercesc::DOMElement *elem_prime)
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

  Shape_prime *prime = new Shape_prime(implicit_curve);
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
