# EMF(ebpf-module-framework)
EMF(ebpf-module-framework)是一个基eBPF的框架，主要用于提供一种规则，方便开发者可以更轻松的增加一个eBPF程序。

## EMF总体介绍
待续

## EMF模块介绍
EMF支持两种加载模式：一、EMF模块静态链接；二、EMF模块动态加载。通常，在入口函数main启动后(core/emf.c)，EMF框架会去主动查找静态模块，分别去执行静态模块的初始化函数和加载函数；然后EMF才会去读取动态模块，并处理动态模块的初始化和加载函数。所以这里有一个优先级的关系，即静态模块的优先级永远高于动态模块的优先级。

一、静态链接
```
EMF框架支持在代码中以静态模块的方式进行编写，具体语法如下：
struct module example_ebpf = { 定义一个eBPF的静态模块
	.prio = 1,                   定义该eBPF模块的加载优先级
	.name = "example",           定义该eBPF模块的名字
	.create = example_create,    定义该eBPF模块的初始化函数
	.attach = example_attach,    定义该eBPF模块的加载函数
	.destroy = example_destroy,  定义该eBPF模块的卸载函数
};
 
ebpf_module(example_ebpf);     将该eBPF模块插入到EMF框架

ebpf_module函数支持将eBPF静态模块连接到EMF框架当中。开发者只需要按照这种格式将eBPF模块写出来，然后将文件扔到modules目录下即可。在编译阶段，编译框架会将这个模块自动静态链接到EMF的对应位置。

详细请参考modules/example.c代码。
```
二、动态链接
```
EMF框架支持在代码中以动态模块的方式进行编写，原理同C语言加载so库文件，具体语法如下：
struct module example_ebpf = { 定义一个eBPF的静态模块
	.prio = 1,                   定义该eBPF模块的加载优先级
	.name = "example",           定义该eBPF模块的名字
	.create = example_create,    定义该eBPF模块的初始化函数
	.attach = example_attach,    定义该eBPF模块的加载函数
	.destroy = example_destroy,  定义该eBPF模块的卸载函数
};
 
ebpf_module(example_ebpf);     将该eBPF模块插入到EMF框架

ebpf_module函数同样也支持eBPF动态模块。开发者只需要按照这种格式将eBPF模块写出来，然后将对应的源文件扔到dynamic目录下即可。在编译阶段，编译框架会将这个模块自动编译成EMF框架支持的动态eBPF模块的格式。

详细请参考modules/example.c代码。
```

三、EMF框架API说明
```
待续
```
