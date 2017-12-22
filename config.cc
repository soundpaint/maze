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

#include <config.hh>
#include <cerrno>
#include <log.hh>
#include <xml-utils.hh>

Config::Config(const char *path)
{
  if (!path) {
    fatal("path is null");
  }
  _path = new std::string(path);
  if (!_path) {
    fatal("not enough memory");
  }
  Xml_utils::initialize();
}

Config::~Config()
{
  Xml_utils::terminate();
  delete _path;
  _path = 0;
}

void
Config::fatal(const char *msg) {
  std::stringstream full_msg;
  full_msg << "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Config::fatal(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Config::fatal(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Config::debug(const char *msg) {
  std::stringstream full_msg;
  full_msg << "cfg: " << msg;
  Log::debug(full_msg.str());
}

void
Config::debug(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "while parsing XML config file: " << msg;
  Log::debug(full_msg.str());
}

void
Config::debug(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "while parsing XML config file: " << msg;
  Log::debug(full_msg.str());
}

int32_t
Config::str_to_subint32(const char *nptr, char **endptr, const int base,
                        const long min, const long max)
{
  int32_t value = strtol(nptr, endptr, base);
  if (value > max || (errno == ERANGE && value == LONG_MAX)) {
    value = max;
    errno = ERANGE;
  } else if (value < min || (errno == ERANGE && value == LONG_MIN)) {
    value = min;
    errno = ERANGE;
  }
  return value;
}

int8_t
Config::str_to_int8(const char *nptr, char **endptr, const int base)
{
  return (int8_t)str_to_subint32(nptr, endptr, base, -128, 127);
}

uint8_t
Config::str_to_uint8(const char *nptr, char **endptr, const int base)
{
  return (uint8_t)str_to_subint32(nptr, endptr, base, 0, 255);
}

int16_t
Config::str_to_int16(const char *nptr, char **endptr, const int base)
{
  return (int16_t)str_to_subint32(nptr, endptr, base, -32768, 32767);
}

uint16_t
Config::str_to_uint16(const char *nptr, char **endptr, const int base)
{
  return (uint16_t)str_to_subint32(nptr, endptr, base, 0, 65535);
}

int8_t
Config::parse_decimal_int8(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  int8_t value = str_to_int8(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a byte, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

uint8_t
Config::parse_decimal_uint8(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  uint16_t value = str_to_uint8(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not an unsigned byte, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

int16_t
Config::parse_decimal_int16(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  int16_t value = str_to_int16(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a short, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

uint16_t
Config::parse_decimal_uint16(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  uint16_t value = str_to_uint16(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not an unsigned short, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

int32_t
Config::parse_decimal_int32(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  int32_t value = strtol(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a long, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

uint32_t
Config::parse_decimal_uint32(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  uint32_t value = strtoul(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not an unsigned long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

int64_t
Config::parse_decimal_int64(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  int64_t value = strtoll(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a long long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

uint64_t
Config::parse_decimal_uint64(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  uint64_t value = strtoull(token_as_c_star, &err_pos, 10);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not an unsigned long long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

float
Config::parse_float(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  float value = strtof(token_as_c_star, &err_pos);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a double, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

double
Config::parse_double(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  double value = strtod(token_as_c_star, &err_pos);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a double, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

long double
Config::parse_long_double(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  double value = strtold(token_as_c_star, &err_pos);
  xercesc::XMLString::release(&token_as_c_star);
  token_as_c_star = 0;
  if (*err_pos) {
    std::stringstream full_msg;
    full_msg << "value is not a long double, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  return value;
}

xercesc::DOMElement *
Config::get_single_child_element(const xercesc::DOMElement *parent,
				 const char *single_child_name_as_c_star,
                                 const bool required)
{
  XMLCh *single_child_name =
    xercesc::XMLString::transcode(single_child_name_as_c_star);
  xercesc::DOMElement *single_child_element =
    get_single_child_element(parent, single_child_name, required);
  xercesc::XMLString::release(&single_child_name);
  return single_child_element;
}

xercesc::DOMElement *
Config::get_single_child_element(const xercesc::DOMElement *parent,
				 const XMLCh *single_child_name,
                                 const bool required)
{
  int64_t matching_node_index = -1;
  const xercesc::DOMNodeList *node_list =
    parent->getChildNodes();
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem) {
        // not an element, but e.g. text node => skip
        continue;
      }
      const XMLCh *elem_name = elem->getNodeName();
      if (xercesc::XMLString::equals(elem_name, single_child_name)) {
        if (matching_node_index >= 0) {
          std::stringstream msg;
          char *single_child_name_as_c_star =
            xercesc::XMLString::transcode(single_child_name);
          char *parent_name_as_c_star =
            xercesc::XMLString::transcode(parent->getNodeName());
          msg << "expected single node '" << single_child_name_as_c_star <<
            "' beneath node '" << parent_name_as_c_star <<
            "', but got " << length << " nodes instead";
          xercesc::XMLString::release(&single_child_name_as_c_star);
          xercesc::XMLString::release(&parent_name_as_c_star);
          fatal(msg.str());
        }
        matching_node_index = node_index;
      }
    }
  }
  if (required) {
    if (matching_node_index < 0) {
      std::stringstream msg;
      char *single_child_name_as_c_star =
        xercesc::XMLString::transcode(single_child_name);
      char *parent_name_as_c_star =
        xercesc::XMLString::transcode(parent->getNodeName());
      msg << "missing node '" << single_child_name_as_c_star <<
        "' beneath node '" << parent_name_as_c_star << "'";
      xercesc::XMLString::release(&single_child_name_as_c_star);
      xercesc::XMLString::release(&parent_name_as_c_star);
      fatal(msg.str());
    }
  }
  xercesc::DOMElement *single_child_element;
  if (matching_node_index >= 0) {
    xercesc::DOMNode *node = node_list->item(matching_node_index);
    if (!node) {
      fatal("unexpected null node");
    }
    single_child_element = dynamic_cast<xercesc::DOMElement *>(node);
    if (!single_child_element) {
      fatal("unexpected null element");
    }
  } else {
    single_child_element = 0;
  }
  return single_child_element;
}

const Xml_document *
Config::transform_xml(const Xml_document *source_xml,
                      const char *stylesheet_path)
{
  if (!source_xml) {
    fatal("source_xml is null");
  }
  if (!stylesheet_path) {
    fatal("stylesheet_path is null");
  }
  std::string *str_stylesheet_path = new std::string(stylesheet_path);
  if (!str_stylesheet_path) {
    fatal("not enough memory");
  }
  Xml_document *transformed_xml = 0;
  // = Xml_utils::transform_xml(source_xml, str_stylesheet_path); // TODO

  delete str_stylesheet_path;
  str_stylesheet_path= 0;

  return transformed_xml;
}

const Xml_document *
Config::preprocess_xml(const Xml_document *config_xml) const
{
  // default implementation: no preprocessing
  return config_xml;
}

void
Config::reload()
{
  const Xml_document *raw_config_xml = Xml_utils::load_xml_from_file(_path);
  const Xml_document *preprocessed_config_xml = preprocess_xml(raw_config_xml);
  const xercesc::DOMElement *elem_config =
    preprocessed_config_xml->get_document_element();
  reload(elem_config);
  print_config();

  // raw_config_xml is deleted by preprocessor
  raw_config_xml = 0;

  delete preprocessed_config_xml;
  preprocessed_config_xml = 0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
