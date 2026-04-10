#!/bin/bash

FILE=A
SIZE=$((4*1024*1024 + 1))

# создать файл нулями
dd if=/dev/zero of=$FILE bs=1 count=$SIZE status=none

# записать единицы в нужные позиции
printf "\x01" | dd of=$FILE bs=1 seek=0 conv=notrunc status=none
printf "\x01" | dd of=$FILE bs=1 seek=10000 conv=notrunc status=none
printf "\x01" | dd of=$FILE bs=1 seek=$((SIZE-1)) conv=notrunc status=none

echo "File A created"