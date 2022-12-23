#ifndef __EMF_H
#define __EMF_H

#include <stdio.h>
#include <linux/list.h>

struct module {
	struct list_head list;
	int (*create)(struct module *module);
	int (*attach)(struct module *module);
	int (*poll)(struct module *module);
	int (*notify)(struct module *module);
	int (*destroy)(struct module *module);
	char *name;
	int prio;
};

void ebpf_module_register(struct module *module);

#define ebpf_module(ebpf_m)							\
	static void __attribute__((constructor)) __register_##ebpf_m(void)	\
	{									\
		ebpf_module_register(&ebpf_m);					\
	}									\

#endif
