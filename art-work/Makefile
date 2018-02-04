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

include ../defs.mk

ROOT=..
BUILD_BIN=$(BUILD)/bin
SRC_DIA=$(wildcard *.dia)
OBJ_DIA_PNG=$(patsubst %,$(BUILD_BIN)/%,$(SRC_DIA:.dia=.png))
SRC_PNG=$(wildcard *.png)
OBJ_PNG=$(patsubst %,$(BUILD_BIN)/%,$(SRC_PNG))
OBJ=$(OBJ_DIA_PNG) $(OBJ_PNG)

all: $(OBJ)

$(BUILD_BIN):
	mkdir -p $@

$(BUILD_BIN)/%.png: %.dia | $(BUILD_BIN)
	dia --export=$@ $<

$(BUILD_BIN)/%.png: %.png | $(BUILD_BIN)
	cp -pf $< $@

coreclean:
	rm -f core core.* vgcore.*

objclean:
	rm -rf $(OBJ)

bkpclean:
	rm -f *~

clean: objclean bkpclean
