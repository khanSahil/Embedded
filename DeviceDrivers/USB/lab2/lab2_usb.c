#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/mod_devicetable.h>
#include<linux/slab.h>
#include"lab_usb.h"
MODULE_LICENSE("GPL");

char *buff = NULL;

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

static void usb_disconnect(struct usb_interface *interface)
{
	printk("MY_USB_DISCONNECT : Device Removed\n");
} 

static int __init usb_init(void)
{	
	int result = usb_register(&usb_driver);
	if(result != SUCCESS){
		printk("ERROR : Couldn't register the USB Drive\n");
		return FAILURE;
	}
	printk("MY_USB_INIT : I am in USB_INIT function\n");
}
static void __exit usb_exit(void)
{
	kfree(buff);
	usb_deregister(&usb_driver);

	printk("MY_USB_EXIT : I am in USB_EXIT function\n");
	return SUCCESS;
}


// Printing the usb_device_id structure and all desciptors
static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{

	printk("\n***========USB_DEVICE_ID_STRUCTURE_BEGIN========***\n");
	printk("idVendor = 0x%x\n",id->idVendor);
	printk("idProduct = 0x%x\n",id->idProduct);
	printk("bcdDevice_lo = 0x%x\n",id->bcdDevice_lo);
	printk("bcdDevice_hi = 0x%x\n",id->bcdDevice_hi);
	
	printk("bDeviceSubClass = 0x%x\n",id->bDeviceClass);
	printk("bDeviceSubClass = 0x%x\n",id->bDeviceSubClass);
	printk("bDeviceProtocol = 0x%x\n",id->bDeviceProtocol);

	printk("bInterfaceClass = 0x%x\n",id->bInterfaceClass);
	printk("bInterfaceSubClass = 0x%x\n",id->bInterfaceSubClass);
	printk("bInterfaceProtocol = 0x%x\n",id->bInterfaceProtocol);
	
	printk("bInterfaceNumber = 0x%x\n",id->bInterfaceNumber);
	printk("***========USB_DEVICE_ID_STRUCTURE_END========***\n\n");
	
	struct usb_device *usbDevice = interface_to_usbdev(intf);
	buff = kmalloc(sizeof(usbDevice->descriptor),GFP_KERNEL);
	
	if(buff == NULL)
	{
		printk("Memory not allocated to recieve Descriptor\n");
		return -1;
	}
	
	int result = usb_get_descriptor(usbDevice,USB_DT_DEVICE,0,(void*)buff,sizeof(usbDevice->descriptor));
	if(result != sizeof(usbDevice->descriptor))
	{
		printk("Descriptor not Read\n");
		return -1;
	}
	printk("***========USB_DEVICE_DESCRIPTOR_STRUCTURE_BEGIN========***\n");
	printk("bLength = %d\n",usbDevice->descriptor.bLength);
	printk("bDescriptorType = 0x%x\n",usbDevice->descriptor.bDescriptorType);
	printk("bcdUSB = 0x%x\n",usbDevice->descriptor.bcdUSB);
	printk("bDeviceClass = 0x%x\n",usbDevice->descriptor.bDeviceClass);
	printk("bDeviceSubClass = %d\n",usbDevice->descriptor.bDeviceSubClass);
	printk("bDeviceProtocol = %d\n",usbDevice->descriptor.bDeviceProtocol);
	printk("bMaxPacketSize0 = %d\n",usbDevice->descriptor.bMaxPacketSize0);
	printk("idVendor = 0x%x\n",usbDevice->descriptor.idVendor);
	printk("idProduct = 0x%x\n",usbDevice->descriptor.idProduct);
	printk("bcdDevice = 0x%x\n",usbDevice->descriptor.bcdDevice);
	printk("iManufacturer = %d\n",usbDevice->descriptor.iManufacturer);
	printk("iProduct =%d\n",usbDevice->descriptor.iProduct);
	printk("iSerialNumber = %d\n",usbDevice->descriptor.iSerialNumber);
	printk("bNumConfigurations = %d\n",usbDevice->descriptor.bNumConfigurations);
	printk("***========USB_DEVICE_DESCRIPTOR_STRUCTURE_BEGIN========***\n\n");
	
	return SUCCESS;
}

module_init(usb_init);
module_exit(usb_exit);
