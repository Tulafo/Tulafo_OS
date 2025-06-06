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
    rm -f "$f"
done

echo "Compiling bootloader..."
cd bootloader
nasm "boot.asm" -f bin -o "stage_1.bin"
nasm "stage_2.asm" -f bin -o "stage_2.bin"
cat "stage_1.bin" "stage_2.bin" > "../boot.bin"
rm "stage_1.bin" "stage_2.bin"
cd ..

#boot_filesize=$(stat -c%s ./boot.bin)
#padding_size=$((1048576 - boot_filesize))

#dd if=/dev/zero of=padding.bin bs=1 count=$padding_size
#cat boot.bin padding.bin >> full_boot.bin
#rm boot.bin padding.bin

#mv full_boot.bin boot.bin


# Compile kernel files
for dir in ./kernel ./lib ./drivers; do
    for in_file in "$dir"/*.c "$dir"/*.cpp; do
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

rm OS.bin

echo "Linking..."

"$COMPILER/i386-elf-ld" -T "linker.ld"  -Ttext 0x1000 -o "full_kernel.bin" $kernel_files --oformat binary


cat "boot.bin" "full_kernel.bin" > "everything.bin"

OS_filesize=$(stat -c%s ./everything.bin)
OS_padding_size=$((1048576 - OS_filesize))
dd if=/dev/zero of=OS_padding.bin bs=1 count=$OS_padding_size
cat "everything.bin" "OS_padding.bin" >> "OS.bin"

rm "boot.bin" "full_kernel.bin" "everything.bin" "OS_padding.bin"


dd if=OS.bin of=OS.img bs=512 count=2048 conv=notrunc     # Copies booloader and kernel in reserved sectors



echo "Starting virtual machine"
#qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
qemu-system-x86_64 -drive format=raw,file="OS.img",index=0,if=ide -m 2G
#qemu-system-x86_64 -cdrom OS.iso -m 128M
