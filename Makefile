TOOLCHAIN=arm-none-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(CC)

LD_FILE=mkl25z4.ld
CC_FLAGS=-c -mcpu=cortex-m0 -mabi=aapcs -mthumb -msoft-float -g3
LD_FLAGS=-mthumb -mcpu=cortex-m0 -T$(LD_FILE)

TARGET := kl25z
TARGET_EXT := elf


C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c,%.o,$(C_SRCS))

%.o : %.c
	@echo 'Building file: $(@F)'
	@echo '--------------------'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(CC_FLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '


all: clean $(TARGET).$(TARGET_EXT)

# Tool invocations
$(TARGET).$(TARGET_EXT): $(C_OBJS)
	@echo 'Building target: $@'
	@echo '-------------------'
	@echo 'Invoking: MCU Linker'
	$(LD) $(LD_FLAGS) $(C_OBJS) -o $(TARGET).$(TARGET_EXT)
	@echo 'Finished building target: $@'
	@echo ' '

clean: 
	rm -f $(TARGET).$(TARGET_EXT) *.o

.PHONY: clean all
