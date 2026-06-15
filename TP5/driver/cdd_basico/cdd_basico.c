#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo TP5");
MODULE_DESCRIPTION("Character Device Driver básico para TP5");

#define DEVICE_NAME "tp5"
#define CLASS_NAME  "tp5_class"

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

/* ============================================================
 * Operaciones del dispositivo
 * ============================================================ */

static int tp5_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "TP5: open()\n");
    return 0;
}

static int tp5_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "TP5: release()\n");
    return 0;
}

static ssize_t tp5_read(
    struct file *file,
    char __user *buf,
    size_t len,
    loff_t *offset)
{
    printk(KERN_INFO "TP5: read()\n");

    return 0;
}

static ssize_t tp5_write(
    struct file *file,
    const char __user *buf,
    size_t len,
    loff_t *offset)
{
    printk(KERN_INFO "TP5: write()\n");

    return len;
}

/* ============================================================
 * File operations
 * ============================================================ */

static struct file_operations tp5_fops = {
    .owner   = THIS_MODULE,
    .open    = tp5_open,
    .release = tp5_release,
    .read    = tp5_read,
    .write   = tp5_write,
};

/* ============================================================
 * Inicialización
 * ============================================================ */

static int __init tp5_init(void)
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "TP5: registrando character device\n");

    ret = alloc_chrdev_region(&first, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    cl = class_create(CLASS_NAME);
    if (IS_ERR(cl))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    dev_ret = device_create(
        cl,
        NULL,
        first,
        NULL,
        DEVICE_NAME);

    if (IS_ERR(dev_ret))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &tp5_fops);

    ret = cdev_add(&c_dev, first, 1);
    if (ret < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    printk(KERN_INFO "TP5: device creado correctamente\n");
    return 0;
}

/* ============================================================
 * Finalización
 * ============================================================ */

static void __exit tp5_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    printk(KERN_INFO "TP5: device eliminado\n");
}

module_init(tp5_init);
module_exit(tp5_exit);