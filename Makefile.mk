ifeq (,$(findstring $(ARCH), riscv arm native spike))
$(error Usage: make ARCH={riscv|arm|native|spike})
endif

ARCH_DIR=../../arch/$(ARCH)
include $(ARCH_DIR)/Makefile.mk
