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

#include <xml-utils.hh>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <log.hh>

void
Xml_utils::initialize()
{
  try {
    xercesc::XMLPlatformUtils::Initialize();
    xalanc::XalanTransformer::initialize();
  } catch (const xercesc::XMLException& exc) {
    std::stringstream full_msg;
    full_msg << "failed initializing XML/XSL platform: " << &exc;
    Log::fatal(full_msg.str());
  }
}

void
Xml_utils::terminate()
{
  try {
    xalanc::XalanTransformer::terminate();
    xalanc::XalanTransformer::ICUCleanUp();
    xercesc::XMLPlatformUtils::Terminate();
  } catch (const xercesc::XMLException& exc) {
    std::stringstream full_msg;
    full_msg << "failed terminating XML/XSL platform: " << &exc;
    Log::fatal(full_msg.str());
  }
}

Xml_document *
Xml_utils::load_xml_from_file(const std::string *path)
{
  Xml_document *document = new Xml_document(path);
  if (!document) {
    Log::fatal("not enough memory");
  }
  return document;
}

Xml_document *
Xml_utils::transform(const Xml_document *source,
                     const std::string *stylesheet_path)
{
  if (!source) {
    Log::fatal("source is null");
  }
  if (!stylesheet_path) {
    Log::fatal("stylesheet_path");
  }
  return source->transform(stylesheet_path);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
