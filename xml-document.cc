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

#include <xml-document.hh>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <log.hh>

XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XALAN(XercesParserLiaison)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)
XALAN_USING_XALAN(XercesDOMSupport)
XALAN_USING_XALAN(XercesDOMWrapperParsedSource)
XALAN_USING_XALAN(XalanDOMString)

Xml_document::Xml_document(const std::string *path)
{
  if (!path) {
    Log::fatal("path is null");
  }
  _impl = 0;
  _parser = 0;
  _document = 0;
  parse(path);
}

Xml_document::~Xml_document()
{
  // delete _err_handler; // TODO

  // Do *not* call "_document->release()", since the document is
  // automatically released by xercesc when releasing the parser.
  _document = 0;

  _parser->release();
  _parser = 0;

  //_impl->release();
  _impl = 0;
}

const char *
Xml_document::FATAL_MSG_PREFIX = "failed parsing XML file";

void
Xml_document::fatal(const char *msg, const std::string *path) {
  std::stringstream full_msg;
  full_msg << FATAL_MSG_PREFIX << " " << *path << ": " << msg;
  Log::fatal(full_msg.str());
}

void
Xml_document::fatal(const std::string msg, const std::string *path)
{
  std::stringstream full_msg;
  full_msg << FATAL_MSG_PREFIX << " " << *path << ": " << msg;
  Log::fatal(full_msg.str());
}

void
Xml_document::fatal(const XMLCh *msg, const std::string *path) {
  std::stringstream full_msg;
  full_msg << FATAL_MSG_PREFIX << " " << *path << ": " << msg;
  Log::fatal(full_msg.str());
}

void
Xml_document::parse(const std::string *path)
{
  XMLCh temp_str[100];
  xercesc::XMLString::transcode("LS", temp_str, 99);
  _impl = xercesc::DOMImplementationRegistry::getDOMImplementation(temp_str);
  _parser =
    ((xercesc::DOMImplementationLS *)_impl)->
      createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  if (!_parser) {
    fatal("not enough memory", path);
  }
  if (_parser->getDomConfig()->
    canSetParameter(xercesc::XMLUni::fgDOMValidate, true))
  {
    _parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMValidate, true);
  }
  if (_parser->getDomConfig()->
    canSetParameter(xercesc::XMLUni::fgDOMNamespaces, true))
  {
    _parser->getDomConfig()->
      setParameter(xercesc::XMLUni::fgDOMNamespaces, true);
  }
  if (_parser->getDomConfig()->
    canSetParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true))
  {
    _parser->getDomConfig()->
      setParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
  }

  // TODO:
  //MyDOMErrorHandler* _err_handler = new myDOMErrorHandler();
  //_parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, _err_handler);

  {
    std::stringstream message;
    message << "reading XML config from file " << *path;
    Log::info(message.str());
  }

  try {
    _document = _parser->parseURI(path->c_str());
  } catch (const xercesc::XMLException& exc) {
    fatal(exc.getMessage(), path);
  } catch (const xercesc::DOMException& exc) {
    fatal(exc.getMessage(), path);
  } catch (...) {
    fatal("unknown exception while reading XML file", path);
  }
  if (!_document) {
    fatal("document is null (maybe XML file does not exist?)", path);
  }
}

xercesc::DOMElement *
Xml_document::get_document_element() const
{
  return _document->getDocumentElement();
}

Xml_document *
Xml_document::transform(const std::string *stylesheet_path) const
{
  XalanTransformer transformer;
  XSLTInputSource xsl_in(stylesheet_path->c_str());
  
  XSLTInputSource xml_in("foo.xml");
  XSLTResultTarget xml_out("foo-out.xml");

  XercesParserLiaison parser_liaison;
  XercesDOMSupport dom_support(parser_liaison);
  XalanDOMString uri; // TODO

  const XercesDOMWrapperParsedSource
    parsed_source(_document,
                  parser_liaison,
                  dom_support,
                  uri);

  const int result = transformer.transform(parsed_source, xsl_in, xml_out);
  // TODO
  return 0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
