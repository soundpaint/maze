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

#include <shape.hh>
#include <log.hh>
Shape::Shape(const Xml_string *id,
             const Shape_terms *shape_terms) :
  _id(id),
  _shape_terms(shape_terms)
{
  if (!id) {
    Log::fatal("id is null");
  }
  if (!shape_terms) {
    Log::fatal("shape_terms is null");
  }
}

Shape::~Shape()
{
  //free(_id); // TODO
  _id = 0;
  delete _shape_terms;
  _shape_terms = 0;
}

const std::string
Shape::to_string() const
{
  std::stringstream str;
  str << "Shape{";
  str << "id=" << _id;
  str << ", shape_terms=" << _shape_terms;
  str <<"}";
  return std::string(str.str());
}

const Xml_string *
Shape::get_id() const
{
  return _id;
}

const Shape_terms *
Shape::get_terms() const
{
  return _shape_terms;
}

const bool
Shape::is_inside(const double x, const double y) const
{
  return _shape_terms->is_inside(x, y);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
