################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/tinyusb/src/portable/st/synopsys/subdir.mk
-include Drivers/tinyusb/src/portable/st/stm32_fsdev/subdir.mk
-include Drivers/tinyusb/src/device/subdir.mk
-include Drivers/tinyusb/src/common/subdir.mk
-include Drivers/tinyusb/src/class/vendor/subdir.mk
-include Drivers/tinyusb/src/class/usbtmc/subdir.mk
-include Drivers/tinyusb/src/class/msc/subdir.mk
-include Drivers/tinyusb/src/class/midi/subdir.mk
-include Drivers/tinyusb/src/class/hid/subdir.mk
-include Drivers/tinyusb/src/class/dfu/subdir.mk
-include Drivers/tinyusb/src/class/cdc/subdir.mk
-include Drivers/tinyusb/src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
firmware-STM32.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
firmware-STM32.list \

OBJCOPY_BIN += \
firmware-STM32.bin \


# All Target
all: firmware-STM32.elf secondary-outputs

# Tool invocations
firmware-STM32.elf: $(OBJS) $(USER_OBJS) /Users/tom/Git/cz20-badge/firmware-STM32/STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "firmware-STM32.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"/Users/tom/Git/cz20-badge/firmware-STM32/STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="firmware-STM32.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

firmware-STM32.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "firmware-STM32.list"
	@echo 'Finished building: $@'
	@echo ' '

firmware-STM32.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "firmware-STM32.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
