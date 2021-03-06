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

#ifndef IMPLICIT_CURVE_PARSER_TOKEN_HH
#define IMPLICIT_CURVE_PARSER_TOKEN_HH

#include <string>

class Implicit_curve_parser_token
{
public:
  enum Type {
    NONE = 0,
    ADD,
    MINUS,
    MUL,
    VAR_X,
    VAR_Y,
    DOUBLE,
    STOP
  };
  static Implicit_curve_parser_token None, Add, Minus, Mul, Var_x, Var_y, Eof;
  virtual ~Implicit_curve_parser_token();
  const Type get_type() const;
  const double get_double_value() const;
  static const Implicit_curve_parser_token *
  create_double_token(const double value);
  static const Implicit_curve_parser_token *
  create_double_token(const std::string value);
  std::string to_string() const;
private:
  Implicit_curve_parser_token(const Type type);
  Implicit_curve_parser_token(const double value);
  Type _type;
  double _value_double;
};

#endif /* IMPLICIT_CURVE_PARSER_TOKEN_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
