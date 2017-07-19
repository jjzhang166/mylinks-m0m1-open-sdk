#
# rules.mak
#

ARCH = ba22
CPU = -march=ba2 -mbe -mabi=3 -mtj-rodata -mno-hard-div -mno-hard-mul -ffixed-r31 -ffixed-r30
CROSS_PREFIX=$(TOOLCHAIN_PATH)ba-elf-
LDFLAGS = -mno-hard-div -mno-hard-mul -mbe -Os
ifeq (1,$(IPO))
CFLAGS += -flto -fwhole-program
LDFLAGS += -flto -fwhole-program -mno-hard-div -mno-hard-mul -mbe
endif

CFLAGS += -fno-strict-aliasing  -fno-common $(CPU) -I. -I./include -Os -g -fregmove
CFLAGS += -DBIG_ENDIAN
CFLAGS += -fno-builtin
#CFLAGS += -msoft-float
CFLAGS += -G0
CFLAGS += -Werror
CFLAGS += $(CFLAGS_GCC)
#
CFLAGS_src/start.o = -Wa,-gstabs
LDFLAGS += -nostartfiles -Wl,-static

CFLAGS += -I$(TOPDIR)/include/freertos -I$(TOPDIR)/lib/lwip/include -I$(TOPDIR)/lib/lwip -I$(TOPDIR)/lib/lwip/include/ipv4 
CFLAGS += -D__REV=$(__REV)

ifndef	V
Q=@
ECHO=@echo
MAKE=make -s
else
Q=
ECHO=@\#
MAKE=make
endif
XCC = $(CROSS_PREFIX)gcc
XLD = $(CROSS_PREFIX)ld
XOC = $(CROSS_PREFIX)objcopy
XNM = $(CROSS_PREFIX)nm
XOD = $(CROSS_PREFIX)objdump
XAR = $(CROSS_PREFIX)ar
XSZ = $(CROSS_PREFIX)size
XSR = $(CROSS_PREFIX)strip
#D2U = dos2unix -k	#red-hat
D2U = dos2unix

# RULES
%.o: %.o.gz
	$(ECHO) "\t[GZIP] $<"
	$(Q)gzip -d -c $< > $*.o
%.a: %.a.gz
	$(ECHO) "\t[GZIP] $<"
	$(Q)gzip -d -c $< > $*.a

%.d: %.o.gz
	$(ECHO) "\t[GZIP] $<"
	$(Q)gzip -d -c $< > $*.o
	
%.d : %.c
	$(ECHO) "\t [CC] $<"
	$(Q)$(XCC) -c  -o $(@:.d=.o) $(CFLAGS) $(EXTRA_CFLAGS) $(MIPS) $(CFLAGS_$(@:.d=.o)) -Wp,-MD,$(@:.d=.tmp) $<
	@sed -e '/^ *\\/d' -e "s#.*: #$@: #" $(@:.d=.tmp) > $@ ; rm $(@:.d=.tmp)
	
%.d : %.S
	$(ECHO) "\t[ASM] $<"
	$(Q)$(XCC) -c  -o $(@:.d=.o) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$(@:.d=.o)) -Wp,-MD,$(@:.d=.tmp) $<
	@sed -e '/^ *\\/d' -e "s#.*: #$@: #" $(@:.d=.tmp) > $@ ; rm $(@:.d=.tmp)

%.img: %
	$(ECHO) "\t[IMG] $<"
	$(Q)$(XOC) -O binary -R .dma $< $@

%.i: %.c
	$(XCC) -E -o $*.i $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) $< 
		
%.map: %
	$(ECHO) "\t[MAP] $<"
	$(Q)$(XNM) $(@:.map=) | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)' | sort > $@

%.dis: %
	$(XOD) -S --show-raw-insn $< > $@

%.dis: %.o
	$(XOD) -S --show-raw-insn $< > $@

%.sz: %
	$(XNM) -r --size-sort $< > $@

im_header.bin: $(LIBPATH)/im_header.c $(TOPDIR)/include/app_hdr.ld
	@$(XCC) -o im_header.o -c -I$(TOPDIR)/include -I./ $(LIBPATH)/im_header.c -DIH_RUN=$(shell sed -n 's/\([0-9a-f]*\) [A-Z] app_main/0x\1/p' app2.map) -DIH_SIZE=$(shell stat -c%s app2.img) -DIH_CHKSUM=0 -DIH_NEXT=8 -DIH_VER=$(__REV) -w -include lib_autoconf.h
	@$(XCC) $(LDFLAGS) -nostdlib -T$(TOPDIR)/include/app_hdr.ld  -o im_header im_header.o
	@$(XOC) -j .data -O binary im_header $@

rp_str1 = "s/CONFIG_DATABUF_START/"$(CONFIG_DATABUF_START)"/g;s/CONFIG_DMA_SIZE/"$(CONFIG_DMA_SIZE)"/g"

proj.ld: $(TOPDIR)/include/app.ld
	sed -e $(rp_str1) $< > $@

lib:
	$(Q)$(MAKE) -C lib all

LIBA = $(notdir $(shell pwd)).a

$(LIBA): $(OBJS:.o=.d)
	$(ECHO) "\t[AR] $@"
	$(Q)$(XAR) rsc $@ $(OBJS)
	mv $@ ../

PROJ_NAME = `pwd | sed 's,.*/,,'`

ifneq ($(MAKECMDGOALS),clean)
ifneq "" "$(OBJS:.o=.d)"
-include $(OBJS:.o=.d)
endif
endif

.PHONY: lib
