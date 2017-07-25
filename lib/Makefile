-include $(TOPDIR)/.lib_config
export TOPDIR 

EXTRA_CFLAGS = -I$(TOPDIR)/include -include $(TOPDIR)/include/lib_autoconf.h
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/include
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/include/ipv4
EXTRA_CFLAGS += -I$(TOPDIR)/include/mico
export EXTRA_CFLAGS


-include libconf.mak

LIBDIRS = $(libdirs-y) $(libdirs-m) $(libdirs-e)
LIBS = $(LIBDIRS:%=%.a) 

$(LIBDIRS:%=_dir_%) :
	@if [ -e $(@:_dir_%=%)/Makefile ]; then echo "\n      [$(@:_dir_%=%)] .."; rm -f $(@:_dir_%=%).a; \
	$(MAKE) -C $(@:_dir_%=%) clean ; $(MAKE) -C $(@:_dir_%=%) CFLAGS="$(CFLAGS)" all; fi
all: $(LIBDIRS:%=_dir_%)

-include $(TOPDIR)/rules.mak

.PHONY: all
