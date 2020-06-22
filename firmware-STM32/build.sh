#!/bin/bash

echo "Building main firmware"
mkdir -p build
cd build
cmake .. -DWITH_BOOTLOADER_SUPPORT=ON || exit
make || exit

echo "Building bootloader"
cd ../../bootloader-STM32
mkdir -p build
cd build
cmake .. || exit
make || exit

cd ../../firmware-STM32
cp ../bootloader-STM32/build/bootloader-STM32.bin build

cd build
cp bootloader-STM32.bin combined-STM32.bin
truncate -s $((0x4000)) combined-STM32.bin
cat firmware-STM32.bin >> combined-STM32.bin

echo "Bins are present in ./build/. Use flash.sh to flash to a device."

