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

#ifndef IMPLICIT_CURVE_AST_HH
#define IMPLICIT_CURVE_AST_HH

#include <string>
#include <vector>
#include <unordered_map>

/*
 * Abstract syntax tree for implicit curves.  Actually, due to the (by
 * now) highly restricted syntax of implicit curves, the syntax tree
 * is degenerated to a vector of flat term expressions.  It may grow
 * to a fully-fledged tree structure some time later.  Rather than
 * implementing a fully-fledged visitor design pattern for tree walks,
 * for now we simply iterate over the linear structure of the vector,
 * where appropriate.
 */
class Implicit_curve_ast
{
public:
  class Term {
  public:
    // syntactical structure
    enum Sign {
      PLUS, MINUS, SIGN_UNINITIALIZED
    };
    enum Variable {
      VAR_CONST = 0,
      VAR_X,
      VAR_Y,
      VAR_XX,
      VAR_XY,
      VAR_YY,
      VAR_UNINITIALIZED
    };
    Term(const Sign sign, const double weight, const Variable variable);
    virtual ~Term();
    const Sign get_sign() const;
    const double get_weight() const;
    const Variable get_variable() const;
    const std::string to_string() const;
  private:
    const Sign _sign;
    const double _weight;
    const Variable _variable;
  };

  class Implicit_curve {
  public:
    // syntactical structure
    Implicit_curve();
    virtual ~Implicit_curve();
    void clear();
    void add_term(const Term::Sign sign,
                  const double weight,
                  const Term::Variable variable);
    const std::vector<Term *> *get_terms() const;
    const std::string to_string() const;

    // semantic attributes
    const bool has_summarized_term_weight(const Term::Variable variable) const;
    const double get_summarized_term_weight(const Term::Variable variable);
    const double try_get_summarized_term_weight_with_default(const Term::Variable variable,
                                                             const double default_value);
    void set_summarized_term_weight(const Term::Variable variable,
                                    const double weight);
  private:
    // syntactical structure
    std::vector<Term *> _terms;

    // semantic attributes
    std::unordered_map</*const Implicit_curve_ast::Term::Variable*/ int, double>
    _summarized_term_weights;
  };

  // syntactical structure
  Implicit_curve_ast();
  virtual ~Implicit_curve_ast();
  void clear();
  void add_term(const Term::Sign sign,
                const double weight,
                const Term::Variable variable);
  Implicit_curve *get_implicit_curve();

private:
  Implicit_curve _implicit_curve;
};

#endif /* IMPLICIT_CURVE_AST_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
