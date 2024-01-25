VERSION = 1.0

CC = cc
PP = cpp
PROVE = prove

BASE = ../..
BUILDDIR = build

CFLAGS = -g -std=c99 -Wall -Wshadow
CFLAGS += -DVERSION=\"$(VERSION)\"
