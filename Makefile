CC = gcc -fpic -g -O0
ASM = nasm -g
LD = ld

BOOT_DIR = boot
DRIVERS_DIR = drivers
KERNEL_DIR = kernel
TARGET_DIR = target

# RUNNABLE

deploy: $(TARGET_DIR)/os-image
redeploy: clean deploy

disk: deploy
	sh make_disk_image.sh

run_only:
	qemu-system-i386 -machine type=pc-i440fx-3.1 -kernel $(TARGET_DIR)/os-image

run: deploy run_only

debug_boot: deploy
	qemu-system-i386 -machine type=pc-i440fx-3.1 -D log.txt -hda $(TARGET_DIR)/disk.img -s -S

debug_only:
	qemu-system-i386 -machine type=pc-i440fx-3.1 -D log.txt -kernel $(TARGET_DIR)/os-image -s -S

debug: deploy debug_only

all: deploy

clean:
	rm -rf $(TARGET_DIR)/

# Boot

BOOT_SRCS_ASM = $(wildcard $(BOOT_DIR)/*.asm) $(wildcard $(BOOT_DIR)/*/*.asm)

BOOT_TARGETS = $(patsubst $(BOOT_DIR)/%.asm,$(TARGET_DIR)/%.o,$(BOOT_SRCS_ASM))

BOOT_SUB_DIRS = $(patsubst $(TARGET_DIR)/%,%,$(dir $(BOOT_TARGETS)))

$(TARGET_DIR)/%.o: $(BOOT_DIR)/%.asm
	$(ASM) $< -f elf32 -o $@

# Drivers

DRIVERS_SRCS_C   = $(wildcard $(DRIVERS_DIR)/*.c) $(wildcard $(DRIVERS_DIR)/*/*.c)
DRIVERS_SRCS_ASM = $(wildcard $(DRIVERS_DIR)/*.asm) $(wildcard $(DRIVERS_DIR)/*/*.asm)

DRIVERS_TARGETS = $(patsubst $(DRIVERS_DIR)/%.c,$(TARGET_DIR)/%.o,$(DRIVERS_SRCS_C)) \
                 $(patsubst $(DRIVERS_DIR)/%.asm,$(TARGET_DIR)/%.o,$(DRIVERS_SRCS_ASM))

DRIVERS_SUB_DIRS = $(patsubst $(TARGET_DIR)/%,%,$(dir $(DRIVERS_TARGETS)))

$(TARGET_DIR)/%.o: $(DRIVERS_DIR)/%.asm
	$(ASM) $< -f elf32 -o $@

$(TARGET_DIR)/%.o: $(DRIVERS_DIR)/%.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

# Kernel

KERNEL_SRCS_C   = $(wildcard $(KERNEL_DIR)/*.c) $(wildcard $(KERNEL_DIR)/*/*.c)
KERNEL_SRCS_ASM = $(wildcard $(KERNEL_DIR)/*.asm) $(wildcard $(KERNEL_DIR)/*/*.asm)

KERNEL_TARGETS = $(patsubst $(KERNEL_DIR)/%.c,$(TARGET_DIR)/%.o,$(KERNEL_SRCS_C)) \
                 $(patsubst $(KERNEL_DIR)/%.asm,$(TARGET_DIR)/%.o,$(KERNEL_SRCS_ASM))

KERNEL_SUB_DIRS = $(patsubst $(TARGET_DIR)/%,%,$(dir $(KERNEL_TARGETS)))

$(TARGET_DIR)/%.o: $(KERNEL_DIR)/%.asm
	$(ASM) $< -f elf32 -o $@

$(TARGET_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

# Finalize

ALL_TARGETS = $(BOOT_TARGETS) $(DRIVERS_TARGETS) $(KERNEL_TARGETS)

prep_target_subdirs:
	mkdir -p $(dir $(ALL_TARGETS))

compile_os: prep_target_subdirs $(ALL_TARGETS)

$(TARGET_DIR)/os-image: compile_os
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $(ALL_TARGETS)
