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

#include <rounded-square-shape.hh>
#include <log.hh>

Rounded_square_shape::Rounded_square_shape(const double upper_left_roundness,
					   const double upper_right_roundness,
					   const double lower_left_roundness,
					   const double lower_right_roundness,
					   QBrush *foreground,
					   const double foreground_potential,
					   QBrush *background,
					   const double background_potential)
{
  fill_roundness_info(&_upper_left_roundness, upper_left_roundness,
		      0.0, 0.0, "upper left");
  fill_roundness_info(&_upper_right_roundness, upper_right_roundness,
		      1.0, 0.0, "upper right");
  fill_roundness_info(&_lower_left_roundness, lower_left_roundness,
		      0.0, 1.0, "lower left");
  fill_roundness_info(&_lower_right_roundness, lower_right_roundness,
		      1.0, 1.0, "lower right");
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

Rounded_square_shape::~Rounded_square_shape()
{
  clear_roundness(&_upper_left_roundness);
  clear_roundness(&_upper_right_roundness);
  clear_roundness(&_lower_left_roundness);
  clear_roundness(&_lower_right_roundness);
  _foreground = 0;
  _foreground_potential = 0.0;
  _background = 0;
  _background_potential = 0.0;
}

void
Rounded_square_shape::clear_roundness(struct roundness_t *roundness)
{
  roundness->center_x = 0.0;
  roundness->center_y = 0.0;
  roundness->squared_radius = 0.0;
}

void
Rounded_square_shape::fill_roundness_info(struct roundness_t *roundness,
					  const double roundness_value,
					  const double edge_x,
					  const double edge_y,
					  const char *label)
{
  if ((roundness_value < 0.0) || (roundness_value > 1.0)) {
    std::stringstream msg;
    msg << "Rounded_square_shape::check_roundness(): " <<
      "roundness value of " << label << " corner out of range";
    Log::fatal(msg.str());
  }
  roundness->edge_x = edge_x;
  roundness->edge_y = edge_y;
  roundness->center_x = edge_x + roundness_value * (0.5 - edge_x);
  roundness->center_y = edge_y + roundness_value * (0.5 - edge_y);
  roundness->squared_radius = 0.25 * roundness_value * roundness_value;
}

const double
Rounded_square_shape::get_potential(const double x,
				    const double y,
				    const struct roundness_t *roundness) const
{
  const double dx = roundness->center_x - x;
  const double dy = roundness->center_y - y;
  const double squared_radius = roundness->squared_radius;
  const double edge_x = roundness->edge_x;
  const double edge_y = roundness->edge_y;
  const double center_x = roundness->center_x;
  const double center_y = roundness->center_y;
  const bool x_in_edge_quadrant =
    ((edge_x <= x) && (x <= center_x)) ||
    ((center_x <= x) && (x <= edge_x));
  const bool y_in_edge_quadrant =
    ((edge_y <= y) && (y <= center_y)) ||
    ((center_y <= y) && (y <= edge_y));
  const bool in_edge_quadrant = x_in_edge_quadrant && y_in_edge_quadrant;
  const bool outside =
    in_edge_quadrant && (dx * dx + dy * dy >= squared_radius);
  return outside ? _background_potential : _foreground_potential;
}

const double
Rounded_square_shape::get_potential(const double x, const double y) const
{
  double potential = 0.0;
  if (y < 0.5) {
    if (y >= 0.0) {
      // upper half
      if (x < 0.5) {
	// upper left corner
	if (x >= 0.0) {
	  potential = get_potential(x, y, &_upper_left_roundness);
	} else {
	  Log::fatal("Rounded_square_shape::get_potential(): x < 0.0");
	}
      } else {
	// upper right corner
	if (x <= 1.0) {
	  potential = get_potential(x, y, &_upper_right_roundness);
	} else {
	  Log::fatal("Rounded_square_shape::get_potential(): x > 1.0");
	}
      }
    } else {
      Log::fatal("Rounded_square_shape::get_potential(): y < 0.0");
    }
  } else {
    // lower half
    if (y <= 1.0) {
      if (x < 0.5) {
	// lower left corner
	if (x >= 0.0) {
	  potential = get_potential(x, y, &_lower_left_roundness);
	} else {
	  Log::fatal("Rounded_square_shape::get_potential(): x < 0.0");
	}
      } else {
	// lower right corner
	if (x <= 1.0) {
	  potential = get_potential(x, y, &_lower_right_roundness);
	} else {
	  Log::fatal("Rounded_square_shape::get_potential(): x > 1.0");
	}
      }
    } else {
      Log::fatal("Rounded_square_shape::get_potential(): y > 1.0");
    }
  }
  return potential;
}

QBrush *
Rounded_square_shape::get_brush(const double x, const double y) const
{
  const double potential = get_potential(x, y);
  QBrush *brush;
  if (potential == _foreground_potential) {
    brush = _foreground;
  } else if (potential == _background_potential) {
    brush = _background;
  } else {
    Log::fatal("Rounded_square_shape::get_brush(): unexpected potential");
  }
  return brush;
}

const double
Rounded_square_shape::get_avg_tan(const double block_offset_x,
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
