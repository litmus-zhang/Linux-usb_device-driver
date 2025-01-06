#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x82
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int Tape_open(struct inode *i, struct file *f)
{
    return 0;
}
static int Tape_close(struct inode *i, struct file *f)
{
    return 0;
}
static ssize_t Tape_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
    int retval;
    int read_cnt;

    /* Read the data from the bulk endpoint */
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN),
            bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt)))
    {
        return -EFAULT;
    }

    return MIN(cnt, read_cnt);
}
static ssize_t Tape_write(struct file *f, const char __user *buf, size_t cnt,
                                    loff_t *off)
{
    int retval;
    int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);

    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)))
    {
        return -EFAULT;
    }

    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
            bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }

    return wrote_cnt;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .oTape = Tape_open,
    .release = Tape_close,
    .read = Tape_read,
    .write = Tape_write,
};

static int Tape_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    int retval;

    device = interface_to_usbdev(interface);

    class.name = "usb/Tape%d";
    class.fops = &fops;
    if ((retval = usb_register_dev(interface, &class)) < 0)
    {
        /* Something prevented us from registering this driver */
        printk(KERN_ERR "Not able to get a minor for this device.");
    }
    else
    {
        printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
    }

    return retval;
}

static void Tape_disconnect(struct usb_interface *interface)
{
    usb_deregister_dev(interface, &class);
}

/* Table of devices that work with this driver */
static struct usb_device_id Tape_table[] =
{
    { USB_DEVICE(0x058F, 0x6387) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, Tape_table);

static struct usb_driver Tape_driver =
{
    .name = "Tape_driver",
    .probe = Tape_probe,
    .disconnect = Tape_disconnect,
    .id_table = Tape_table,
};

static int __init Tape_init(void)
{
    int result;

    /* Register this driver with the USB subsystem */
    if ((result = usb_register(&Tape_driver)))
    {
        printk(KERN_ERR "usb_register failed. Error number %d", result);
    }
    return result;
}

static void __exit Tape_exit(void)
{
    /* Deregister this driver with the USB subsystem */
    usb_deregister(&Tape_driver);
}

module_init(Tape_init);
module_exit(Tape_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Litmus Zhang <abdulsalamlukmon9@gmail.com>");
MODULE_DESCRIPTION("USB Tape Device Driver");