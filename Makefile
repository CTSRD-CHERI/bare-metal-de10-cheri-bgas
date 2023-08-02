RVPFX = riscv64-unknown-elf
RVCC ?= $(RVPFX)-gcc
RVAR ?= $(RVPFX)-ar
RVOBJDUMP ?= $(RVPFX)-objdump
BUILD_DIR ?= $(CURDIR)/build

CHERIBGAS_DE10_BARE_DIR = $(CURDIR)/libcheribgas_de10_bare
CHERIBGAS_DE10_BARE_ASMSRCS = init.S
CHERIBGAS_DE10_BARE_CSRCS = cheribgas_de10_bare.c uart16550.c

CFLAGS=-ffreestanding
CFLAGS+=-nostdlib
CFLAGS+=-nostartfiles
CFLAGS+=-mcmodel=medany
CFLAGS+=-O1

EXENAME=bare-main.elf

all: $(EXENAME)

$(EXENAME): main.c libcheribgas_de10_bare.a
	$(RVCC) $(CFLAGS) -I$(CHERIBGAS_DE10_BARE_DIR) -Tlink.ld -o $@ $^ -L. -lcheribgas_de10_bare
	$(RVOBJDUMP) -d $@ > $@.dump

libcheribgas_de10_bare.a: $(patsubst %.S, $(BUILD_DIR)/%.o, $(CHERIBGAS_DE10_BARE_ASMSRCS)) $(patsubst %.c, $(BUILD_DIR)/%.o, $(CHERIBGAS_DE10_BARE_CSRCS))
	$(RVAR) rcs $@ $^

$(BUILD_DIR)/%.o: $(addprefix $(CHERIBGAS_DE10_BARE_DIR)/, %.c)
	@mkdir -p $(BUILD_DIR)
	$(RVCC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(addprefix $(CHERIBGAS_DE10_BARE_DIR)/, %.S)
	@mkdir -p $(BUILD_DIR)
	$(RVCC) $(CFLAGS) -c -o $@ $<

# https://stackoverflow.com/questions/31390127/how-can-i-compile-c-code-to-get-a-bare-metal-skeleton-of-a-minimal-risc-v-assemb
#
# riscv64-unknown-elf-gcc -nostdlib -nostartfiles -Tlink.ld -o hello hello.c uart16550.c
# riscv64-unknown-elf-objdump -d hello
#
# the -mcmodel=medany : https://forums.sifive.com/t/relocation-truncated-to-fit-r-riscv-hi20/2430/2

.PHONY: clean mrproper

clean:
	rm -rf $(BUILD_DIR)

mrproper: clean
	rm -rf *.a $(EXENAME) $(EXENAME).dump
