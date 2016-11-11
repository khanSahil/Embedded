#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/mod_devicetable.h>
#include"lab1_usb.h"
MODULE_LICENSE("GPL");

static struct usb_device_id usb_table [] = {
	{USB_DEVICE(USB_VENDOR_ID,USB_PRODUCT_ID)},
	{}
};

MODULE_DEVICE_TABLE(usb,usb_table);

static struct usb_driver usb_driver = {
	.name = "usb_skeleton",
	.id_table = usb_table,
	.probe = usb_probe,
	.disconnect = usb_disconnect,
};

static int __init usb_init(void)
{	
	int result = usb_register(&usb_driver);
	if(result == SUCCESS){
		printk("MY_USB_INIT : I am in USB_INIT function\n");
		return SUCCESS;
	}
	printk("ERROR : Couldn't register the USB Drive\n");
	return FAILURE;
}
static void __exit usb_exit(void)
{
	usb_deregister(&usb_driver);
	printk("MY_USB_EXIT : I am in USB_EXIT function\n");
	return SUCCESS;
}

static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	printk("MY_USB_PROBE : Device Detected vendor_id [0x%x] , Product_id [0x%x]\n",id->idVendor,id->idProduct);
	return SUCCESS;
}

static void usb_disconnect(struct usb_interface *interface)
{
	printk("MY_USB_DISCONNECT : Device Removed\n");
} 


module_init(usb_init);
module_exit(usb_exit);
