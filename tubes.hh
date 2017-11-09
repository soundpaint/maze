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

#ifndef TUBES_HH
#define TUBES_HH

#include <ishape.hh>

class Tubes : public IShape
{
public:
  Tubes(const bool north_exit,
	const bool north_link,
	const bool south_exit,
	const bool south_link,
	const bool west_exit,
	const bool west_link,
	const bool east_exit,
	const bool east_link,
	QBrush *foreground,
	const double foreground_potential,
	QBrush *background,
	const double background_potential);
  virtual ~Tubes();
  const double get_potential(const double x, const double y) const;
  QBrush *get_brush(const double x, const double y) const;
  const double get_avg_tan(const double block_offset_x,
			   const double block_offset_y,
			   const double dx,
			   const double dy) const;

private:
  bool _north_exit;
  bool _north_link;
  bool _south_exit;
  bool _south_link;
  bool _west_exit;
  bool _west_link;
  bool _east_exit;
  bool _east_link;
  QBrush *_foreground;
  double _foreground_potential;
  QBrush *_background;
  double _background_potential;
  const double get_potential(const double potential,
			     const double x,
			     const double y,
			     const double x_min,
			     const double x_max,
			     const double y_min,
			     const double y_max,
			     const bool link) const;
};

#endif /* ROUNDED_SQUARE_SHAPE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
