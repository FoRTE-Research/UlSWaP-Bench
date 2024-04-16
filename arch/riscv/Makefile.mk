ifeq (,$(findstring $(MAX_PRINTF_LEVEL), DOUBLE FLOAT LONG_LONG INTEGER MINIMAL))
$(error MAX_PRINTF_LEVEL needs to be specified in benchmark Makefile)
endif

FRANKEN_LLVM_ROOT = $(HOME)/llvm-project
PICOLIBC_ROOT = $(HOME)/picolibc

CC_PATH = $(FRANKEN_LLVM_ROOT)/build/bin
CC = $(CC_PATH)/clang
TOOLCHAIN = riscv64-unknown-elf
LIB_PATH = $(PICOLIBC_ROOT)/franken-install/picolibc/rv32imafdc-unknown-elf
OPTLVL = -O3 -fno-inline

TARGET = riscv32-unknown-elf
RV_ARCH = $(shell cat $(HOME)/arch)

GENERAL_FLAGS = $(OPTLVL) -Wall -DBARE_METAL -DDEBUG=0 -DVERILATOR=0 \
				-fno-builtin -ffreestanding -fomit-frame-pointer -fno-optimize-sibling-calls -fno-builtin-fma -ffp-contract=off
RISCV_FLAGS = --target=$(TARGET) -march=$(RV_ARCH) -mno-relax
PICOLIBC_FLAGS = -DPICOLIBC_$(MAX_PRINTF_LEVEL)_PRINTF_SCANF
FLAGS = $(GENERAL_FLAGS) $(RISCV_FLAGS) $(PICOLIBC_FLAGS) $(BENCHMARK_FLAGS)

LIBS = -lc -lm
OBJS := vectors.o putget.o supportFuncs.o ../common.o $(OBJS)
LINKDIR = -L$(LIB_PATH)/lib -L$(FRANKEN_LLVM_ROOT)/build-rt/lib/linux
INCLIB = -I$(LIB_PATH)/include

all: main.elf

%.o: %.S
	$(CC) $(FLAGS) $(INCLIB) -Wno-unused-command-line-argument -c -o $@ $<

%.o: $(ARCH_DIR)/%.S
	$(CC) $(FLAGS) $(INCLIB) -Wno-unused-command-line-argument -c -o $@ $<

%.o: %.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $<

%.o: $(ARCH_DIR)/%.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $<

main.elf: $(OBJS) $(ARCH_DIR)/vectors.S $(ARCH_DIR)/putget.S $(ARCH_DIR)/supportFuncs.c
	$(CC) $(FLAGS) -T $(ARCH_DIR)/memmap.ld $(LINKDIR) $(OBJS) $(LIBS) -o main.elf
	$(TOOLCHAIN)-objdump -d main.elf > main.lst
	$(TOOLCHAIN)-objcopy main.elf main.bin -O binary

clean: more_clean
	rm -rf *.o *.elf *.lst *.bin *~ ../*.o
