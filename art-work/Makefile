include ../defs.mk

ROOT=..
BUILD_ART_WORK=$(BUILD)/art-work
SRC=$(wildcard *.dia)
OBJ_SVG=$(patsubst %,$(BUILD_ART_WORK)/%,$(SRC:.dia=.png))
OBJ=$(OBJ_SVG)

all: $(BUILD_ART_WORK) $(OBJ)

$(BUILD_ART_WORK):
	mkdir -p $@

$(BUILD_ART_WORK)/%.png: %.dia
	dia --export=$@ $<

coreclean:
	rm -f core core.* vgcore.*

objclean:
	rm -rf $(BUILD_ART_WORK)

bkpclean:
	rm -f *~

clean: objclean bkpclean
