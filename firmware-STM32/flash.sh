#!/bin/bash

echo "Flashing combined bootloader+firmware"
openocd -f interface/stlink-v2.cfg -f apm32f1x.cfg -c "program build/combined-STM32.bin verify reset exit 0x08000000"
