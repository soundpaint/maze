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

#include <implicit-curve-parser-token.hh>
#include <log.hh>

Implicit_curve_parser_token
Implicit_curve_parser_token::None(NONE);

Implicit_curve_parser_token
Implicit_curve_parser_token::Add(ADD);

Implicit_curve_parser_token
Implicit_curve_parser_token::Minus(MINUS);

Implicit_curve_parser_token
Implicit_curve_parser_token::Mul(MUL);

Implicit_curve_parser_token
Implicit_curve_parser_token::Var_x(VAR_X);

Implicit_curve_parser_token
Implicit_curve_parser_token::Var_y(VAR_Y);

Implicit_curve_parser_token::Implicit_curve_parser_token(const Type type)
{
  _type = type;
  _value_double = 0.0;
}

Implicit_curve_parser_token::Implicit_curve_parser_token(const double value)
{
  _type = DOUBLE;
  _value_double = value;
}

Implicit_curve_parser_token::~Implicit_curve_parser_token()
{
  _type = NONE;
  _value_double = 0;
}

const Implicit_curve_parser_token::Type
Implicit_curve_parser_token::get_type() const
{
  return _type;
}

const double
Implicit_curve_parser_token::get_double_value() const
{
  if (_type != DOUBLE) {
    Log::fatal("symbol is not a double");
  }
  return _value_double;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
