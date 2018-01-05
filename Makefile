CC:=avr-gcc

CFLAGS+= \
	-std=c99 -g \
	-ffunction-sections \
	-fdata-sections \
	-fno-strict-aliasing \
	-Wunused-function \
	-Werror \
	-I. \
	-Iapp \

TARGET:=app/main.c

MMCU:=atmega2560

MMCU_SHORT:=m2560

COM_PORT:=/dev/tty.usbmodem1411

make: $(TARGET)
	@$(CC) -g -Os -mmcu=$(MMCU) -c $(TARGET) $(CFLAGS)
	@#-p will ignore error if folder already exists
	@mkdir -p out
	@$(CC) -g -mmcu=$(MMCU) -o program.elf main.o $(CFLAGS)
	@avr-objcopy -j .text -j .data -O ihex program.elf program.hex
	@mv *.o out
	@mv *.elf out
	@mv *.hex out

	@echo 'Successfully built project'

send:
	avrdude -p $(MMCU_SHORT) -c stk500v2 -P $(COM_PORT) -b 115200 -F -U flash:w:out/program.hex

.PHONY: send

clean:
	@echo 'Removing out folder'
	@rm -rf out/

.PHONY: clean







