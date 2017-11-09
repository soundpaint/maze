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

#ifndef IMPLICIT_CURVE_HH
#define IMPLICIT_CURVE_HH

/**
 * implicit_curve ::= weighted_term { add_op weighted_term } .
 * add_op ::= plus | minus .
 * plus ::= '+' .
 * minus ::= '-' .
 * weighted_term ::= weight mul variable_term | const_term .
 * mul ::= '*'
 * weight ::= [ sign ] <non-negative-double> .
 * sign ::= plus | minus .
 * variable_term ::= quad_term | linear_term .
 * quad_term ::= term_yy | term_xy | term_xx .
 * linear_term ::= term_y | term_x .
 * term_yy ::= var_y mul var_y .
 * term_xy ::= var_x mul var_y | var_y mul var_x .
 * term_xx ::= var_x mul var_x .
 * term_y ::= var_y .
 * term_x ::= var_x .
 * var_x ::= 'x' .
 * var_y ::= 'y' .
 * const_term ::= <double> .
 */
class Implicit_curve
{
public:
  Implicit_curve(const char *expression);
  virtual ~Implicit_curve();
private:
  double _weight_term_yy;
  double _weight_term_xy;
  double _weight_term_xx;
  double _weight_term_y;
  double _weight_term_x;
  double _weight_term_const;
  void parse(const char *expression);
};

#endif /* IMPLICIT_CURVE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
