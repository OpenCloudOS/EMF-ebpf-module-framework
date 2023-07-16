TOPDIR ?= ../../
SYSROOT ?= $(TOPDIR)/sysroot

# Set USE_LIBCAP,USE_BFD,USE_DISASSEMBLER and USE_REALLOCARRAY to y to enable it
USE_BFD ?= y
USE_LIBCAP ?= y
USE_BPF_SKELS ?= y
USE_DISASSEMBLER ?= y
USE_REALLOCARRAY ?= n

# Used to generate vmlinux.h
BPF_VMLINUX_H ?= $(TOPDIR)/include/vmlinux.h
VMLINUX_BTF ?= /sys/kernel/btf/vmlinux

LIBBPF := $(SYSROOT)/lib/libbpf.a

VERSION := 5.4.110
BFD_OBJ = jit_disasm.o
BOOTSTRAP_DIR := bootstrap

CLANG ?= clang
LLVM_STRIP ?= llvm-strip
ifneq ($(LLVM),)
  HOSTCC ?= clang
else
  HOSTCC ?= gcc
endif

ALL_SRCS = $(wildcard *.c)
ALL_OBJS = $(patsubst %.c,%.o,$(ALL_SRCS))
BPFTOOL_OBJS := $(filter-out $(BFD_OBJ),$(ALL_OBJS)) disasm.o
BOOTSTRAP_OBJS = $(addprefix $(BOOTSTRAP_DIR)/,main.o common.o json_writer.o gen.o btf.o xlated_dumper.o btf_dumper.o disasm.o)

BPF_CFLAGS += -W -Wall -Wextra -Wno-unused-parameter
BPF_CFLAGS += -Wno-type-limits -Wno-missing-field-initializers
BPF_CFLAGS += -O2 $(EXTRA_CFLAGS) -DBPFTOOL_VERSION='"$(VERSION)"' -D__EXPORTED_HEADERS__ -DPACKAGE='"bpftool"'

BPF_CFLAGS += -I$(or ,.) -Ikernel/ -I$(SYSROOT)/include \
	      -I$(TOPDIR)/include -I$(TOPDIR)/include/uapi
SKELS_CFLAGS := -g -O2 -Wall -I$(or ,.) -I$(SYSROOT)/include -I$(TOPDIR)/include/uapi
BPF_LDFLAGS += $(EXTRA_LDFLAGS) -L$(SYSROOT)/lib

LIBS = $(LIBBPF) -lelf -lz 
ifeq ($(USE_LIBCAP),y)
LIBS += -lcap
BPF_CFLAGS += -DUSE_LIBCAP
endif

ifeq ($(USE_BFD),y)
BPFTOOL_OBJS += $(BFD_OBJ)
LIBS += -lbfd -ldl -lopcodes
BPF_CFLAGS += -DHAVE_LIBBFD_SUPPORT
endif

ifeq ($(USE_DISASSEMBLER),y)
BPF_CFLAGS += -DDISASM_FOUR_ARGS_SIGNATURE
endif

ifeq ($(USE_REALLOCARRAY),y)
BPF_CFLAGS += -DCOMPAT_NEED_REALLOCARRAY
endif

all: bpftool install

ifeq ($(USE_BPF_SKELS),y)
vmlinux.h: bootstrap
ifeq ($(BPF_VMLINUX_H),)
vmlinux.h: $(VMLINUX_BTF) bootstrap
	$(BOOTSTRAP_DIR)/bpftool btf dump file $< format c > $@
else
	@cp "$(BPF_VMLINUX_H)" $@
endif

%.bpf.o: skeleton/%.bpf.c vmlinux.h $(LIBBPF)
	$(CLANG) $(SKELS_CFLAGS) -target bpf -c $< -o $@
	$(LLVM_STRIP) -g $@

%.skel.h: %.bpf.o bootstrap
	$(BOOTSTRAP_DIR)/bpftool gen skeleton $< > $@

prog.o: profiler.skel.h

pids.o: pid_iter.skel.h
endif

bootstrap_init:
	@mkdir -p $(BOOTSTRAP_DIR)

bootstrap: $(BOOTSTRAP_DIR)/bpftool

bpftool: $(BPFTOOL_OBJS)
	$(CC) $(CFLAGS) $(BPF_CFLAGS) $(LDFLAGS) $(BPF_LDFLAGS) $(BPFTOOL_OBJS) $(LIBS) -o $@

$(BOOTSTRAP_DIR)/bpftool: $(BOOTSTRAP_OBJS)
	$(HOSTCC) $(CFLAGS) $(BPF_CFLAGS) $(LDFLAGS) $(BPF_LDFLAGS) $(BOOTSTRAP_OBJS) $(LIBS) -o $@

disasm.o:kernel/disasm.c
	$(CC) $(CFLAGS) $(BPF_CFLAGS) -c -MMD $< -o $@

$(BOOTSTRAP_DIR)/disasm.o:kernel/disasm.c
	$(HOSTCC) $(CFLAGS) $(BPF_CFLAGS) -c -MMD $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(BPF_CFLAGS) -c -MMD $< -o $@

$(BOOTSTRAP_DIR)/%.o: %.c | bootstrap_init
	$(HOSTCC) $(CFLAGS) $(BPF_CFLAGS) -c -MMD $< -o $@

install:
	@mkdir -p $(SYSROOT)/bin
	@mkdir -p $(SYSROOT)/include
	@mkdir -p $(SYSROOT)/bin/bootstrap
	cp -a bpftool $(SYSROOT)/bin/
	cp -a vmlinux.h $(SYSROOT)/include/
	cp -a $(BOOTSTRAP_DIR)/bpftool $(SYSROOT)/bin/bootstrap

clean:
	rm -rf bpftool *.o *.d *.skel.h
	rm -rf $(BOOTSTRAP_DIR) vmlinux.h

.PHONY:clean bootstrap bootstrap_init
