#!env make
.POSIX:

TARGETS = \
	lib/libutils.a

include config.mk

.SUFFIXES:
.PHONY: all tests clean

all: $(TARGETS)

lib/libutils.a: libs.mk source/source.mk
	$(MAKE) -f libs.mk SUBDIR=source

tests:
	$(MAKE) -f libs.mk SUBDIR=source tests

clean:
	rm -rf bin deps lib $(builddir)
