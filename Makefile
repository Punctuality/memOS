CC = gcc -fpic -g -O0
ASM = nasm -g
LD = ld

# Custom

deploy-image: recompile clean-object-files

disk: target/os-image
	sh make_disk_image.sh

run: all target/os-image
	qemu-system-i386 -machine type=pc-i440fx-3.1 -kernel target/os-image

debug: all target/os-image
	qemu-system-i386 -machine type=pc-i440fx-3.1 -D log.txt -hda target/disk.img -s -S

debug2: all target/os-image
	qemu-system-i386 -machine type=pc-i440fx-3.1 -D log.txt -kernel target/os-image -s -S

recompile: prep all

all: target/os-image

prep: clean boot/ kernel/ drivers/
	mkdir target

clean:
	rm -rf target/

clean-object-files:
	rm target/*.o
	rm target/*.bin

# Boot

boot/entry.o: boot/entry.asm
	$(ASM) $< -f elf32 -o $@

target/kernel_entry.o: boot/kernel_entry.asm
	$(ASM) $< -f elf32 -o $@

target/bootstrap.bin: boot/bootstrap.asm
	# $(ASM) $< -f bin -o $@
	$(ASM) $< -f elf32 -o $@

boot_targets: target/kernel_entry.o

# Drivers

target/screen.o: drivers/screen.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

target/idt.o: drivers/idt.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

target/keyboard.o: drivers/keyboard.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

target/io_functions.o: drivers/io_functions.asm
	$(ASM) $< -f elf32 -o $@

driver_targets: target/screen.o target/idt.o target/keyboard.o target/io_functions.o

# Kernel

target/util.o: kernel/util.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

target/shell.o: kernel/shell.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

target/kernel.o: kernel/kernel.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@


target/load.o: kernel/memory/load_page_dir.asm
	$(ASM) $< -f elf32 -o $@

target/enable.o: kernel/memory/enable_paging.asm
	$(ASM) $< -f elf32 -o $@

target/hate.o: kernel/memory/i_hate_paging.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

kernel_targets: target/util.o target/shell.o target/load.o target/enable.o target/hate.o target/kernel.o

# Finalize

target/kernel.bin: boot/entry.o target/screen.o target/idt.o target/keyboard.o target/util.o target/shell.o target/load.o target/enable.o target/hate.o target/kernel.o target/io_functions.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

target/os-image: target/kernel.bin
	cp $^ $@

#target/os-image: target/bootstrap.bin target/kernel.bin
#	cat $^ > $@
