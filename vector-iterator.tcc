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

#ifndef VECTOR_ITERATOR_TCC
#define VECTOR_ITERATOR_TCC

#include <vector-iterator.hh>

template<class T>
Vector_iterator<T>::Vector_iterator (std::vector<T> const *elements)
  : _pos (0), _elements (elements)
{
}

template<class T>
bool
Vector_iterator<T>::has_next () const
{
  return
    _pos < _elements->size ();
}

template<class T>
T const
Vector_iterator<T>::get_next ()
{
  assert (has_next ());
  return
    _elements->at (_pos++);
}

#endif // VECTOR_ITERATOR_TCC

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
