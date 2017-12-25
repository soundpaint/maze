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

#ifndef TILES_STORE_HH
#define TILES_STORE_HH

#include <unordered_map>
#include <tile.hh>
#include <xml-string.hh>

class Tiles_store
{
public:
  Tiles_store();
  virtual ~Tiles_store();
  void add(Tile *tile);
  const bool exists_id(const Xml_string *id) const;
  const bool exists_alias_char(const Xml_string *alias_char) const;
  const Tile *lookup_by_id(const Xml_string *id) const;
  const Tile *lookup_by_alias_char(const Xml_string *alias_char) const;
  void dump() const;
private:
  typedef std::unordered_map<const Xml_string *,
                             const Tile *,
                             Xml_string::hashing_functor,
                             Xml_string::equal_functor>
  xml_string_to_tile_t;

  static const xml_string_to_tile_t::size_type BUCKET_COUNT = 5;

  xml_string_to_tile_t *_id_to_tile;
  xml_string_to_tile_t *_alias_char_to_tile;
};

#endif /* TILES_STORE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
