SNIPER_CC ?= gcc
SNIPER_CFLAGS := -O2 -Wall -Iinclude
SNIPER_LDFLAGS := 

SNIPER_SRCS := emf.c test.c
SNIPER_OBJS := $(SNIPER_SRCS:.c=.o)

emf: $(SNIPER_OBJS)
	$(SNIPER_CC) $(SNIPER_LDFLAGS) $^ -o $@

%.o: %.c
	$(SNIPER_CC) -c $^ $(SNIPER_CFLAGS) -o $@

clean:
	rm -rf *.o
	rm -rf emf

.PHONY: emf
