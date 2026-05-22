#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TP4");
MODULE_DESCRIPTION("Modulo basico TP4");

static int __init modulo_init(void)
{
    printk(KERN_INFO "Modulo TP4 cargado\n");
    return 0;
}

static void __exit modulo_exit(void)
{
    printk(KERN_INFO "Modulo TP4 descargado\n");
}

module_init(modulo_init);
module_exit(modulo_exit);