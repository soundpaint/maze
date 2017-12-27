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

#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xml-document.hh>

class Config
{
protected:
  Config(const char *path);
  virtual ~Config();

  // Override this method in order to add preprocessing of the
  // configuration XML.  Default implementation just returns the
  // original XML.  Implementation of this method is responsible for
  // release memory of original XML, if a different document is
  // returned.
  virtual const Xml_document *
  preprocess_xml(const Xml_document *config_xml) const;

  // helper function for implementors of preprocess_xml() method
  static const Xml_document *
  transform_xml(const Xml_document *source_xml, const char *stylesheet_path);

  virtual void reload();
  static void fatal(const char *msg);
  static void fatal(const std::string msg);
  static void fatal(const XMLCh *msg);
  static void debug(const char *msg);
  static void debug(const std::string msg);
  static void debug(const XMLCh *msg);
  static int32_t str_to_subint32(const char *nptr, char **endptr,
                                 const int base,
                                 const long min, const long max);
  static int8_t str_to_int8(const char *nptr, char **endptr, const int base);
  static uint8_t str_to_uint8(const char *nptr, char **endptr, const int base);
  static int16_t str_to_int16(const char *nptr, char **endptr, const int base);
  static uint16_t str_to_uint16(const char *nptr, char **endptr,
                                const int base);
  static int8_t parse_decimal_int8(const XMLCh *token);
  static uint8_t parse_decimal_uint8(const XMLCh *token);
  static int16_t parse_decimal_int16(const XMLCh *token);
  static uint16_t parse_decimal_uint16(const XMLCh *token);
  static int32_t parse_decimal_int32(const XMLCh *token);
  static uint32_t parse_decimal_uint32(const XMLCh *token);
  static int64_t parse_decimal_int64(const XMLCh *token);
  static uint64_t parse_decimal_uint64(const XMLCh *token);
  static float parse_float(const XMLCh *token);
  static double parse_double(const XMLCh *token);
  static long double parse_long_double(const XMLCh *token);
  static xercesc::DOMElement *
  get_single_child_element(const xercesc::DOMElement *parent,
                           const XMLCh *single_child_name,
                           const bool required = false);
  virtual void reload(const xercesc::DOMElement *doc_elem) = 0;
  virtual void print_config() = 0;
private:
  std::string *_path;
};

#endif /* CONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
