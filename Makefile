BUILD_DIR := ./build

QEMU = qemu-system-riscv64
QEMUOPTS = -machine virt -bios none -kernel $(BUILD_DIR)/kernel.img -m size=1G
QEMUOPTS_MM = -machine virt -bios none -kernel $(BUILD_DIR)/kernel.img -m size=3G -gdb tcp::1234
IMAGES = $(BUILD_DIR)/kernel.img

qemu: $(IMAGES)
	$(QEMU) $(QEMUOPTS)

qemu-nox: $(IMAGES)
	@echo "***"
	@echo "*** Use Ctrl-a x to exit qemu"
	@echo "***"
	$(QEMU) -nographic $(QEMUOPTS)

qemu-nox-mm: $(IMAGES)
	@echo "***"
	@echo "*** Use Ctrl-a x to exit qemu"
	@echo "***"
	$(QEMU) -nographic $(QEMUOPTS_MM)

qemu-gdb: $(IMAGES)
	@echo "***"
	@echo "*** Now run 'make gdb'." 1>&2
	@echo "***"
	$(QEMU) $(QEMUOPTS) -S  -gdb tcp::1234

qemu-nox-gdb: $(IMAGES)
	@echo "***"
	@echo "*** Now run 'make gdb'." 1>&2
	@echo "***"
	$(QEMU) -nographic $(QEMUOPTS) -S  -gdb tcp::1234

print-qemu:
	@echo $(QEMU)

print-gdbport:
	echo 1234

gdb:
	gdb-multiarch -n -x .gdbinit
