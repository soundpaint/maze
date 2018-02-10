# Maze -- A maze / flipper game implementation for RPi with Sense Hat
# Copyright (C) 2016, 2017, 2018 Jürgen Reuter
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

ROOT=..

include ../defs.mk
include qt5.mk
include rtimu.mk

SH = sh -c
CPP = c++
MY_CXX_OPTS = -std=c++11 -Wall -g -O0 $(LOCAL_CXX_OPTS)
MY_LD_OPTS = $(LOCAL_LD_OPTS)

LOCAL_CXX_OPTS = \
  -fPIC

LOCAL_LD_OPTS = \
  -fPIC $(MY_MOC_FILES)

MY_BIN_FILES = $(BUILD_BIN)/maze

MY_QT5_OBJ_FILES = \
  about-dialog.o license-dialog.o main-window.o maze.o message-overlay.o \
  playing-field.o sensors.o sensors-display.o simulation.o splash-screen.o \
  status-line.o

MY_MOC_FILES = $(patsubst %.o,$(BUILD)/obj/%.moc.o,$(MY_QT5_OBJ_FILES))

MY_OBJ_FILES = \
  $(patsubst %.o,$(BUILD_OBJ)/%.o, \
  ball.o ball-init-data.o balls.o bivariate-quadratic-function.o \
  brush-field.o config.o force-field.o fractals-brush-factory.o \
  implicit-curve.o implicit-curve-compiler.o implicit-curve-ast.o \
  implicit-curve-parser.o implicit-curve-parser-token.o \
  implicit-curve-tokenizer.o julia-set.o log.o mandelbrot-set.o maze-config.o \
  pixmap-brush-factory.o point-3d.o shape.o shape-expression.o sobel.o \
  solid-brush-factory.o tile.o xml-document.o xml-node-list.o xml-string.o \
  xml-utils.o \
  $(MY_QT5_OBJ_FILES))

LIB_OBJ_FILES =

MY_INCLUDE_DIRS = \
  -I$(QT5_INCLUDE_DIR) \
  -I$(RTIMULIB_INCLUDE_DIR) \
  -I$(BUILD_SRC) \
  -I.

MY_LIB_DIRS =

MY_LIBS = \
  -lpthread -lxerces-c -lxalan-c -lxalanMsg -lQt5Core -lQt5Widgets -lQt5Gui -lm -lRTIMULib

CONFIG_XML=$(BUILD_BIN)/config.xml

all: $(CONFIG_XML) $(MY_BIN_FILES)

$(BUILD_SRC):
	mkdir -p $@

$(BUILD_OBJ):
	mkdir -p $@

$(BUILD_BIN):
	mkdir -p $@

$(CONFIG_XML): $(SRC)/config.xml
	cp -pf $< $@

$(BUILD_OBJ)/%.o: %.cc | $(BUILD_OBJ)
	$(CPP) $(MY_INCLUDE_DIRS) $(MY_CXX_OPTS) -c -o $@ $<

$(BUILD_OBJ)/%.moc.o: $(BUILD_SRC)/%.moc.cc | $(BUILD_OBJ)
	$(CPP) $(MY_INCLUDE_DIRS) $(MY_CXX_OPTS) -c -o $@ $<

$(BUILD_SRC)/%.moc.cc: %.hh | $(BUILD_SRC)
	moc -I. -o $@ $<

$(MY_BIN_FILES): $(MY_OBJ_FILES) $(MY_MOC_FILES) | $(BUILD_BIN)
	$(CPP) $(MY_CXX_OPTS) $(MY_INCLUDE_DIRS) $(LIB_OBJ_FILES) $(MY_OBJ_FILES) $(MY_LD_OPTS) $(MY_LIB_DIRS) $(MY_LIBS) -o $@

objclean:
	rm -f $(MY_OBJ_FILES) $(MY_BIN_FILES) $(MY_MOC_FILES)

bkpclean:
	rm -f *~

coreclean:
	rm -f core core.* vgcore.*

clean: objclean

distclean: objclean bkpclean coreclean

.SECONDARY: $(MY_OBJ_FILES)

.SUFFIXES:

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
