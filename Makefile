# SYNAPSE SO - Makefile
# Licensed under GPLv3

# Required tools:
# - gcc with 32-bit support (multilib)
# - nasm (Netwide Assembler)
# - GNU ld (linker)
# - grub-mkrescue (GRUB bootloader)
# - qemu-system-x86_64 (for testing)

# Compiler and tools
CC = gcc
AS = nasm
LD = ld
GRUB_MKRESCUE = grub-mkrescue

# Compiler flags
CFLAGS = -m32 -ffreestanding -nostdlib -fno-stack-protector -fno-pie -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T boot/linker.ld
ASFLAGS = -f elf32

# Directories
KERNEL_DIR = kernel
BOOT_DIR = boot
BUILD_DIR = build
ISO_DIR = isodir

# Source files
BOOT_ASM = $(BOOT_DIR)/boot.asm
KERNEL_ASM = $(KERNEL_DIR)/isr.asm
KERNEL_C = $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/vga.c $(KERNEL_DIR)/gdt.c $(KERNEL_DIR)/idt.c
KERNEL_LIB = $(KERNEL_DIR)/lib/string.c

# Object files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_ASM_OBJ = $(BUILD_DIR)/isr.o
KERNEL_C_OBJ = $(KERNEL_C:.c=.o)
KERNEL_C_OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(KERNEL_C_OBJ)))
KERNEL_LIB_OBJ = $(KERNEL_LIB:.c=.o)
KERNEL_LIB_OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(KERNEL_LIB_OBJ)))

# Output files
KERNEL_BIN = $(BUILD_DIR)/kernel.elf
ISO_IMAGE = synapse.iso

# Default target
all: $(ISO_IMAGE)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile boot assembly
$(BOOT_OBJ): $(BOOT_ASM) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile kernel assembly
$(KERNEL_ASM_OBJ): $(KERNEL_ASM) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile kernel C files (explicit rules to avoid pattern ambiguity)
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR)/include -c $< -o $@

$(BUILD_DIR)/vga.o: $(KERNEL_DIR)/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR)/include -c $< -o $@

$(BUILD_DIR)/gdt.o: $(KERNEL_DIR)/gdt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR)/include -c $< -o $@

$(BUILD_DIR)/idt.o: $(KERNEL_DIR)/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR)/include -c $< -o $@

# Compile library files
$(BUILD_DIR)/string.o: $(KERNEL_DIR)/lib/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR)/include -c $< -o $@

# Link kernel
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) $(KERNEL_LIB_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Create ISO image
$(ISO_IMAGE): $(KERNEL_BIN)
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.elf
	@echo "menuentry \"SYNAPSE SO\" {" > $(ISO_DIR)/boot/grub/grub.cfg
	@echo "    multiboot /boot/kernel.elf" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "    boot" >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo "}" >> $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

# Check for required tools
check-tools:
	@echo "Checking for required tools..."
	@command -v $(CC) >/dev/null 2>&1 || { echo "ERROR: gcc not found"; exit 1; }
	@$(CC) -m32 -xc /dev/null -c -o /dev/null 2>/dev/null || { echo "ERROR: gcc 32-bit support (multilib) not found"; exit 1; }
	@command -v $(AS) >/dev/null 2>&1 || { echo "ERROR: nasm not found"; exit 1; }
	@command -v $(LD) >/dev/null 2>&1 || { echo "ERROR: ld not found"; exit 1; }
	@command -v $(GRUB_MKRESCUE) >/dev/null 2>&1 || { echo "ERROR: grub-mkrescue not found"; exit 1; }
	@command -v qemu-system-x86_64 >/dev/null 2>&1 || { echo "WARNING: qemu-system-x86_64 not found (needed for 'make run')"; }
	@echo "All required tools are available."

# Run kernel in QEMU
run: $(ISO_IMAGE)
	qemu-system-x86_64 -cdrom $(ISO_IMAGE) -m 512M

# Run kernel with debug
debug: $(ISO_IMAGE)
	qemu-system-x86_64 -cdrom $(ISO_IMAGE) -m 512M -d int,cpu_reset

# Run kernel with GDB server
gdb: $(ISO_IMAGE)
	qemu-system-x86_64 -cdrom $(ISO_IMAGE) -m 512M -s -S

# Clean build files
clean:
	@rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO_IMAGE)

# Rebuild
rebuild: clean all

# Show size
size: $(KERNEL_BIN)
	size $(KERNEL_BIN)

# Help
help:
	@echo "SYNAPSE SO Build System"
	@echo "======================="
	@echo ""
	@echo "Required Tools:"
	@echo "  - gcc with 32-bit support"
	@echo "  - nasm (assembler)"
	@echo "  - GNU ld (linker)"
	@echo "  - grub-mkrescue"
	@echo "  - qemu-system-x86_64"
	@echo ""
	@echo "Targets:"
	@echo "  all         - Build kernel and ISO (default)"
	@echo "  check-tools - Verify required tools are installed"
	@echo "  run         - Run kernel in QEMU"
	@echo "  debug       - Run kernel in QEMU with debug output"
	@echo "  gdb         - Run kernel in QEMU with GDB server"
	@echo "  clean       - Remove build files"
	@echo "  rebuild     - Clean and rebuild"
	@echo "  size        - Show kernel size information"
	@echo "  help        - Show this help message"

.PHONY: all check-tools run debug gdb clean rebuild size help
