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

#include <xml-node-list.hh>
#include <log.hh>

Xml_node_list::Xml_node_list()
{
  _nodes = new std::vector<xercesc::DOMNode *>();
  if (!_nodes) {
    Log::fatal("not enough memory");
  }
}

Xml_node_list::~Xml_node_list()
{
  delete _nodes;
  _nodes = 0;
}

void
Xml_node_list::add(xercesc::DOMNode *node)
{
  _nodes->push_back(node);
}

xercesc::DOMNode *
Xml_node_list::item(const XMLSize_t index) const
{
  if (index > getLength()) {
    Log::fatal("index out of bounds");
  }
  return _nodes->at(index);
}

XMLSize_t
Xml_node_list::getLength() const
{
  return _nodes->size();
}


/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
