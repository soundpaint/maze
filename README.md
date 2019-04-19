# maze
A maze / flipper game implementation for a Raspberry Pi with Sense Hat

## Implicit Curve Shapes

The algorithmic key feature of this game implementation is the way the
ball's reflection is computed.  Most standard implementations use
segments of straight line as wall, resulting in an angle of reflection
being equal to the angle of incidence.  For computing reflections on
_curved_ lines, one would have to compute the tangent of the curve in
the point where the ball hits the curve.

This game implementation follows a different approach: Walls are
described by shapes of _implicit curves_.  Implicit curves have the
neat property that the equation of the tangent line at a regular point
(_x_<sub>0</sub>, _y_<sub>0</sub>) is

  _F_<sub>_x_</sub>(_x_<sub>0</sub>,
  _y_<sub>0</sub>)(_x_ − _x_<sub>0</sub>) +
  _F_<sub>_y_</sub>(_x_<sub>0</sub>,
  _y_<sub>0</sub>)(_y_ − _y_<sub>0</sub>) = 0,

such that the slope is

  -_F_<sub>_x_</sub>(_x_<sub>0</sub>, _y_<sub>0</sub>) /
  _F_<sub>_y_</sub>(_x_<sub>0</sub>, _y_<sub>0</sub>).

From the slope, the angle of reflection can be very simply computed
from the angle of incidence by adding twice the difference between the
slope's angle and the angle of incidence to the angle of incidence.

This implementation supports 2nd degree polynomial implicit curves,
and intersection (operator "and") and union (operator "or") and
negation (operator "not") of such curves.  The game field is built by
_tiling_, that is by defining a set of different tiles and then
building the field by any combination of the tiles in the set.  For
each tile, a shape is defined by an implicit curve.  For example, the
following lines

```
  <shape id="full_small_centered_circle">
    <!-- (x-½)² + (y-½)² ≤ ¼ -->
    <implicit-curve>x*x - x + 0.25 + y*y - y + 0.25 - 0.25</implicit-curve>
  </shape>
```

define the shape for a tile that consists of a small filled circle
that is centered within the tile.  Implicit curves are always assumed
to have the form _F_(_x_, _y_)≤0, such that it is sufficient to specify the
left side of the equation.

Other examples:

### Empty Shape

The constant equation 1≤0 is never true for any (_x_, _y_) (since
neither _x_ nor _y_ appears in the equation), such that this shape
will result in an empty tile.

```
  <shape id="empty_tile">
    <!-- 1 ≤ 0 -->
    <implicit-curve>1</implicit-curve>
  </shape>
```

### Solid Shape

The constant equation -1≤0 is always true for any (_x_, _y_) (since
neither _x_ nor _y_ appears in the equation), such that this shape
will result in a solid filled tile.

```
  <shape id="solid_tile">
    <!-- - 1 ≤ 0 -->
    <implicit-curve>-1</implicit-curve>
  </shape>
```

### Shape With Circle in Upper Left Corner

```
  <shape id="circle_in_upper_left_corner">
    <and>
      <!-- (x-1)² + (y-1)² ≤ 1 -->
      <implicit-curve>x*x - 2*x + 1 + y*y - 2*y + 1 - 1</implicit-curve>
      <!-- x + y ≤ 1 -->
      <implicit-curve>x + 2*y - 1</implicit-curve>
    </and>
  </shape>
```

## Background and Foreground Brushes

Effectively, each implicit curve defines for each screen pixel, if it
should be displayed as foreground (when _F_(_x_, _y_)≤0) or otherwise
as background (when _F_(_x_, _y_)>0).

For painting foreground and background, brushes can be defined.  As a
special feature, besides solid brushes and brushes based on image art
work, this game implementation also supports fractals as brushes by
specifying a region of either the Mandelbrot or the Julia set, for
example:

```
  <brush id="background_mandelbrot_1">
    <fractal>
      <mandelbrot />
      <max-iterations>256</max-iterations>
      <x-offset>-1.360</x-offset>
      <y-offset>-0.095</y-offset>
      <x-scale>+0.06125</x-scale>
      <y-scale>+0.06125</y-scale>
    </fractal>
  </brush>
```

or

```
  <brush id="background_julia_1">
    <fractal>
      <julia>
        <arg-n>7</arg-n>
        <arg-c>
          <real>+0.626</real>
          <imag>+0.0</imag>
        </arg-c>
      </julia>
      <max-iterations>4096</max-iterations>
      <x-offset>-3.0</x-offset>
      <y-offset>-3.0</y-offset>
      <x-scale>+6.0</x-scale>
      <y-scale>+6.0</y-scale>
    </fractal>
  </brush>
```
## Tiles

Tiles are defined by referring to a shape and optionally overriding
default foreground and / or background brushes, for example:

```
  <tile id="hole">
    <foreground>
      <brush>
        <solid>#99AA00</solid>
      </brush>
    </foreground>
    <shape ref="full_small_centered_hole" />
  </tile>
```

## Game Field

Finally, the game field is composed of a rectangular set of tiles.
Tiles may be referred to either by their full name, or by a shortcut,
such that the full field may be visualized as ASCII art for easier
editing.  The field specification also contains the initial ball
position(s) and velocity(ies) and mass(es).  Example:

```
  <field>
    <ignore>&#x09;</ignore>
    <ignore>&#x0a;</ignore>
    <ignore>&#x0d;</ignore>
    <ignore>&#x20;</ignore>
    <tile-shortcut id="╭" ref="wall-rounded-upper-left" />
    <tile-shortcut id="╰" ref="wall-rounded-lower-left" />
    <tile-shortcut id="╮" ref="wall-rounded-upper-right" />
    <tile-shortcut id="╯" ref="wall-rounded-lower-right" />
    <tile-shortcut id="┼" ref="wall-solid" />
    <tile-shortcut id="╳" ref="corridor" />
    <tile-shortcut id="o" ref="circle" />
    <tile-shortcut id="0" ref="hole" />
    <columns>32</columns>
    <rows>16</rows>
    <contents>
      ┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼
      ┼┼┼┼┼┼┼┼┼┼┼╯╳╳╳╳╰┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼
      ┼┼╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼┼┼┼┼┼┼┼┼┼╮╳╳╳╳╭┼┼╮╳╳╳╳╳0╳╳╳╳┼┼
      ┼┼╳╳┼┼╳╳╳╳┼┼╳╳╳╳┼┼┼┼┼┼╮╳╳╳╳╳╳╳┼┼
      ┼┼╳╳┼┼╳╳╭┼┼┼╳╳╳╳┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼
      ┼┼╳╳╰╯╳╳╰┼┼╯╳╳╳╳╰┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼
      ┼┼╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼╳╳╳o╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼╳╳╳╳╳╳╭┼┼┼┼┼┼┼┼┼┼┼┼┼┼╮╳╳╭╮╳╳┼┼
      ┼┼╳╳╳╳╳╳╰┼┼┼┼┼┼┼┼┼┼┼┼┼┼╯╳╳╰╯╳╳┼┼
      ┼┼╳╳╳╳╳╳╳╳╳╳┼┼┼┼╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼╳╳╳╳╳╳╳╳╳╳┼┼┼┼╳╳╳╳╳╳╳╳╳╳╳╳╳╳┼┼
      ┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼╮╳╳┼┼
      ┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼
    </contents>
    <ball>
      <position>
        <row>4</row>
        <column>12</column>
      </position>
      <velocity>
        <x>0.000020</x>
        <y>0.000011</y>
      </velocity>
      <mass>1.0</mass>
    </ball>
  </field>
```

Tile shortcuts are specified within the ``field`` elment rather on the
``tile`` element, since you may want the same tile to appear with a
different shortcut for each field definition.  (Currently, only one
field can be defined, but as soon as the game will support multiple
levels, there will also be multiple field definitions).

## Current Implementation Status of Reflection Computation

The computation of ball reflections already works fine on the level of
tiles for any 2nd degree polynomial implicit curve.

However, consider the special case of a solid tile.  When a ball hits
such a tile excatly on one of its corner points with a degree of 45°,
the reflection will be equal to the arithmetic means of the
reflections of either adjacent side of that corner.  This is fine for
an isolated tile.  However, if there is a row of multiple adjacent
solid tiles, they together form a straight line segment as wall, but
when the ball again hits a corner pixel of one of these tiles,
building the arithmetic means of adjacent side reflections will still
occur, resulting in the ball being reflected with an improper
reflection angle.

There is an obvious solution to this problem: When calculating the
angle of reflection, tiles may not be considered separately, but also
adjacent tiles must be taken into consideration.  More precisely, for
each pixel on the border of a tile (and, in particular, for each pixel
in one of the four corners of a tile), the shape of the neighbouring
tile must be taken into account.  This requirement can be easily
implemented by combining the two adjacent shapes just like the "and"
operator, that is already implemented for specifying shapes.  I have
not yet implemented this solution, but it should be doable in a
straight forward manner, as soon as I will have time to continue this
implementation of a maze / flipper game.
