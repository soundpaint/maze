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

#include <implicit-curve-tokenizer.hh>
#include <log.hh>

Implicit_curve_tokenizer::Implicit_curve_tokenizer(const std::string expression)
{
  _expression = new std::string(expression);
  if (!_expression) {
    Log::fatal("not enough memory");
  }
  _pos = 0;
}

Implicit_curve_tokenizer::~Implicit_curve_tokenizer()
{
  _pos = 0;
  delete _expression;
  _expression = 0;
}

const bool
Implicit_curve_tokenizer::eof() const
{
  return _pos >= _expression->length();
}

Implicit_curve_parser_token *
Implicit_curve_tokenizer::get_next_token()
{
  State state = ORPHANE;
  // TODO
  return NULL;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
