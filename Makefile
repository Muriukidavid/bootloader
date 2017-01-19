OUT_DIR := out
# add traling / if not empty
ifeq ($(strip $(OUT_DIR)), )
	OBJ_FOLDER =
else
	OBJ_FOLDER = $(strip $(OUT_DIR))/
endif

TOOLCHAIN=arm-none-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(CC)

LD_FILE=mkl25z4.ld
CC_FLAGS=-c -mcpu=cortex-m0 -mabi=aapcs -mthumb -msoft-float -g3
LD_FLAGS=-mthumb -mcpu=cortex-m0 -T$(LD_FILE)

TARGET := kl25z
TARGET_EXT := elf
RM := rm -rf

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c,$(OBJ_FOLDER)%.o,$(C_SRCS))

$(OBJ_FOLDER)%.o : %.c
	@echo 'Building file: $(@F)'
	@echo '--------------------'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(CC_FLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '


all: clean create_outputdir $(TARGET).$(TARGET_EXT)
create_outputdir:
	$(shell mkdir $(OBJ_FOLDER) 2>/dev/null)
# Tool invocations
$(TARGET).$(TARGET_EXT): $(C_OBJS)
	@echo 'Building target: $@'
	@echo '-------------------'
	@echo 'Invoking: MCU Linker'
	$(LD) $(LD_FLAGS) $(C_OBJS) -o $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT)
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	@echo 'Removing entire out directory'
	@echo '-----------------------------'
	$(RM) $(OBJ_FOLDER)*.* $(OBJ_FOLDER)
	@echo ' '

.PHONY: all
