/*
 * Maze -- A maze / flipper game implementation for RPi with Sense Hat
 * Copyright (C) 2016, 2017, 2018 Jürgen Reuter
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

#ifndef SYMBOL_TABLE_HH
#define SYMBOL_TABLE_HH

#include <string>
#include <unordered_map>
#include <xml-string.hh>

template<class T>
class Symbol_table
{
public:
  Symbol_table(const std::string *table_id);
  virtual ~Symbol_table();
  void add(const Xml_string *id, T t);
  const bool exists(const Xml_string *id) const;
  T lookup(const Xml_string *id) const;
  const std::string *to_string() const;
private:
  const std::string *_table_id;
  typedef std::unordered_map<const Xml_string *,
                             T,
                             Xml_string::hashing_functor,
                             Xml_string::equal_functor>
  xml_string_to_t_t;
  static const typename xml_string_to_t_t::size_type BUCKET_COUNT = 5;
  xml_string_to_t_t *_symbols;
};

#endif /* SYMBOL_TABLE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
