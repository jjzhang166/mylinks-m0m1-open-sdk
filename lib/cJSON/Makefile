include $(TOPDIR)/.lib_config

OBJS += cJSON.o
EXTRA_CFLAGS += -I$(TOPDIR)/include/

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
