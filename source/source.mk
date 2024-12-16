LIB = lib/libutils.a

HDRS = \
	include/utils.h

SRCS = \
	source/getopt.c

TESTSRCS = \
	tests/test-getopt.c

CFLAGS += \
	-Iinclude -I$(libutf8_INCLUDE) -D_POSIX_C_SOURCE=200809L
