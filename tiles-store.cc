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

#include <tiles-store.hh>
#include <log.hh>

Tiles_store::Tiles_store()
{
  _id_to_tile =
    new xml_string_to_tile_t(BUCKET_COUNT,
                             Xml_string::hashing_functor(),
                             Xml_string::equal_functor());
  if (!_id_to_tile) {
    Log::fatal("not enough memory");
  }

  _alias_char_to_tile =
    new xml_string_to_tile_t(BUCKET_COUNT,
                             Xml_string::hashing_functor(),
                             Xml_string::equal_functor());
  if (!_alias_char_to_tile) {
    Log::fatal("not enough memory");
  }
}

Tiles_store::~Tiles_store()
{
  delete _id_to_tile;
  _id_to_tile = 0;
  delete _alias_char_to_tile;
  _alias_char_to_tile = 0;
}

void
Tiles_store::add(Tile *tile)
{
  if (!tile) {
    Log::fatal("unexpected null tile");
  }

  const Xml_string *id = tile->get_id();
  (*_id_to_tile)[id] = tile;

  const Xml_string *alias_char = tile->get_alias_char();
  (*_alias_char_to_tile)[alias_char] = tile;
}

const bool
Tiles_store::exists_id(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return
    _id_to_tile->count(id) > 0;
}

const bool
Tiles_store::exists_alias_char(const Xml_string *alias_char) const
{
  if (!alias_char) {
    Log::fatal("unexpected null alias_char");
  }
  return
    _alias_char_to_tile->count(alias_char) > 0;
}

void
Tiles_store::dump() const
{
  for (std::pair<const Xml_string *,
         const Tile *> element : *_id_to_tile) {
    char *id_as_c_star = element.first->transcode();
    const Xml_string *alias_char = element.second->get_alias_char();
    char *alias_char_as_c_star = alias_char->transcode();
    std::cout << "'" << id_as_c_star << "' => '" <<
      alias_char_as_c_star << "'" << std::endl;
    alias_char->release(&id_as_c_star);
    alias_char->release(&alias_char_as_c_star);
  }
}

const Tile *
Tiles_store::lookup_by_id(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return (*_id_to_tile)[id];
}

const Tile *
Tiles_store::lookup_by_alias_char(const Xml_string *alias_char) const
{
  if (!alias_char) {
    Log::fatal("unexpected null alias_char");
  }
  return (*_alias_char_to_tile)[alias_char];
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */