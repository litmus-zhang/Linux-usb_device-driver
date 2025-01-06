#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static struct usb_device *device;

static int Tape_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "Tape i/f %d now probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber,
            id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);

    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize,
                endpoint->wMaxPacketSize);
    }

    device = interface_to_usbdev(interface);
    return 0;
}

static void Tape_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Tape i/f %d now disconnected\n",
            interface->cur_altsetting->desc.bInterfaceNumber);
}

static struct usb_device_id Tape_table[] =
{
    { USB_DEVICE(0x058F, 0x6387) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, Tape_table);

static struct usb_driver Tape_driver =
{
    .name = "Tape_info",
    .probe = Tape_probe,
    .disconnect = Tape_disconnect,
    .id_table = Tape_table,
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
MODULE_DESCRIPTION("IBM Tape Info Driver");