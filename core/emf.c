#include <stdio.h>
#include <emf.h>
#include <linux/list.h>

static struct list_head modules_list = LIST_HEAD_INIT(modules_list);

void ebpf_module_register(struct module *module)
{
	list_add_tail(&module->list, &modules_list);
}

static int ebpf_modules_create(void)
{
	int ret = 0;
	struct module *module;

	list_for_each_entry(module, &modules_list, list) {
		if (module->create) {
			ret = module->create(module);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

static int ebpf_modules_attach(void)
{
	int ret = 0;
	struct module *module;

	list_for_each_entry(module, &modules_list, list) {
		if (module->attach) {
			ret = module->attach(module);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

static int ebpf_modules_poll(void)
{
	int ret = 0;
	struct module *module;

	list_for_each_entry(module, &modules_list, list) {
		if (module->poll) {
			ret = module->poll(module);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

static int ebpf_modules_destroy(void)
{
	int ret = 0;
	struct module *module;

	list_for_each_entry(module, &modules_list, list) {
		if (module->destroy) {
			ret = module->destroy(module);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

int main(int argc, char *argv[])
{
	ebpf_modules_create();
	ebpf_modules_attach();
	ebpf_modules_poll();
	ebpf_modules_destroy();

	return 0;
}
