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

#include <maze-config-blocks-store.hh>
#include <log.hh>

Maze_config_blocks_store::Maze_config_blocks_store()
{
  _id_to_block = new std::unordered_map<const std::string *,
                                        const Maze_config_block *>();
  if (!_id_to_block) {
    Log::fatal("not enough memory");
  }
  _alias_char_to_block = new std::unordered_map<const std::string *,
                                                const Maze_config_block *>();
  if (!_alias_char_to_block) {
    Log::fatal("not enough memory");
  }
}

Maze_config_blocks_store::~Maze_config_blocks_store()
{
  // TODO: Check: First delete vector elements before deleting the
  // vector (memory leak)?
  delete _id_to_block;
  _id_to_block = 0;
  delete _alias_char_to_block;
  _alias_char_to_block = 0;
}

void
Maze_config_blocks_store::add(Maze_config_block *block)
{
  if (!block) {
    Log::fatal("unexpected null block");
  }
  // TODO: store in hash table with key "id"
  const char *id = block->get_id();
  //_blocks->push_back(block);
  const std::string *str_id = new std::string(id);
  if (!str_id) {
    Log::fatal("not enough memory");
  }
  //_blocks->insert({str_id, block});
  (*_id_to_block)[str_id] = block;

  const char alias_char = block->get_alias_char();
  const std::string *str_alias_char = new std::string(1, alias_char);
  if (!str_alias_char) {
    Log::fatal("not enough memory");
  }
  (*_alias_char_to_block)[str_alias_char] = block;
}

const bool
Maze_config_blocks_store::exists(const char *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  const std::string *str_id = new std::string(id);
  if (!str_id) {
    Log::fatal("not enough memory");
  }
  const bool result = _id_to_block->find(str_id) == _id_to_block->end(); // TODO
  delete str_id;
  return result;
}

const bool
Maze_config_blocks_store::exists(const char alias_char) const
{
  const std::string str_alias_char(1, alias_char);
  return
    _alias_char_to_block->find(&str_alias_char) ==
    _alias_char_to_block->end(); // TODO
}

void
Maze_config_blocks_store::dump() const
{
  for (std::pair<const std::string *,
         const Maze_config_block *> element : *_id_to_block) {
    std::cout << element.first << " :: " << element.second << std::endl;
  }
}

const Maze_config_block *
Maze_config_blocks_store::lookup(const char *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  const std::string *str_id = new std::string(id);
  if (!str_id) {
    Log::fatal("not enough memory");
  }
  const Maze_config_block *block = (*_id_to_block)[str_id];
  delete str_id;
  return block;
}

const Maze_config_block *
Maze_config_blocks_store::lookup(const char alias_char) const
{
  const std::string str_alias_char(1, alias_char);
  return (*_alias_char_to_block)[&str_alias_char];
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
