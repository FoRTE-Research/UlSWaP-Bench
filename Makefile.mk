ifeq (,$(findstring $(ARCH), riscv arm native))
$(error Usage: make ARCH={riscv|arm|native} [PLATFORM={device|spike}])
endif

ARCH_DIR=../$(ARCH)
include $(ARCH_DIR)/Makefile.mk
