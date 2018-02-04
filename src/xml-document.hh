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

#ifndef XML_DOCUMENT_HH
#define XML_DOCUMENT_HH

#include <string>
#include <xercesc/dom/DOM.hpp>

/**
 * xerces-c releases the parsed XML document as soon as the parser is
 * released.  Therefore, the life cycle of the parser must comprehend
 * that of the document.  That is, an XML document and its parser must
 * always be stored side by side.  In order to free the user from this
 * burden, this wrapper class wraps an XML document as well as its
 * companion parser in a single object.  The life cycle of the XML
 * document and the parser is bounded to the life cycle of this
 * wrapper object.
 */
class Xml_document
{
public:
  Xml_document(const std::string *path);
  virtual ~Xml_document();
  xercesc::DOMElement *get_document_element() const;
  Xml_document *transform(const std::string *stylesheet_path) const;
private:
  static const char *FATAL_MSG_PREFIX;
  xercesc::DOMImplementation *_impl;
  xercesc::DOMLSParser *_parser;
  xercesc::DOMDocument *_document;
  static void fatal(const char *msg, const std::string *path);
  static void fatal(const std::string msg, const std::string *path);
  static void fatal(const XMLCh *msg, const std::string *path);
  void parse(const std::string *path);
};

#endif /* XML_DOCUMENT_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
