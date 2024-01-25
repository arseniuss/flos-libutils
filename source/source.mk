LIB = lib/libutils.a

HDRS = \
	include/utils.h

SRCS = \
	source/getopt.c

TESTSRCS = \
	tests/test-getopt.c

CFLAGS += \
	-Iinclude -I$(BASE)/flos/libutf8/include -D_POSIX_C_SOURCE=200809L
