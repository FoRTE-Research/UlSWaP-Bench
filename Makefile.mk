ifeq (,$(findstring $(ARCH), arm native))
$(error Usage: make ARCH={arm|native})
endif

ARCH_DIR=../$(ARCH)
include $(ARCH_DIR)/Makefile.mk
