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

#ifndef BRUSH_FACTORY_SYMBOLS_HH
#define BRUSH_FACTORY_SYMBOLS_HH

#include <unordered_map>
#include <ibrush-factory.hh>
#include <xml-string.hh>

class Brush_factory_symbols
{
public:
  Brush_factory_symbols();
  virtual ~Brush_factory_symbols();
  void add(const Xml_string *id, IBrush_factory *brush_factory);
  const bool exists(const Xml_string *id) const;
  IBrush_factory *lookup(const Xml_string *id) const;
  void dump() const;
private:
  typedef std::unordered_map<const Xml_string *,
                             IBrush_factory *,
                             Xml_string::hashing_functor,
                             Xml_string::equal_functor>
  xml_string_to_brush_factory_t;
  static const xml_string_to_brush_factory_t::size_type BUCKET_COUNT = 5;
  xml_string_to_brush_factory_t *_id_to_brush_factory;
};

#endif /* BRUSH_FACTORY_SYMBOLS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
