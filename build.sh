#!/bin/bash
set -xeuo pipefail

ARGS=()

if [[ "${1:-}" == "--debug" ]]
then
  ARGS=(-D_DEBUG)
fi

mkdir -p EFI/BOOT

gcc -I/usr/include/efi -I/usr/include/efi/x86_64 -fpic -fshort-wchar -mno-red-zone -fno-stack-protector -DEFI_FUNCTION_WRAPPER "${ARGS[@]}" -c tme_msr.c -o tme_msr.o
ld -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic tme_msr.o /usr/lib/crt0-efi-x86_64.o -L/usr/lib -lgnuefi -lefi -o tme_msr.so
objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 tme_msr.so EFI/BOOT/BOOTX64.EFI
