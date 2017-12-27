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

#ifndef TILE_HH
#define TILE_HH

#include <xercesc/dom/DOM.hpp>
#include <QtGui/QBrush>
#include <xml-string.hh>
#include <shape-expression.hh>

class Tile
{
public:
  Tile(const Xml_string *id,
       const QBrush foreground,
       const QBrush background,
       const double foreground_potential,
       const double background_potential,
       const Shape_terms *terms);
  virtual ~Tile();
  const std::string to_string() const;
  const double get_potential(const double x, const double y) const;
  const QBrush *get_brush(const double x, const double y) const;
  const double get_avg_tan(const double tile_offset_x,
                           const double tile_offset_y,
                           const double dx,
                           const double dy) const;
  const Xml_string *get_id() const;
  const QBrush get_foreground() const;
  const QBrush get_background() const;
  const Shape_terms *get_terms() const;
private:
  const Xml_string *_id;
  const QBrush _foreground;
  const QBrush _background;
  const double _foreground_potential;
  const double _background_potential;
  const Shape_terms *_shape_expression;
};

#endif /* TILE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
