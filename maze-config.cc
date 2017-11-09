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
#include <log.hh>

#define CONFIG_SCHEMA_LOCATION "http://soundpaint.org/schema/maze-0.1/config.xsd"

Maze_config::Maze_config(const char *path) : Config(path)
{
  _node_name_any = xercesc::XMLString::transcode("*");
  _node_name_block = xercesc::XMLString::transcode("block");
  _node_name_id = xercesc::XMLString::transcode("id");
  _blocks = new Maze_config_blocks_store();
  Config::reload();
}

Maze_config::~Maze_config()
{
  // Q objects will be deleted by Qt, just set them to 0

  delete _blocks;
  _blocks = 0;
  xercesc::XMLString::release(&_node_name_any);
  _node_name_any = 0;
  xercesc::XMLString::release(&_node_name_block);
  _node_name_block = 0;
  xercesc::XMLString::release(&_node_name_id);
  _node_name_id = 0;
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
  reload_brush(elem_config, &_background);
  reload_blocks(elem_config);
  fatal("config parsed");
}

void
Maze_config::reload_brush(const xercesc::DOMElement *elem_config,
			  QBrush *background)
{
  const xercesc::DOMElement *elem_background =
    get_single_child_element(elem_config, "background");
  if (elem_background) {
    const xercesc::DOMElement *elem_pixmap =
      get_single_child_element(elem_background, "pixmap");
    if (elem_pixmap) {
      const xercesc::DOMElement *elem_file =
	get_single_child_element(elem_pixmap, "file");
      if (elem_file) {
	const XMLCh *node_value_file = elem_file->getTextContent();
	char *str_file_path = xercesc::XMLString::transcode(node_value_file);
	*background = QBrush(str_file_path);
	xercesc::XMLString::release(&str_file_path);
      } else {
	fatal("for now, pixmap definition must contain file definition");
      }
    } else {
      fatal("for now, background definition must contain pixmap definition");
    }
  } else {
    // no background defined
    *background = QBrush();
  }
}

void
Maze_config::reload_blocks(const xercesc::DOMElement *elem_config)
{
  debug("'blocks('");
  const xercesc::DOMNodeList *node_list =
    elem_config->getElementsByTagName(_node_name_block);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem_block =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem_block) {
	fatal("unexpected null element");
      }
      Maze_config_block *block = load_block(elem_block);
      _blocks->add(block);
    }
  } else {
    // no blocks => nothing to parse
  }
  debug("')'");
}

Maze_config_block *
Maze_config::load_block(const xercesc::DOMElement *elem_block)
{
  debug("'block('");
  if (!elem_block) {
    fatal("unexpected null element");
  }
  Maze_config_block *block = new Maze_config_block();
  if (!block) {
    fatal("not enough memory");
  }
  const XMLCh *attr_id = elem_block->getAttribute(_node_name_id);
  if (attr_id) {
    load_block_id(attr_id, block);
  }
  const xercesc::DOMElement *elem_alias =
    get_single_child_element(elem_block, "alias");
  if (elem_alias) {
    load_block_alias(elem_alias, block);
  }
  const xercesc::DOMElement *elem_foreground =
    get_single_child_element(elem_block, "foreground");
  if (elem_foreground) {
    QBrush foreground;
    reload_brush(elem_foreground, &foreground);
    block->set_foreground(foreground);
  }
  const xercesc::DOMElement *elem_background =
    get_single_child_element(elem_block, "background");
  if (elem_background) {
    QBrush background;
    reload_brush(elem_background, &background);
    block->set_background(background);
  }
  const xercesc::DOMElement *elem_shape =
    get_single_child_element(elem_block, "shape");
  if (elem_shape) {
    load_block_shape(elem_shape, block);
  } else {
    fatal("for now, block definition must contain shape definition");
  }
  debug("')'");
  return block;
}

void
Maze_config::load_block_id(const XMLCh *attr_id,
                           Maze_config_block *block)
{
  debug("'id('");
  if (!attr_id) {
    fatal("unexpected null attribute");
  }
  if (!block) {
    fatal("unexpected null block");
  }
  char *attr_id_as_c_star = xercesc::XMLString::transcode(attr_id);
  char *id = strdup(attr_id_as_c_star);
  xercesc::XMLString::release(&attr_id_as_c_star);
  attr_id_as_c_star = 0;
  if (!id) {
    fatal("not enough memory");
  }
  debug(id);
  block->set_id(id);
  debug("')'");
}

void
Maze_config::load_block_alias(const xercesc::DOMElement *elem_alias,
                              Maze_config_block *block)
{
  debug("'alias('");
  if (!elem_alias) {
    fatal("unexpected null element");
  }
  if (!block) {
    fatal("unexpected null block");
  }
  const XMLCh *node_value_alias = elem_alias->getTextContent();
  char *node_value_alias_as_c_star =
    xercesc::XMLString::transcode(node_value_alias);
  char *alias = strdup(node_value_alias_as_c_star);
  xercesc::XMLString::release(&node_value_alias_as_c_star);
  node_value_alias_as_c_star = 0;
  if (!alias) {
    fatal("not enough memory");
  }
  debug(alias);
  block->set_alias(alias);
  debug("')'");
}

void
Maze_config::load_block_shape(const xercesc::DOMElement *elem_shape,
                              Maze_config_block *block)
{
  debug("'block-shape('");
  if (!elem_shape) {
    fatal("unexpected null element");
  }
  if (!block) {
    fatal("unexpected null block");
  }
  if (elem_shape->getChildElementCount() != 1) {
    fatal("'shape' element must contain exactly one child element");
  }
  const xercesc::DOMElement *elem_expression =
    elem_shape->getFirstElementChild();
  block->set_terms(load_shape_expression(elem_expression));
  debug("')'");
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
  Implicit_curve *implicit_curve = new Implicit_curve(str_prime);
  if (!implicit_curve) {
    fatal("not enough memory");
  }
  debug(str_prime);
  xercesc::XMLString::release(&str_prime);
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
  msg << "\r\n";
  msg << "######## CONFIG ########\r\n";
  /*
  msg << "column height: " << _column_height << "\r\n";
  msg << "client id: ";
  if (_client_id) {
    msg << _client_id;
  } else {
    msg << "<use-host-name>";
  } 
  msg << "\r\n";
  msg << "effective client id: " << _effective_client_id << "\r\n";
  msg << "range index: " << _range_index << "\r\n";
  msg << "range out of: " << _range_out_of << "\r\n";
  msg << "range bottom index: " << _range_bottom_index << "\r\n";
  msg << "range top index: " << _range_top_index << "\r\n";
  msg << "effective bottom index: " << _effective_bottom_index << "\r\n";
  msg << "effective top index: " << _effective_top_index << "\r\n";
  print_config_client_sound("red", &msg, _sound_red);
  print_config_client_sound("green", &msg, _sound_green);
  print_config_client_sound("blue", &msg, _sound_blue);
  msg << "client volume: " << _volume << "\r\n";
  msg << "########################";
  debug(msg.str());
  */
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
