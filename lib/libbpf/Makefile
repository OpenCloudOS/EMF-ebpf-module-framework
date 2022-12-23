TOPDIR ?= ..
SYSROOT ?= $(TOPDIR)/sysroot

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS += -g -O2 -Werror -Wall -std=gnu89
BPF_CFLAGS += $(CFLAGS) -fvisibility=hidden
BPF_LDFLAGS += $(LDFLAGS) $(EXTRA_LDFLAGS) -lelf -lz

BPF_CFLAGS += -I. -I$(TOPDIR)/include -I$(TOPDIR)/include/uapi
BPF_CFLAGS += -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 $(EXTRA_CFLAGS)

SHARED_OBJS := $(addprefix shared/,$(OBJS))
STATIC_OBJS := $(addprefix static/,$(OBJS))

all: libbpf.a libbpf.so install

init:
	@mkdir -p static shared

libbpf.a: $(STATIC_OBJS)
	$(AR) rcs $@ $^

libbpf.so: $(SHARED_OBJS)
	$(CC) -shared -Wl,--version-script=libbpf.map \
		  -Wl,-soname,libbpf.so $^ $(BPF_LDFLAGS) -o $@

static/%.o: %.c | init
	$(CC) $(BPF_CFLAGS) -c $< -o $@

shared/%.o: %.c | init
	$(CC) -fPIC $(BPF_CFLAGS) -c $< -o $@

install:
	@mkdir -p $(SYSROOT)/lib
	@mkdir -p $(SYSROOT)/include/bpf
	cp -a *.a *.so $(SYSROOT)/lib
	cp -a *.h $(SYSROOT)/include/bpf

clean:
	rm -rf static shared  *.a *.so *.o

.PHONY: clean install init
