# Maze -- A maze / flipper game implementation for RPi with Sense Hat
# Copyright (C) 2016, 2017  Jürgen Reuter
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see
# <https://www.gnu.org/licenses/>.
#
# For updates and more info or contacting the author, visit:
# <https://github.com/soundpaint/maze>
#
# Author's web site: www.juergen-reuter.de

include qt5.mk

MY_BIN_FILES = maze

MY_GEN_SRC_FILES = \
  ABOUT.h COPYING.h

MY_QT5_OBJ_FILES = \
  about-dialog.o license-dialog.o main-window.o maze.o message-overlay.o \
  playing-field.o simulation.o status-line.o

MY_MOC_FILES = $(patsubst %.o,%.moc.o,$(MY_QT5_OBJ_FILES))

MY_OBJ_FILES = \
  $(MY_GEN_SRC_FILES) \
  ball.o balls.o bivariate-quadratic-function.o brush-field.o config.o \
  force-field.o fractals-factory.o implicit-curve.o \
  implicit-curve-compiler.o implicit-curve-ast.o implicit-curve-parser.o \
  implicit-curve-parser-token.o implicit-curve-tokenizer.o log.o \
  maze-config.o point-3d.o sensors.o shape-expression.o shape-symbols.o \
  sobel.o tile.o tile-symbols.o xml-document.o xml-node-list.o xml-string.o \
  xml-utils.o \
  $(MY_QT5_OBJ_FILES)

LIB_OBJ_FILES =

MY_INCLUDE_DIRS = \
  -I$(QT5_INCLUDE_DIR) \
  -I.

MY_LIB_DIRS =

MY_LIBS = \
  -lpthread -lxerces-c -lxalan-c -lxalanMsg -lQt5Core -lQt5Widgets -lQt5Gui -lm

LOCAL_CXX_OPTS = \
  -fPIC

LOCAL_LD_OPTS = \
  -fPIC $(MY_MOC_FILES)

all: $(MY_BIN_FILES)

ABOUT.h: ABOUT.html
	xxd -i $< > $@

COPYING.h: COPYING.html
	xxd -i $< > $@

include common.mk

.SECONDARY: #keep intermediate files

.SUFFIXES:

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
