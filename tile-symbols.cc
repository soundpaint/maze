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

#include <tile-symbols.hh>
#include <log.hh>

Tile_symbols::Tile_symbols()
{
  _id_to_tile =
    new xml_string_to_tile_t(BUCKET_COUNT,
                             Xml_string::hashing_functor(),
                             Xml_string::equal_functor());
  if (!_id_to_tile) {
    Log::fatal("not enough memory");
  }
}

Tile_symbols::~Tile_symbols()
{
  delete _id_to_tile;
  _id_to_tile = 0;
}

void
Tile_symbols::add(const Xml_string *id, Tile *tile)
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  if (!tile) {
    Log::fatal("unexpected null tile");
  }

  if (exists(id)) {
    std::stringstream msg;
    char *str_id = id->transcode();
    msg << "already have tile with id=" << str_id  << " in symbol table";
    id->release(&str_id);
    Log::fatal(msg.str());
  }

  (*_id_to_tile)[id] = tile;
}

const bool
Tile_symbols::exists(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return
    _id_to_tile->count(id) > 0;
}

Tile *
Tile_symbols::lookup(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return (*_id_to_tile)[id];
}

void
Tile_symbols::dump() const
{
  for (std::pair<const Xml_string *, Tile *> element : *_id_to_tile) {
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
