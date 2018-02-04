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

#ifndef SYMBOL_TABLE_TCC
#define SYMBOL_TABLE_TCC

#include <symbol-table.hh>

template<class T>
Symbol_table<T>::Symbol_table(const std::string *table_id) :
  _table_id(table_id)
{
  if (!table_id) {
    Log::fatal("unexpected null table_id");
  }
  _symbols =
    new xml_string_to_t_t(BUCKET_COUNT,
                          Xml_string::hashing_functor(),
                          Xml_string::equal_functor());
  if (!_symbols) {
    Log::fatal("not enough memory");
  }
}

template<class T>
Symbol_table<T>::~Symbol_table()
{
  delete _symbols;
  _symbols = 0;
  _table_id = 0;
}

template<class T>
void
Symbol_table<T>::add(const Xml_string *id, T t)
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  if (!t) {
    Log::fatal("unexpected null t");
  }
  if (exists(id)) {
    std::stringstream msg;
    char *str_id = id->transcode();
    msg << "already have symbol with id=" << str_id <<
      " in symbol table " << *_table_id;
    id->release(&str_id);
    Log::fatal(msg.str());
  }
  (* _symbols)[id] = t;
}

template<class T>
const bool
Symbol_table<T>::exists(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return
    _symbols->count(id) > 0;
}

template<class T>
T
Symbol_table<T>::lookup(const Xml_string *id) const
{
  if (!id) {
    Log::fatal("unexpected null id");
  }
  return (*_symbols)[id];
}

template<class T>
const std::string *
Symbol_table<T>::to_string() const
{
  std::stringstream msg;
  msg << "Symbol_table<" << *_table_id << ">{";
  bool first = true;
  for (std::pair<const Xml_string *, T> element : *_symbols) {
    if (!first) {
      msg << ", ";
    } else {
      first = false;
    }
    char *id_as_c_star = element.first->transcode();
    msg << "'" << id_as_c_star << "'" << std::endl;
    element.first->release(&id_as_c_star);
  }
  msg << "}";
  const std::string *result = new std::string(msg.str());
  if (!result) {
    Log::fatal("not enough memory");
  }
  return result;
}

#endif // SYMBOL_TABLE_TCC

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
