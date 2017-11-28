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

#ifndef IMPLICIT_CURVE_PARSER_HH
#define IMPLICIT_CURVE_PARSER_HH

#include <implicit-curve.hh>
#include <implicit-curve-tokenizer.hh>
#include <implicit-curve-ast.hh>

/**
 * implicit_curve ::= term { add_op term }* .
 * add_op ::= plus | minus .
 * plus ::= '+' .
 * minus ::= '-' .
 * term ::= [ sign ] unsigned_term .
 * unsigned_term ::= weighted_term | variable_term .
 * weighted_term ::= const_term [ mul variable_term ] .
 * mul ::= '*' .
 * sign ::= plus | minus .
 * variable_term ::= linear_term [ mul linear_term ] .
 * linear_term ::= var_x | var_y .
 * var_x ::= 'x' .
 * var_y ::= 'y' .
 * const_term ::= <unsigned_double> .
 */
class Implicit_curve_parser
{
public:
  Implicit_curve_parser();
  virtual ~Implicit_curve_parser();
  const Implicit_curve *parse(const char *expression);
private:
  Implicit_curve_tokenizer *_tokenizer;
  const Implicit_curve_parser_token *_look_ahead;
  int _rule_depth;
  Implicit_curve_ast _ast;
  void reset();
  const bool eof() const;
  const int get_display_position() const;
  const Implicit_curve_parser_token *look_ahead();
  void accept_token();
  static std::string create_indent(const int count);
  void open_rule(const char *lhs);
  const bool close_rule(const bool result);
  const bool parse_const_term(double *value);
  const bool parse_mul();
  const bool parse_var_x();
  const bool parse_var_y();
  const bool parse_linear_term();
  const bool parse_variable_term();
  const bool parse_weighted_term(double *weight);
  const bool parse_unsigned_term(double *weight);
  const bool parse_sign(Implicit_curve_ast::Term::Sign *sign);
  const bool parse_term();
  const bool parse_plus();
  const bool parse_minus();
  const bool parse_add_op();
  const bool parse_implicit_curve();
};

#endif /* IMPLICIT_CURVE_PARSER_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
