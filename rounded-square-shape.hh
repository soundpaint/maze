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

#ifndef ROUNDED_SQUARE_SHAPE_HH
#define ROUNDED_SQUARE_SHAPE_HH

#include <ishape.hh>

class Rounded_square_shape : public IShape
{
public:
  Rounded_square_shape(const double upper_left_roundness,
		       const double upper_right_roundness,
		       const double lower_left_roundness,
		       const double lower_right_roundness,
		       QBrush *foreground,
		       const double foreground_potential,
		       QBrush *background,
		       const double background_potential);
  virtual ~Rounded_square_shape();
  const double get_potential(const double x, const double y) const;
  QBrush *get_brush(const double x, const double y) const;
  const double get_avg_tan(const double block_offset_x,
			   const double block_offset_y,
			   const double dx,
			   const double dy) const;

private:
  struct roundness_t {
    double edge_x, edge_y;
    double center_x, center_y;
    double squared_radius;
  };
  struct roundness_t _upper_left_roundness;
  struct roundness_t _upper_right_roundness;
  struct roundness_t _lower_left_roundness;
  struct roundness_t _lower_right_roundness;
  QBrush *_foreground;
  double _foreground_potential;
  QBrush *_background;
  double _background_potential;
  void fill_roundness_info(struct roundness_t *roundness,
			   const double roundness_value,
			   const double edge_x,
			   const double edge_y,
			   const char *label);
  void clear_roundness(struct roundness_t *roundness);
  const double get_potential(const double x, const double y,
			     const struct roundness_t *roundness) const;
};

#endif /* ROUNDED_SQUARE_SHAPE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
