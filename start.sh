#!/bin/bash

clear

COMPILER="/usr/local/i386elfgcc/bin"


mkdir -p "./orgs"
if [ $? -eq 0 ]; then
    echo "Directory ./orgs created successfully."
else
    echo "Failed to create directory ./orgs."
    exit 1
fi

# Delete old origin files to ensure nothig not supposed to gets liked by the linker
echo "Deleting old orgs..."
for f in ./orgs/*.o ; do
    sudo rm -f "$f"
done

echo "Compiling bootloader..."
cd bootloader
nasm "boot.asm" -f bin -o "stage_1.bin"
nasm "stage_2.asm" -f bin -o "stage_2.bin"
cat "stage_1.bin" "stage_2.bin" > "../boot.bin"
rm "stage_1.bin" "stage_2.bin"
cd ..



# Compile kernel files
for dir in ./kernel ./lib ./drivers; do
    for in_file in "$dir"/*.c; do
        if [[ -f "$in_file" ]]; then
            echo "Compiling: $in_file"

            basename=$(basename "$in_file" ".cpp")

            out_file="./orgs/${basename}_C.o"

            "$COMPILER/i386-elf-gcc" -ffreestanding -m32 -g -c -Wno-write-strings  "$in_file" -o "$out_file"
        fi
    done

    for in_file in "$dir"/*.asm; do
        if [[ -f "$in_file" ]]; then
            echo "Compiling: $in_file"

            basename=$(basename "$in_file" ".asm")

            out_file="./orgs/${basename}_ASM.o"

            nasm "$in_file" -f elf -o "$out_file"
        fi
    done
done

# Chaining all file names in the same string to pass them as an argument to the linker
kernel_files="./orgs/kernel_entry_ASM.o"
for in_file in ./orgs/*.o; do
    basename=$(basename "$in_file" ".o")
    if [[ -f "$in_file" ]] && [[ "$basename" != "kernel_entry_ASM" ]]; then
        kernel_files="$kernel_files $in_file"
    fi
done

echo "Linking..."

"$COMPILER/i386-elf-ld" -T "linker.ld"  -Ttext 0x1000 -o "full_kernel.bin" $kernel_files --oformat binary


cat "boot.bin" "full_kernel.bin" > "everything.bin"
cat "everything.bin" "zeros.bin" > "OS.bin"
rm "boot.bin" "full_kernel.bin" "everything.bin"

sudo dd if=/dev/zero of=OS.iso bs=1M count=10
dd if=OS.bin of=OS.iso conv=notrunc





echo "Starting virtual machine"
#qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
qemu-system-x86_64 -drive format=raw,file="OS.iso",index=0,if=ide -m 2G
#qemu-system-x86_64 -cdrom OS.iso -m 128M
