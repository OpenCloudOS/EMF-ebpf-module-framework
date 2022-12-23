#include <emf.h>

static int example_create(struct module *module)
{
	printf("example create\n");
	return 0;
}

static int example_attach(struct module *module)
{
	printf("example attach\n");
	return 0;
}

static int example_destroy(struct module *module)
{
	printf("example destroy\n");
	return 0;
}

struct module example_ebpf = {
	.prio = 1,
	.name = "example",
	.create = example_create,
	.attach = example_attach,
	.destroy = example_destroy,
};
 
ebpf_module(example_ebpf);
