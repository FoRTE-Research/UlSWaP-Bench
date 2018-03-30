TC_PATH = /usr
TOOLCHAIN = arm-none-eabi
ARMGNU = $(TC_PATH)/bin/$(TOOLCHAIN)
CC = $ arm-none-eabi-gcc
CHIP = cortex-m0plus
OPTLVL = -O3 

#FLAGS = -DBARE_METAL -Wall $(OPTLVL) -target $(TOOLCHAIN) -mcpu=$(CHIP) -mthumb --specs=nosys.specs -nostartfiles -ffreestanding -std=c99 -fomit-frame-pointer -fno-optimize-sibling-calls #-static
FLAGS = -DBARE_METAL -Wall $(OPTLVL) -march=armv6-m -mcpu=$(CHIP) -mthumb -msoft-float --specs=nosys.specs -nostartfiles -ffreestanding -std=c99 -fomit-frame-pointer -fno-optimize-sibling-calls 
#LIBS = --start-group -lm -lc -lbuiltins --end-group
LIBS = -lnosys -lc -lm
OBJS := vectors.o putget.o supportFuncs.o LED.o $(OBJS)
#LINKDIR=-L$(TC_PATH)/$(TOOLCHAIN)/lib
LINKDIR=
INCLIB = -I$(TC_PATH)/$(TOOLCHAIN)/include 

all: main.elf 

%.o: %.s
	$(CC) $(FLAGS) $(INCLIB) -c -o $@ $<

%.o: ../%.s
	$(CC) $(FLAGS) $(INCLIB) -c -o $@ $<

%.o: %.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $< 

%.o: ../%.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $< 

main.elf: $(OBJS) ../vectors.s ../putget.s ../supportFuncs.c ../LED.s
	$(CC) $(FLAGS) -u _printf_float -T ../memmap $(LINKDIR) $(OBJS) -o main.elf $(LIBS)
	$(TOOLCHAIN)-objdump -d main.elf > main.lst
	$(TOOLCHAIN)-objcopy main.elf main.bin -O binary

clean: more_clean
	rm -rf *.o *.elf output* *.lst *.bin *~
