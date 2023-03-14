TC_PATH = /home/harrison/ti/msp430-gcc
TOOLCHAIN = msp430-elf
CC = $ msp430-elf-gcc
#DEVICE = msp430g2553
DEVICE = msp430fr5994

# MSP430FR5994 by default reserves have of SRAM for LEA, which we generally don't want
ifeq ($(DEVICE), msp430fr5994)
DEVICE_LD = ../ld/$(DEVICE)_no_lea.ld
else
DEVICE_LD = ../ld/$(DEVICE).ld
endif

FLAGS = -DBARE_METAL -Wall -mmcu=$(DEVICE) -fdata-sections -ffunction-sections -T $(DEVICE_LD) -ffreestanding
LIBS = -lnosys -lc -lm
OBJS := putget.o supportFuncs.o LED.o $(OBJS)
LINKDIR = -L$(TC_PATH)/include
INCLIB = -I$(TC_PATH)/include

all: main.elf

%.o: %.s
	$(CC) $(FLAGS) $(INCLIB) -c -o $@ $<

%.o: ../%.s
	$(CC) $(FLAGS) $(INCLIB) -c -o $@ $<

%.o: %.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $< 

%.o: ../%.c
	$(CC) ${FLAGS} $(INCLIB) -c -o $@ $< 

main.elf: $(OBJS) ../putget.s ../supportFuncs.c ../LED.s
	$(CC) $(FLAGS) $(LINKDIR) $(INCLIBS) $(OBJS) -o main.elf $(LIBS)
	$(TOOLCHAIN)-objdump -d main.elf > main.lst
	$(TOOLCHAIN)-objcopy main.elf main.bin -O binary

clean: more_clean
	rm -rf *.o *.elf output* *.lst *.bin *~

