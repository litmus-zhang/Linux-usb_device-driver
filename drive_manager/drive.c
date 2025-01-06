// header file for linux USB drivers
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <sys/IBM_tape.h>

char vid[9];
char pid[17];
char revision[5];
struct inquiry_data inqdata;
printf("Issuing inquiry...\n");
memset(&inqdata, 0, sizeof(struct inquiry_data));
if (!ioctl (fd, SIOC_INQUIRY, &inqdata)) {
printf ("The SIOC_INQUIRY ioctl succeeded\n");
printf ("\nThe inquiry data is:\n");
/*-
* Just a dump byte won’t work because of the compiler
* bit field mapping
-*/
/* print out structure data field */
printf("\nInquiry Data:\n");
printf("Peripheral Qualifer-----------------0x%02x\n", inqdata.qual);
printf("Peripheral Device Type--------------0x%02x\n", inqdata.type);
printf("Removal Medium Bit------------------%d\n", inqdata.rm);
printf("Device Type Modifier----------------0x%02x\n", inqdata.mod);
printf("ISO version-------------------------0x%02x\n", inqdata.iso);
printf("ECMA version------------------------0x%02x\n", inqdata.ecma);
printf("ANSI version------------------------0x%02x\n", inqdata.ansi);
printf("Asynchronous Event Notification Bit-%d\n", inqdata.aenc);
printf("Terminate I/O Process Message Bit---%d\n", inqdata.trmiop);
printf("Response Data Format----------------0x%02x\n", inqdata.rdf);
printf("Additional Length-------------------0x%02x\n", inqdata.len);
printf("Medium Changer Mode-----------------0x%02x\n", inqdata.mchngr);
printf("Relative Addressing Bit-------------%d\n", inqdata.reladr);
printf("32 Bit Wide Data Transfers Bit------%d\n", inqdata.wbus32);
printf("16 Bit Wide Data Transfers Bit------%d\n", inqdata.wbus16);
printf("Synchronous Data Transfers Bit------%d\n", inqdata.sync);
printf("Linked Commands Bit-----------------%d\n", inqdata.linked);
printf("Command Queueing Bit----------------%d\n", inqdata.cmdque);
printf("Soft Reset Bit----------------------%d\n", inqdata.sftre);
strncpy(vid, inqdata.vid, 8);
vid[8] = ’\0’;
strncpy(pid, inqdata.pid, 16);
pid[16] = ’\0’;
strncpy(revision, inqdata.revision, 4);
revision[4] = ’\0’;
printf("Vendor ID-----------------------------%s\n", vid);
printf("Product ID----------------------------%s\n", pid);
printf("Product Revision Level----------------%s\n", revision);
dump_bytes(inqdata.vendor1, 20, "vendor1");
dump_bytes(inqdata.vendor2, 31, "vendor2");
}
else {
perror ("The SIOC_INQUIRY ioctl failed");
sioc_request_sense();
}

// SIOC RESERVER COMMAND
if (!ioctl (fd, SIOC_RESERVE, NULL)) {
printf ("The SIOC_RESERVE ioctl succeeded\n");
}
else {
perror ("The SIOC_RESERVE ioctl failed");
sioc_request_sense();
}



static struct usb_driver skel_driver = {
    .owner = THIS_MODULE,
    .name = "IBM tape reader driver",
    .id_table = skel_table,
    .probe = skel_probe,
    .disconnect = skel_disconnect,
};


static int __init usb_skel_init(void){
    int result;
    // register this driver with the USB subsystem
    result = usb_register(&skel_driver);
    if (result)
        err("usb_register failed. Error number %d", result);

    return result;
}

static void __exit usb_skel_exit(void){
    // deregister this driver with USB subsystem
    usb_deregister(&skel_driver);

}
// structure that describe a USB data transmission
struct urb;

// function for data transfer
int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe, void *data, int len, int *actual_length, int timeout);
int usb_control_msg(struct usb_device *usb_device, unsigned int pipe, __u8 request, __u8 requesttype, __u16 value, __u16 index, void *data, __u16 size, int timeout);