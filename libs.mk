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

OBJS != echo $(SRCS:.c=.o) | sed -e 's/$(SUBDIR)\//$(builddir)\/$(SUBDIR)\//g'
DEPS != echo $(SRCS:.c=.d) | sed -e 's/$(SUBDIR)\//$(builddir)\/$(SUBDIR)\//g'
PPS != echo $(SRCS:.c=.c.pp) | sed -e 's/$(SUBDIR)\//$(builddir)\/$(SUBDIR)\//g'
TESTS != echo $(TESTSRCS:.c=) | sed -e 's/tests\//$(builddir)\//g'

.SUFFIXES:
.PHONY: all tests clean

all: $(LIB)

$(LIB): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $^

$(builddir)/%.o: %.c
	@mkdir -p $(@D)
	$(PP) $(CFLAGS) $< > $(builddir)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(builddir)/$*.d -c -o $@ $<

tests: $(TESTS) $(LIB)
	$(PROVE) $(PROVE_FLAGS) $(TESTS)

$(builddir)/%: tests/%.c $(LIB)
	@mkdir -p $(builddir)/$(*D)
	$(PP) $(CFLAGS) $< > $(builddir)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(builddir)/$*.d -o $@ $^

clean:
	rm -rf $(builddir)/$(SUBDIR) $(LIB) $(TESTS)

-include $(DEPS)
