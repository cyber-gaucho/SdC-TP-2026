#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo TP5");
MODULE_DESCRIPTION("Character Device Driver GPIO para TP5");

#define DEVICE_NAME "tp5"
#define CLASS_NAME  "tp5_class"

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

/* Canal seleccionado */
static char selected_channel = '1';

/* GPIO descriptors */
static struct gpio_desc *gpio17_desc;
static struct gpio_desc *gpio27_desc;

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
    char value;
    int gpio_value;

    printk(KERN_INFO "TP5: read()\n");

    if (*offset > 0)
        return 0;

    if (selected_channel == '1')
        gpio_value = gpiod_get_value(gpio17_desc);
    else
        gpio_value = gpiod_get_value(gpio27_desc);

    if (gpio_value < 0)
    {
        printk(KERN_ERR "TP5: error leyendo GPIO (%d)\n", gpio_value);
        return gpio_value;
    }

    value = gpio_value ? '1' : '0';

    if (copy_to_user(buf, &value, 1))
        return -EFAULT;

    (*offset)++;

    return 1;
}

static ssize_t tp5_write(
    struct file *file,
    const char __user *buf,
    size_t len,
    loff_t *offset)
{
    char cmd;

    printk(KERN_INFO "TP5: write()\n");

    if (len == 0)
        return 0;

    if (copy_from_user(&cmd, buf, 1))
        return -EFAULT;

    if (cmd == '1' || cmd == '2')
    {
        selected_channel = cmd;

        printk(KERN_INFO,
               "TP5: canal seleccionado -> GPIO%c\n",
               cmd);
    }
    else
    {
        printk(KERN_ERR "TP5: comando invalido '%c'\n", cmd);
        return -EINVAL;
    }

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

    printk(KERN_INFO "TP5: paso 1 - alloc_chrdev_region\n");

    ret = alloc_chrdev_region(&first, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_ERR
               "TP5: alloc_chrdev_region fallo (%d)\n",
               ret);
        return ret;
    }

    printk(KERN_INFO "TP5: paso 2 - class_create\n");

    cl = class_create(CLASS_NAME);
    if (IS_ERR(cl))
    {
        ret = PTR_ERR(cl);

        printk(KERN_ERR
               "TP5: class_create fallo (%d)\n",
               ret);

        unregister_chrdev_region(first, 1);
        return ret;
    }

    printk(KERN_INFO "TP5: paso 3 - device_create\n");

    dev_ret = device_create(
        cl,
        NULL,
        first,
        NULL,
        DEVICE_NAME);

    if (IS_ERR(dev_ret))
    {
        ret = PTR_ERR(dev_ret);

        printk(KERN_ERR
               "TP5: device_create fallo (%d)\n",
               ret);

        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    printk(KERN_INFO "TP5: paso 4 - cdev_init\n");

    cdev_init(&c_dev, &tp5_fops);

    printk(KERN_INFO "TP5: paso 5 - cdev_add\n");

    ret = cdev_add(&c_dev, first, 1);
    if (ret < 0)
    {
        printk(KERN_ERR
               "TP5: cdev_add fallo (%d)\n",
               ret);

        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    printk(KERN_INFO "TP5: paso 6 - gpio_to_desc(17)\n");

    gpio17_desc = gpio_to_desc(17);

    if (!gpio17_desc)
    {
        printk(KERN_ERR
               "TP5: gpio_to_desc(17) devolvio NULL\n");

        cdev_del(&c_dev);
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);

        return -ENODEV;
    }

    printk(KERN_INFO "TP5: GPIO17 descriptor OK\n");

    printk(KERN_INFO "TP5: paso 7 - gpio_to_desc(27)\n");

    gpio27_desc = gpio_to_desc(27);

    if (!gpio27_desc)
    {
        printk(KERN_ERR
               "TP5: gpio_to_desc(27) devolvio NULL\n");

        cdev_del(&c_dev);
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);

        return -ENODEV;
    }

    printk(KERN_INFO "TP5: GPIO27 descriptor OK\n");

    printk(KERN_INFO
           "TP5: escribir '1' selecciona GPIO17\n");

    printk(KERN_INFO
           "TP5: escribir '2' selecciona GPIO27\n");

    printk(KERN_INFO
           "TP5: device creado correctamente\n");

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