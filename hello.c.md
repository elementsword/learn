hello.c

```c
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void)
{
        printk(KERN_ERR "Hello World enter\n");
        return 0;
}

static void __exit hello_exit(void)
{
        printk(KERN_ERR "Hello World exit\n");
}
module_exit(hello_exit); //指定模块退出函数 
module_init(hello_init); //指定模块加载函数
MODULE_LICENSE("GPL v2");
```

模块加载函数 
当通过insmod或modprobe命令加载内核模块时，模块的加载函数会自动被内核执行，完成本模块的相关初始化工作。
Linux内核模块加载函数一般以__init标识声明，模块加载函数以“module_init（函数名）”的形式被指定。它返回整型值，若初始化成功，应返回0。而在初始化失败时，应该返回错误编码。

模块卸载函数

当通过rmmod命令卸载某模块时，模块的卸载函数会自动被内核执行，完成与模块卸载函数相反的功能。
Linux内核模块加载函数一般以__exit标识声明，模块卸载函数在模块卸载的时候执行，而不返回任何值，且必须以“module_exit（函数名）”的形式来指定。

```
KVERS = $(shell uname -r)
# Kernel modules
#将后面的东东编译为“内核模块”， obj-y 编译进内核，obj-n 不编译。
obj-m += hello.o

# 开启EXTRA_CFLAGS=-g-O0，可以得到包含调试信息的hello.ko模块。
EXTRA_CFLAGS=-g -O0
#-C 表示让 make 命令进入指定的内核源代码目录
build: kernel_modules
kernel_modules:
        make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
        make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
```

`KVERS = $(shell uname -r)`
动态获取当前系统的内核版本号（例如`5.4.0-80-generic`），用于定位内核头文件路径

`obj-m += hello.o`
指定将`hello.c`编译为可加载模块（.ko 文件）。若需将模块直接编译进内核，需改为`obj-y`，但此场景不适用动态模块加载。