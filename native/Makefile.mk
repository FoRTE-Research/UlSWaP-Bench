CC = cc
LIBS = -lc -lm
FLAGS = -O3 -static -v -Wall -g
OBJS := supportFuncs.o ../common.o $(OBJS)

all: main.elf

%.o: %.s
	$(CC) $(FLAGS) -c -o $@ $<

%.o: $(ARCH_DIR)/%.s
	$(CC) $(FLAGS) -c -o $@ $<

%.o: %.c
	$(CC) ${FLAGS} -c -o $@ $<

%.o: $(ARCH_DIR)/%.c
	$(CC) ${FLAGS} -c -o $@ $<

main.elf: $(OBJS) $(ARCH_DIR)/supportFuncs.c
	$(CC) $(FLAGS) $(OBJS) $(LIBS) -o main.elf
	objdump -d main.elf > main.lst
	objcopy main.elf main.bin -O binary

clean: more_clean
	rm -rf *.o *.elf output* *.lst *.bin *~ ../*.o
