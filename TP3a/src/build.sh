#!/bin/bash

echo "[1/3] Compilando aplicacion.c a código objeto..."
gcc -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol \
    -fpic -ffreestanding -fno-stack-protector -fno-strict-aliasing \
    -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -Wall \
    -c -o aplicacion.o aplicacion.c

if [ $? -ne 0 ]; then echo "Error en la compilación."; exit 1; fi

echo "[2/3] Linkeando y generando objeto compartido (.so)..."
ld -shared -Bsymbolic -L/usr/lib -L/usr/lib/efi \
   -T /usr/lib/elf_x86_64_efi.lds /usr/lib/crt0-efi-x86_64.o \
   aplicacion.o -o aplicacion.so -lefi -lgnuefi

if [ $? -ne 0 ]; then echo "Error en el linkeo."; exit 1; fi

echo "[3/3] Convirtiendo a ejecutable PE/COFF (.efi)..."
objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
        -j .rela -j .rel.* -j .rela.* -j .reloc \
        --target=efi-app-x86_64 aplicacion.so aplicacion.efi

if [ $? -ne 0 ]; then echo "Error en objcopy."; exit 1; fi

echo "¡Éxito! Archivo aplicacion.efi generado correctamente."