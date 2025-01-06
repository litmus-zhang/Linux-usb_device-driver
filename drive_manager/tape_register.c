#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int Tape_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "Tape drive (%04X:%04X) plugged\n", id->idVendor,
                                id->idProduct);
    return 0;
}

static void Tape_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Tape drive removed\n");
}

static struct usb_device_id Tape_table[] =
{
    { USB_DEVICE(0x058F, 0x6387) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, Tape_table);

static struct usb_driver Tape_driver =
{
    .name = "Tape_driver",
    .id_table = Tape_table,
    .probe = Tape_probe,
    .disconnect = Tape_disconnect,
};

static int __init Tape_init(void)
{
    return usb_register(&Tape_driver);
}

static void __exit Tape_exit(void)
{
    usb_deregister(&Tape_driver);
}

module_init(Tape_init);
module_exit(Tape_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Litmus Zhang <abdulsalamlukmon9@gmail.com>");
MODULE_DESCRIPTION("USB Tape Registration Driver");