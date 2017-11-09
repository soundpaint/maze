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

#include <tubes.hh>
#include <log.hh>

Tubes::Tubes(const bool north_exit,
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
	     const double background_potential)
{
  _north_exit = north_exit;
  _north_link = north_link;
  _south_exit = south_exit;
  _south_link = south_link;
  _west_exit = west_exit;
  _west_link = west_link;
  _east_exit = east_exit;
  _east_link = east_link;
  if (!foreground) {
    Log::fatal("Rounded_square_shape::Rounded_square_shape(): "
	       "foreground is null");
  }
  _foreground = foreground;
  _foreground_potential = foreground_potential;
  if (!background) {
    Log::fatal("Rounded_square_shape::Rounded_square_shape(): "
	       "background is null");
  }
  _background = background;
  _background_potential = background_potential;
}

Tubes::~Tubes()
{
  _north_exit = false;
  _north_link = false;
  _south_exit = false;
  _south_link = false;
  _west_exit = false;
  _west_link = false;
  _east_exit = false;
  _east_link = false;
  _foreground = 0;
  _foreground_potential = 0.0;
  _background = 0;
  _background_potential = 0.0;
}

const double
Tubes::get_potential(const double potential,
		     const double x,
		     const double y,
		     const double x_min,
		     const double x_max,
		     const double y_min,
		     const double y_max,
		     const bool link) const
{
  double result = potential;
  if (potential == _background_potential) {
    // done
  } else {
    double x_min_ext, x_max_ext, y_min_ext, y_max_ext;
    if (link) {
      x_min_ext = x_min > 0.5 ? 0.5 : x_min;
      x_max_ext = x_max < 0.5 ? 0.5 : x_max;
      y_min_ext = y_min > 0.5 ? 0.5 : y_min;
      y_max_ext = y_max < 0.5 ? 0.5 : y_max;
    } else {
      x_min_ext = x_min;
      x_max_ext = x_max;
      y_min_ext = y_min;
      y_max_ext = y_max;
    }
    if ((x >= x_min_ext) && (x <= x_max_ext)) {
      if ((y >= y_min_ext) && (y <= y_max_ext)) {
	result = _background_potential;
      }
    }
  }
  return result;
}

const double
Tubes::get_potential(const double x, const double y) const
{
  const double tube_radius = 0.07;
  const double link_length = 0.2;
  double potential = _foreground_potential;
  if (_north_exit) {
    potential = get_potential(potential, x, y,
			      0.5 - tube_radius,
			      0.5 + tube_radius,
			      0.0,
			      0.5 - link_length,
			      _north_link);
  }
  if (_south_exit) {
    potential = get_potential(potential, x, y,
			      0.5 - tube_radius,
			      0.5 + tube_radius,
			      0.5 + link_length,
			      1.0,
			      _south_link);
  }
  if (_west_exit) {
    potential = get_potential(potential, x, y,
			      0.0,
			      0.5 - link_length,
			      0.5 - tube_radius,
			      0.5 + tube_radius,
			      _west_link);
  }
  if (_east_exit) {
    potential = get_potential(potential, x, y,
			      0.5 + link_length,
			      1.0,
			      0.5 - tube_radius,
			      0.5 + tube_radius,
			      _east_link);
  }
  return potential;
}

QBrush *
Tubes::get_brush(const double x, const double y) const
{
  const double potential = get_potential(x, y);
  QBrush *brush;
  if (potential == _foreground_potential) {
    brush = _foreground;
  } else if (potential == _background_potential) {
    brush = _background;
  } else {
    Log::fatal("Tubes::get_brush(): unexpected potential");
  }
  return brush;
}

const double
Tubes::get_avg_tan(const double block_offset_x,
		   const double block_offset_y,
		   const double dx,
		   const double dy) const
{
  // TODO
  return 0.0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
