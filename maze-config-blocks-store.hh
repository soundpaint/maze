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

#ifndef MAZE_CONFIG_BLOCKS_STORE_HH
#define MAZE_CONFIG_BLOCKS_STORE_HH

#include <unordered_map>
//#include <vector>
#include <maze-config-block.hh>

class Maze_config_blocks_store
{
public:
  Maze_config_blocks_store();
  virtual ~Maze_config_blocks_store();
  void add(Maze_config_block *block);
  const bool exists(const char *id) const;
  const bool exists(const char alias_char) const;
  const Maze_config_block *lookup(const char *id) const;
  const Maze_config_block *lookup(const char alias_char) const;
  void dump() const;
private:
  //std::vector<const Maze_config_block *> *_blocks;
  std::unordered_map<const std::string *,
                     const Maze_config_block *> *_id_to_block;
  std::unordered_map<const std::string *,
                     const Maze_config_block *> *_alias_char_to_block;
};

#endif /* MAZE_CONFIG_BLOCKS_STORE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
