#!env make
.POSIX:

# Global configuration
include config.mk
#
# Source makefile should include lists of these variables:
#    LIB - name of the library
#    HDRS - list of headers to install
#    SRCS - list of source files
#    PKGS - list of dependent libraries
#
# Also it can modify some variables:
#    CFLAGS - build flags for C files
#
include $(SUBDIR)/source.mk

OBJS != echo $(SRCS:.c=.o) | sed -e 's/$(SUBDIR)\//$(BUILDDIR)\/$(SUBDIR)\//g'
DEPS != echo $(SRCS:.c=.d) | sed -e 's/$(SUBDIR)\//$(BUILDDIR)\/$(SUBDIR)\//g'
PPS != echo $(SRCS:.c=.c.pp) | sed -e 's/$(SUBDIR)\//$(BUILDDIR)\/$(SUBDIR)\//g'
TESTS != echo $(TESTSRCS:.c=) | sed -e 's/tests\//$(BUILDDIR)\//g'

.SUFFIXES:
.PHONY: all tests clean

all: $(LIB)

$(LIB): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(PP) $(CFLAGS) $< > $(BUILDDIR)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(BUILDDIR)/$*.d -c -o $@ $<

tests: $(TESTS) $(LIB)
	$(PROVE) $(PROVE_FLAGS) $(TESTS)

$(BUILDDIR)/%: tests/%.c $(LIB)
	@mkdir -p $(BUILDDIR)/$(*D)
	$(PP) $(CFLAGS) $< > $(BUILDDIR)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(BUILDDIR)/$*.d -o $@ $^

clean:
	rm -rf $(BUILDDIR)/$(SUBDIR) $(LIB) $(TESTS)

-include $(DEPS)
