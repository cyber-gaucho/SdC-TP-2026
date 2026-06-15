#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo mate --refill");
MODULE_DESCRIPTION("Primer modulo del kernel para TP5");

static int __init hello_init(void)
{
    printk(KERN_INFO "TP5: modulo hello cargado\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "TP5: modulo hello descargado\n");
}

module_init(hello_init);
module_exit(hello_exit);
