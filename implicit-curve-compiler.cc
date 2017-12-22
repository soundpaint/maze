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

#include <implicit-curve-compiler.hh>
#include <log.hh>
#include <implicit-curve.hh>
#include <unordered_map>

Implicit_curve_compiler::Implicit_curve_compiler()
{
}

Implicit_curve_compiler::~Implicit_curve_compiler()
{
}

void
Implicit_curve_compiler::summarize_terms_by_variables(Implicit_curve_ast *implicit_curve_ast)
{
  /*
  std::unordered_map<const Implicit_curve_ast::Term::Variable, double>
    summarized_term_weights;
  */
  Implicit_curve_ast::Implicit_curve *implicit_curve =
    implicit_curve_ast->get_implicit_curve();
  const std::vector<Implicit_curve_ast::Term *> *terms =
    implicit_curve->get_terms();
  for (const Implicit_curve_ast::Term *term : *terms) {
    const Implicit_curve_ast::Term::Sign sign = term->get_sign();
    const double raw_weight = term->get_weight();
    const double weight =
      (sign == Implicit_curve_ast::Term::MINUS) ? -raw_weight : raw_weight;
    const Implicit_curve_ast::Term::Variable variable = term->get_variable();
    double summarized_term_weight;
    if (implicit_curve->has_summarized_term_weight(variable)) {
      summarized_term_weight =
        implicit_curve->get_summarized_term_weight(variable);
    } else {
      summarized_term_weight = 0.0;
    };
    implicit_curve->set_summarized_term_weight(variable,
                                               summarized_term_weight + weight);
  }
}

void
Implicit_curve_compiler::optimize(Implicit_curve_ast *implicit_curve_ast)
{
  summarize_terms_by_variables(implicit_curve_ast);
}

const Implicit_curve *
Implicit_curve_compiler::generate_code(Implicit_curve_ast *implicit_curve_ast)
{
  Implicit_curve_ast::Implicit_curve *implicit_curve =
    implicit_curve_ast->get_implicit_curve();
  const double weight_term_yy =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_YY, 0.0);
  const double weight_term_xy =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_XY, 0.0);
  const double weight_term_xx =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_XX, 0.0);
  const double weight_term_y =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_Y, 0.0);
  const double weight_term_x =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_X, 0.0);
  const double weight_term_const =
    implicit_curve->
    try_get_summarized_term_weight_with_default(Implicit_curve_ast::Term::VAR_CONST, 0.0);
  const Implicit_curve *result =
    new Implicit_curve(weight_term_yy,
                       weight_term_xy,
                       weight_term_xx,
                       weight_term_y,
                       weight_term_x,
                       weight_term_const);
  if (!result) {
    Log::fatal("not enough memory");
  }

  return result;
}

const Implicit_curve *
Implicit_curve_compiler::compile(const char *expression)
{
  Implicit_curve_ast implicit_curve_ast =
    _implicit_curve_parser.parse(expression);

  optimize(&implicit_curve_ast);

  const Implicit_curve *implicit_curve = generate_code(&implicit_curve_ast);
  return implicit_curve;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
