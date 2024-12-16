#!env make
.POSIX:

LIBS = \
	lib/libutils.a

include config.mk

.SUFFIXES:
.PHONY: all tests clean $(LIBS)

all: $(LIBS)

lib/libutils.a: libs.mk source/source.mk
	$(MAKE) -f libs.mk SUBDIR=source

tests:
	$(MAKE) -f libs.mk SUBDIR=source tests

clean:
	rm -rf bin lib $(builddir)
