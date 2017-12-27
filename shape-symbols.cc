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

#include <shape-symbols.hh>
#include <log.hh>

Shape_symbols::Shape_symbols()
{
  _id_to_shape =
    new xml_string_to_shape_t(BUCKET_COUNT,
                              Xml_string::hashing_functor(),
                              Xml_string::equal_functor());
  if (!_id_to_shape) {
    Log::fatal("not enough memory");
  }
}

Shape_symbols::~Shape_symbols()
{
  delete _id_to_shape;
  _id_to_shape = 0;
}

void
Shape_symbols::add(const Xml_string *id, const Shape *shape)
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  if (!shape) {
    Log::fatal("unexpected null shape");
  }

  if (exists(id)) {
    std::stringstream msg;
    char *str_id = id->transcode();
    msg << "already have shape with id=" << str_id  << " in symbol table";
    id->release(&str_id);
    Log::fatal(msg.str());
  }

  (*_id_to_shape)[id] = shape;
}

const bool
Shape_symbols::exists(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return
    _id_to_shape->count(id) > 0;
}

const Shape *
Shape_symbols::lookup(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return (*_id_to_shape)[id];
}

void
Shape_symbols::dump() const
{
  for (std::pair<const Xml_string *, const Shape *> element : *_id_to_shape) {
    char *id_as_c_star = element.first->transcode();
    std::cout << "'" << id_as_c_star << std::endl;
    element.first->release(&id_as_c_star);
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
