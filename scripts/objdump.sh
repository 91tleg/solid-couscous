#!/usr/bin/env bash

FUNC_NAME="${1:-fn}"
OUT_FILE="${FUNC_NAME}.txt"

xtensa-esp32-elf-objdump -d .pio/build/esp32dev/firmware.elf | \
sed -n "/<${FUNC_NAME}>:/,/^$/p" > "$OUT_FILE"