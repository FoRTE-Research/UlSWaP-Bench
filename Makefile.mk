ifeq (,$(findstring $(ARCH), arm))
$(error Usage: make ARCH=arm)
endif

ARCH_DIR=../$(ARCH)
include $(ARCH_DIR)/Makefile.mk
