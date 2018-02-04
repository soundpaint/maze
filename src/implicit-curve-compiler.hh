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

#ifndef IMPLICIT_CURVE_COMPILER_HH
#define IMPLICIT_CURVE_COMPILER_HH

#endif /* IMPLICIT_CURVE_COMPILER_HH */

#include <implicit-curve-parser.hh>
#include <implicit-curve.hh>

class Implicit_curve_compiler
{
public:
  Implicit_curve_compiler();
  virtual ~Implicit_curve_compiler();
  static const Implicit_curve *compile(const char *expression);
private:
  static void summarize_terms_by_variables(Implicit_curve_ast *implicit_curve_ast);
  static void optimize(Implicit_curve_ast *implicit_curve_ast);
  static const Implicit_curve *
  generate_code(Implicit_curve_ast *implicit_curve_ast);
};

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
