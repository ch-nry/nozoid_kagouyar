#!/bin/bash

CHIPSET=stm32h7x
FLASH_ADDRESS=0x08000000
OCD=openocd
OCD_DIR=/usr/local/share/openocd/scripts
PGM_DEVICE=interface/stlink.cfg
OCDFLAGS="-f $PGM_DEVICE -f target/$CHIPSET.cfg"


$OCD -s $OCD_DIR $OCDFLAGS -c "program ./binaire/Kagouyar.elf verify reset exit"
