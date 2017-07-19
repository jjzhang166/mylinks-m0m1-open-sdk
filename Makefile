#
# Top level Makefile
#
__REV=937
TOPDIR=$(shell pwd)
LIBPATH=$(TOPDIR)/lib
export TOPDIR LIBPATH __REV
PROJ_PATH := $(shell find proj/ -maxdepth 1 -type d)
PROJ_DIR := $(notdir $(PROJ_PATH))
PROJ_DIR_CLEAN := $(PROJ_DIR:%=clean-%)
FIRM_CONVERT := $(TOPDIR)/utility/convert_firm.py

BUILT_H	= $(TOPDIR)/include/built_info.h
ifeq ($(BUILT_H),$(wildcard $(BUILT_H)))
	BUILT_C=$(shell sed -n 's/.*_COUNT\s\([0-9]*\)/\1/p' $(BUILT_H))
else
	BUILT_C=0
endif
$(BUILT_H):
	@echo "/*" > $@
	@echo "		built information" >> $@
	@echo "*/" >> $@
	@echo "#define SW_BUILD_TIME \"$(shell date)\""  >> $@
	@echo "#define SW_BUILD_COUNT $(shell expr $(BUILT_C) + 1)" >> $@

#
# When we release it to customer, use sdk_toolchain.mak and they will 
# unzip ba-elf_4.7.3.tgz in toolchain dir.
#
# When developing internal, just simply install toolchain in system dir.
#
-include toolchain.mak
-include rules.mak
-include develop.mak

$(TOPDIR)/utility/chksum: $(TOPDIR)/utility/chksum.c
	rm -rf $(TOPDIR)/utility/chksum
	gcc -o $@ $<

$(PROJ_DIR) : $(BUILT_H) $(TOPDIR)/utility/chksum
	$(Q)$(MAKE) -C proj/$@ all

$(PROJ_DIR_CLEAN) :
	$(Q)$(MAKE) -C proj/$(@:clean-%=%) clean

firm_arr :
	$(Q)$(FIRM_CONVERT) $(TOPDIR) $(__REV)

update_all_defconfig:
	for file in `find config -type f \( -name *_lib_defconfig -and ! -name \*svn\* \)`; do cp $$file .lib_config; utility/mconf utility/lib_config.in lib save; cp .lib_config $$file; done
	cp config/asic_rlib_defconfig .rlib_config; utility/mconf utility/rlib_config.in rlib save; cp .rlib_config config/asic_rlib_defconfig
.PHONY: $(BUILT_H)
