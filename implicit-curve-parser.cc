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

#include <implicit-curve-parser.hh>
#include <math.h>
#include <log.hh>

Implicit_curve_parser::Implicit_curve_parser()
{
  _tokenizer = 0;
  _look_ahead = 0;
  reset();
}

Implicit_curve_parser::~Implicit_curve_parser()
{
  reset();
}

void
Implicit_curve_parser::reset()
{
  _rule_depth = 0;
  if (_look_ahead) {
    _tokenizer->release_token(_look_ahead);
    _look_ahead = 0;
  }
  if (_tokenizer) {
    delete _tokenizer;
    _tokenizer = 0;
  }
}

const bool
Implicit_curve_parser::eof() const
{
  return _tokenizer->eof();
}

const int
Implicit_curve_parser::get_display_position() const
{
  return _tokenizer->get_display_position();
}

const Implicit_curve_parser_token *
Implicit_curve_parser::look_ahead()
{
  if (!_look_ahead) {
    _look_ahead = _tokenizer->get_next_token();
  }
  return _look_ahead;
}

void
Implicit_curve_parser::accept_token()
{
  if (!_look_ahead) {
    Log::warn("accepting token without looking at it");
    _look_ahead = _tokenizer->get_next_token();
  }
  _tokenizer->release_token(_look_ahead);
  _look_ahead = 0;
}

std::string
Implicit_curve_parser::create_indent(const int count)
{
  std::string result = "";
  result.append(count, ' ');
  return result;
}

void
Implicit_curve_parser::open_rule(const char *lhs)
{
  _rule_depth++;
  std::stringstream msg;
  msg << create_indent(2 * _rule_depth) << "{" << lhs << ":";
  Log::debug(msg.str());
}

const bool
Implicit_curve_parser::close_rule(const bool result)
{
  std::stringstream msg;
  msg << create_indent(2 * _rule_depth) << (result ? "ok" : "err") << "}";
  Log::debug(msg.str());
  _rule_depth--;
  return result;
}

// const_term ::= <unsigned_double> .
const bool
Implicit_curve_parser::parse_const_term(double *value)
{
  open_rule("const_term");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::DOUBLE) {
    *value = look_ahead()->get_double_value();
    accept_token();
    result = true;
  } else {
    //*value = nan(0);
    *value = 0.0;
    result = false;
  }
  return close_rule(result);
}

// mul ::= '*' .
const bool
Implicit_curve_parser::parse_mul()
{
  open_rule("mul");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::MUL) {
    accept_token();
    result = true;
  } else {
    result = false;
  }
  return close_rule(result);
}

// var_x ::= 'x' .
const bool
Implicit_curve_parser::parse_var_x()
{
  open_rule("var_x");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::VAR_X) {
    accept_token();
    result = true;
  } else {
    result = false;
  }
  return close_rule(result);
}

// var_y ::= 'y' .
const bool
Implicit_curve_parser::parse_var_y()
{
  open_rule("var_y");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::VAR_Y) {
    accept_token();
    result = true;
  } else {
    result = false;
  }
  return close_rule(result);
}

// linear_term ::= var_x | var_y .
const bool
Implicit_curve_parser::parse_linear_term()
{
  open_rule("linear_term");
  bool result;
  result = parse_var_x() || parse_var_y();
  return close_rule(result);
}

// variable_term ::= linear_term [ mul linear_term ] .
const bool
Implicit_curve_parser::parse_variable_term()
{
  open_rule("variable_term");
  bool result;
  if (!parse_linear_term()) {
    result = false;
  } else {
    if (parse_mul()) {
      result = parse_linear_term();
    } else {
      return true;
    }
  }
  return close_rule(result);
}

// weighted_term ::= const_term [ mul variable_term ] .
const bool
Implicit_curve_parser::parse_weighted_term(double *weight)
{
  open_rule("weighted_term");
  bool result;
  if (!parse_const_term(weight)) {
    result = false;
  } else {
    if (parse_mul()) {
      result = parse_variable_term();
    }
    result = true;
  }
  return close_rule(result);
}

// unsigned_term ::= weighted_term | variable_term .
const bool
Implicit_curve_parser::parse_unsigned_term(double *weight)
{
  open_rule("unsigned_term");
  bool result;
  Implicit_curve_parser_token::Type type = look_ahead()->get_type();
  if ((type == Implicit_curve_parser_token::VAR_X) ||
      (type == Implicit_curve_parser_token::VAR_Y)) {
    result = parse_variable_term();
    *weight = 1.0;
  } else if (type == Implicit_curve_parser_token::DOUBLE) {
    result = parse_weighted_term(weight);
  } else {
    result = false;
  }
  return close_rule(result);
}

// sign ::= plus | minus .
const bool
Implicit_curve_parser::parse_sign(Implicit_curve_ast::Term::Sign *sign)
{
  open_rule("sign");
  bool result;
  if (parse_plus()) {
    *sign = Implicit_curve_ast::Term::Sign::PLUS;
    result = true;
  } else if (parse_minus()) {
    *sign = Implicit_curve_ast::Term::Sign::MINUS;
    result = true;
  } else {
    *sign = Implicit_curve_ast::Term::Sign::SIGN_UNINITIALIZED;
    result = false;
  }
  return close_rule(result);
}

// term ::= [ sign ] unsigned_term .
const bool
Implicit_curve_parser::parse_term()
{
  open_rule("term");
  bool result;
  Implicit_curve_ast::Term::Sign sign;
  double weight;
  if (parse_sign(&sign)) {
  } else {
    sign = Implicit_curve_ast::Term::Sign::PLUS;
  }
  result = parse_unsigned_term(&weight);
  if (result) {
    Implicit_curve_ast::Term *term =
      new Implicit_curve_ast::Term(sign, weight);
    _ast.get_implicit_curve()->get_terms()->push_back(term);
  }
  return close_rule(result);
}

// plus ::= '+' .
const bool
Implicit_curve_parser::parse_plus()
{
  open_rule("plus");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::ADD) {
    accept_token();
    result = true;
  } else {
    result = false;
  }
  return close_rule(result);
}

// minus ::= '-' .
const bool
Implicit_curve_parser::parse_minus()
{
  open_rule("minus");
  bool result;
  if (look_ahead()->get_type() == Implicit_curve_parser_token::MINUS) {
    accept_token();
    result = true;
  } else {
    result = false;
  }
  return close_rule(result);
}

// add_op ::= plus | minus .
const bool
Implicit_curve_parser::parse_add_op()
{
  open_rule("add_op");
  bool result = parse_plus() || parse_minus();
  return close_rule(result);
}

// implicit_curve ::= term { add_op term } .
const bool
Implicit_curve_parser::parse_implicit_curve()
{
  open_rule("implicit_curve");
  bool result;
  if (!parse_term()) {
    result = false;
  } else {
    while (parse_add_op()) {
      parse_term();
    }
    result = true;
  }
  return close_rule(result);
}

const Implicit_curve *
Implicit_curve_parser::parse(const char *expression)
{
  reset();
  _tokenizer = new Implicit_curve_tokenizer(expression);
  if (!_tokenizer) {
    Log::fatal("not enough memory");
  }
  {
    std::stringstream msg;
    msg << "(raw=" << expression << ")";
    Log::debug(msg.str());
  }
  const Implicit_curve *implicit_curve;
  if (parse_implicit_curve()) {
    if (!_tokenizer->eof()) {
      std::stringstream msg;
      msg << "syntax error in expression '" << expression <<
        "': unexpected trailing characters at position " <<
        get_display_position();
      Log::fatal(msg.str());
    }

    // TODO: fetch weight parameters from AST
    {
      std::stringstream msg;
      msg << "implicit_curve terms: " << _ast.get_implicit_curve()->get_terms();
      Log::debug(msg.str());
    }
    implicit_curve =
      new Implicit_curve(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    if (!implicit_curve) {
      Log::fatal("not enough memory");
    }
  } else {
    std::stringstream msg;
    msg << "syntax error in expression '" << expression <<
      "': implicit curve expected at position " <<
      get_display_position();
    Log::fatal(msg.str());
  }
  return implicit_curve;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
