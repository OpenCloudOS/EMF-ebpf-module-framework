export TOPDIR=$(shell pwd)
export SYSROOT=$(TOPDIR)/sysroot

CC ?= gcc 
STATIC ?= 0
CLANG ?= clang
LLVM_STRIP ?= llvm-strip

SNIPER_CFLAGS += -g -Wall -O2 -I. -Wno-pointer-sign -Wno-unused-value \
          -Wno-incompatible-pointer-types-discards-qualifiers  \
          -fno-asynchronous-unwind-tables -Wno-unknown-attributes \
          -I$(SYSROOT)/include -I$(TOPDIR)/include/ -I$(TOPDIR)/include/uapi


SHARED_LDFLAGS += -L$(SYSROOT)/lib -lbpf
STATIC_LDFLAGS += -static $(SYSROOT)/lib/libbpf.a -lelf -lz

core-src := core/emf.c 
modules-src := $(wildcard modules/*.c)

all: bpf emf

emf: $(core-src:.c=.o) $(modules-src:.c=.o)
ifeq ($(STATIC),1)
	$(CC) $(CFLAGS) $(SNIPER_CFLAGS) $^ $(LDFLAGS) $(STATIC_LDFLAGS) -o $@
else
	$(CC) $(CFLAGS) $(SNIPER_CFLAGS) $^ $(LDFLAGS) $(SHARED_LDFLAGS) -o $@
endif

bpf: libbpf bpftools

libbpf:
	@mkdir -p $(SYSROOT)
	make -C lib/libbpf all

bpftools: libbpf
	@mkdir -p $(SYSROOT)
	make -C tools/bpftool all

%.o:%.c
	$(CC) -c $(SNIPER_CFLAGS) $^ -o $@	

clean:
	rm -rf emf $(SYSROOT)
	make -C lib/libbpf clean
	make -C tools/bpftool clean
	rm -rf $(core-src:.c=.o) $(modules-src:.c=.o)

.PHONY: emf
